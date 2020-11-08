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
        std::vector<llvm::Type*> llvmArgumentTypes{};

        llvmArgumentTypes.reserve(argsNativeMap.size());

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

        this->typeSymbolTable.set(construct, llvmFunctionType);
    }

    void LlvmLoweringPass::visitFunction(std::shared_ptr<Function> construct) {
        /**
         * Special treatment for the main/entry point function, as
         * it should not be mangled.
         */
        std::string name = construct->prototype->isMain()
            ? construct->prototype->name

            : NameMangler::mangle(
                this->localBuffers.modules.forceGetTopItem(),
                construct->prototype->name
            );

        // TODO: Should this be enforced when emitting, or should only be during type checking?
        // A function with a matching identifier already exists.
        if (this->llvmBuffers.modules.forceGetTopItem()->getFunction(name)
            != nullptr) {
            this->context->diagnosticBuilder
                ->bootstrap(diagnostic::functionRedefinition)
                ->formatMessage(construct->prototype->name)
                ->finish();

            // TODO
            throw std::runtime_error("Awaiting new diagnostic buffer checking");
        }

        /**
         * Cast the LLVM value to a LLVM function, since we know
         * getCallee() will return a function.
         */
        auto* llvmFunction = llvm::dyn_cast<llvm::Function>(
            this->llvmBuffers.modules.forceGetTopItem()->getOrInsertFunction(
                name,
                this->eagerVisitType<llvm::FunctionType>(construct->prototype)
            ).getCallee()
        );

        this->llvmBuffers.functions.push(llvmFunction);
        this->valueSymbolTable.set(construct, llvmFunction);

        // TODO: Default to private linkage, but export if applicable (through export: module syntax).
        llvmFunction->setLinkage(llvm::GlobalValue::LinkageTypes::ExternalLinkage);

        // TODO: What about variadic functions?
        // Begin processing arguments. Argument count must be the same.
        if (construct->prototype->args->items->getSize()
            != llvmFunction->arg_size()) {
            // TODO: Use diagnostics API.
            throw std::runtime_error("Expected argument count to be the same as the function's argument count");
        }

        auto& argsNativeMap = construct->prototype->args->items->unwrapConst();
        size_t argCounter = 0;
        size_t llvmArgCounter = 0;

        // TODO: Simplify method of naming LLVM arguments, as this implementation is (probably) inefficient.
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

        /**
         * Entry section must be set. Redundant check, since the verify should
         * function ensure that the block contains a single entry section, but
         * just to make sure.
         */
        if (construct->basicBlocks.empty()) {
            // TODO: Use diagnostics API (internal error?).
            throw std::runtime_error("Block must have at least a single basic block");
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
    }

    void LlvmLoweringPass::visitMethod(std::shared_ptr<Method> construct) {
        // TODO: Append (%StructTypeName* %this) to the prototype's args.
//        this->llvmBuffers.modules.forceGetTopItem()->getOrInsertFunction(
//            NameMangler::mangle(construct->structType->typeName),
//        );
    }
}
