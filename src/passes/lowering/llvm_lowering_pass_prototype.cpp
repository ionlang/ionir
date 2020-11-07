#include <iostream>
#include <llvm/ADT/APInt.h>
#include <llvm/IR/DerivedTypes.h>
#include <ionir/construct/value.h>
#include <ionir/diagnostics/diagnostic.h>
#include <ionir/tracking/name_mangler.h>
#include <ionir/passes/semantic/construct_verification_pass.h>
#include <ionir/passes/lowering/llvm_lowering_pass.h>

namespace ionir {
    void LlvmLoweringPass::visit(std::shared_ptr<Construct> node) {
        // TODO: Verify '||' vs. ('==' & '&&') precedence.
        bool visited =
            node->constructKind == ConstructKind::Module
                && this->moduleSymbolTable.contains(node->dynamicCast<Module>())

            || this->valueSymbolTable.contains(node)
            || this->typeSymbolTable.contains(node);

        // Do not visit any construct more than once.
        if (visited) {
            return;
        }

        /**
         * Only instruct the construct to visit this instance and not
         * its children, since they're already manually visited by
         * the other member methods.
         */
        node->accept(*this);
    }

    bool LlvmLoweringPass::initialize(ionshared::PassInfo& info) {
        /**
         * In order to prevent segmentation faults, the construct
         * tree must be validated before running this pass.
         */
        info.addRequirement<ConstructVerificationPass>();

        return true;
    }

    void LlvmLoweringPass::finish() {
        bool isBuffersEmpty = this->llvmBuffers.modules.isEmpty()
            && this->llvmBuffers.functions.isEmpty()
            && this->llvmBuffers.basicBlocks.isEmpty();

        // At this point, all LLVM buffers should be empty.
        if (!isBuffersEmpty) {
            // TODO: Use diagnostics API.
            std::cout << "Internal warning: LLVM buffers were not empty upon finish" << std::endl;
        }
    }

    void LlvmLoweringPass::visitExtern(std::shared_ptr<Extern> construct) {
        // TODO: Should we be checking whether a function exists/has been emitted via name/id? Or using symbol table? Investigate.
        if (this->llvmBuffers.modules.forceGetTopItem()
            ->getFunction(construct->prototype->name) != nullptr) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::externRedefinition)
                ->finish();

            throw std::runtime_error("Awaiting new diagnostic buffer checking");
        }

        this->valueSymbolTable.set(
            construct,
            this->eagerVisitValue(construct->prototype)
        );
    }

    void LlvmLoweringPass::visitPrototype(std::shared_ptr<Prototype> construct) {
        auto& argsNativeMap = construct->args->items->unwrapConst();
        uint32_t argumentCount = construct->args->items->getSize();
        std::vector<llvm::Type*> llvmArgumentTypes{};

        std::shared_ptr<llvm::Module> llvmModuleBuffer =
            this->llvmBuffers.modules.forceGetTopItem();

        // TODO: Name mangling is being used. Verify through another method/way (or mangle the name).
        llvm::Function* llvmFunction = llvmModuleBuffer->getFunction(construct->name);

        // A function with a matching identifier already exists.
        if (llvmFunction != nullptr) {
            // Function already has a body, disallow re-definition.
            if (llvmFunction->getBasicBlockList().empty()) {
                throw std::runtime_error("Cannot re-define function");
            }
            // If the function takes a different number of arguments, reject.
            else if (llvmFunction->arg_size() != argumentCount) {
                this->context->diagnosticBuilder
                    ->bootstrap(diagnostic::functionRedefinitionDiffArgs)
                    ->finish();

                // TODO
                throw std::runtime_error("Awaiting new diagnostic buffer checking");
            }
        }
        // Otherwise, function will be created.
        else {
            for (const auto& [id, arg] : argsNativeMap) {
                llvmArgumentTypes.push_back(
                    this->eagerVisitType(arg.first)
                );
            }

            llvm::FunctionType* llvmFunctionType = llvm::FunctionType::get(
                this->eagerVisitType(construct->returnType),
                llvmArgumentTypes,
                construct->args->isVariable
            );

            std::string name = construct->isMain()
                ? construct->name

                : NameMangler::mangle(
                    this->localBuffers.modules.forceGetTopItem(),
                    construct->name
                );

            /**
             * Cast the LLVM value to a LLVM function, since we know
             * getCallee() will return a function.
             */
            llvmFunction = llvm::dyn_cast<llvm::Function>(
                llvmModuleBuffer->getOrInsertFunction(
                    name,
                    llvmFunctionType
                ).getCallee()
            );

            // Set the function's linkage.
            llvmFunction->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);
        }

        // Begin processing arguments. Argument count must be the same.
        if (argumentCount != llvmFunction->arg_size()) {
            throw std::runtime_error("Expected argument count to be the same as the function's argument count");
        }

        size_t argCounter = 0;
        size_t llvmArgCounter = 0;

        // TODO: Simplify method of naming LLVM arguments, as this implementation is inefficient.
        for (const auto& [id, arg] : argsNativeMap) {
            for (auto& llvmArgument : llvmFunction->args()) {
                if (llvmArgCounter == argCounter) {
                    llvmArgument.setName(id);
                    llvmArgCounter = 0;

                    break;
                }

                llvmArgCounter++;
            }

            argCounter++;
        }

        this->valueSymbolTable.set(construct, llvmFunction);
    }

    void LlvmLoweringPass::visitFunction(std::shared_ptr<Function> construct) {
        std::shared_ptr<llvm::Module> llvmModuleBuffer =
            this->llvmBuffers.modules.forceGetTopItem();

        // TODO: Name mangling is being used. Verify through another method/way (or mangle the name).
        if (llvmModuleBuffer->getFunction(construct->prototype->name) != nullptr) {
            throw std::runtime_error("A function with the same identifier has been already previously defined");
        }

        auto* llvmFunction =
            this->eagerVisitValue<llvm::Function>(construct->prototype);

        this->llvmBuffers.functions.push(llvmFunction);

        /**
         * Entry section must be set. Redundant check, since the verify should
         * function ensure that the block contains a single entry section, but
         * just to make sure.
         */
        if (construct->basicBlocks.empty()) {
            throw std::runtime_error("No entry basic block exists for block");
        }

        std::shared_ptr<BasicBlock> entryBasicBlock =
            construct->basicBlocks.front();

        // TODO: What about the other basic blocks?
        /**
         * The function body's entry basic block contains no terminal instruction.
         * The parent function's prototype's return type is void. Implicitly append
         * a return void instruction to satisfy LLVM's terminal instruction requirement.
         */
        if (!entryBasicBlock->hasTerminalInst()
            && construct->prototype->returnType->typeKind == TypeKind::Void) {
            entryBasicBlock->appendInst(Construct::makeChild<ReturnInst>(
                entryBasicBlock
            ));
        }

        for (const auto& basicBlock : construct->basicBlocks) {
            this->visit(basicBlock);
        }

        // TODO: Verify the resulting LLVM function (through LLVM)?

        this->llvmBuffers.functions.forcePop();
        this->valueSymbolTable.set(construct, llvmFunction);
    }
}
