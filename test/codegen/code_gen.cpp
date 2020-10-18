#include <vector>
#include <ionir/passes/lowering/llvm_lowering_pass.h>
#include <ionir/const/const.h>
#include <ionir/test/bootstrap.h>
#include <ionir/test/comparison.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

TEST(CodeGenTest, VisitExtern) {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = test::bootstrap::llvmLoweringPass();
    std::shared_ptr<VoidType> returnType = TypeFactory::typeVoid();
    std::shared_ptr<Args> args = std::make_shared<Args>();

    std::shared_ptr<Prototype> prototype =
        std::make_shared<Prototype>(test::constant::foobar, args, returnType, nullptr);

    // TODO: Nullptr parent.
    std::shared_ptr<Extern> externConstruct = std::make_shared<Extern>(nullptr, prototype);

    llvmLoweringPass->visitExtern(externConstruct);

    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "extern_simple"));
}

TEST(CodeGenTest, VisitEmptyFunction) {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = test::bootstrap::llvmLoweringPass();
    std::shared_ptr<VoidType> returnType = TypeFactory::typeVoid();

    std::shared_ptr<Prototype> prototype =
        std::make_shared<Prototype>(test::constant::foobar, std::make_shared<Args>(), returnType, nullptr);

    std::shared_ptr<FunctionBody> body = std::make_shared<FunctionBody>(nullptr);

    std::shared_ptr<BasicBlock> section = std::make_shared<BasicBlock>(BasicBlockOpts{
        body,
        BasicBlockKind::Entry,
        "entry"
    });

    // TODO: Fix mumbo-jumbo debugging code.
    // --------------------------

    typedef PtrSymbolTable<BasicBlock> t;
    typedef ionshared::SymbolTable<std::shared_ptr<BasicBlock>> tt;

    auto sectionsVal = std::map<std::string, std::shared_ptr<BasicBlock>>{
        {Const::basicBlockEntryId, section}
    };

    t sectionsMap = std::make_shared<tt>(sectionsVal);

    // ---------------------------

    body->setSymbolTable(sectionsMap);

    std::shared_ptr<Function> function = std::make_shared<Function>(prototype, body);

    body->parent = function;
    llvmLoweringPass->visitFunction(function);

    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "function_empty"));
}

TEST(CodeGenTest, VisitEmptyGlobal) {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = test::bootstrap::llvmLoweringPass();
    std::shared_ptr<IntegerType> type = TypeFactory::typeInteger(IntegerKind::Int32);
    std::shared_ptr<Global> globalVar = std::make_shared<Global>(type, test::constant::foobar);

    llvmLoweringPass->visitGlobal(globalVar);

    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "global_empty"));
}

TEST(CodeGenTest, VisitGlobalWithValue) {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = test::bootstrap::llvmLoweringPass();
    std::shared_ptr<IntegerType> type = TypeFactory::typeInteger(IntegerKind::Int32);

    std::shared_ptr<Global> globalVar = std::make_shared<Global>(
        type,
        test::constant::foobar,
        std::make_shared<IntegerLiteral>(type, 123)->staticCast<Value<>>()
    );

    llvmLoweringPass->visitGlobal(globalVar);

    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "global_init"));
}

TEST(CodeGenTest, VisitAllocaInst) {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = test::bootstrap::llvmLoweringPass();

    std::vector<std::shared_ptr<Instruction>> insts = {
        std::make_shared<AllocaInst>(AllocaInstOpts{
            nullptr,
            TypeFactory::typeInteger(IntegerKind::Int32)
        })
    };

    std::shared_ptr<Function> function = test::bootstrap::emptyFunction(insts);

    llvmLoweringPass->visitFunction(function);

    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "inst_alloca"));
}

TEST(CodeGenTest, VisitReturnInst) {
    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass = test::bootstrap::llvmLoweringPass();

    std::vector<std::shared_ptr<Instruction>> insts = {
        std::make_shared<ReturnInst>(ReturnInstOpts{
            nullptr,

            std::make_shared<IntegerLiteral>(
                TypeFactory::typeInteger(IntegerKind::Int32),
                1
            )->flatten()
        })
    };

    std::shared_ptr<Function> function = test::bootstrap::emptyFunction(insts);

    llvmLoweringPass->visitFunction(function);

    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "inst_return_i32"));
}

// TODO: Migrated NameResolutionPass.
//TEST(CodeGenTest, VisitAllocaStoreReturnRef) {
//    std::shared_ptr<NameResolutionPass> nameResolutionPass = std::make_shared<NameResolutionPass>();
//    std::shared_ptr<LlvmCodegenPass> llvmCodegenPass = test::bootstrap::llvmCodegenPass();
//
//    llvmCodegenPass->visitRegisterAssign(std::make_shared<RegisterAssign>(
//        test::constant::foo,
//        nullptr
//    ));
//
//    // TODO: Return value type is being reused, even tho in both contexts it's independent (alloca inst, and return inst).
//    std::shared_ptr<IntegerType> returnValueType = TypeFactory::typeInteger(IntegerKind::Int32);
//
//    std::shared_ptr<Function> function = test::bootstrap::emptyFunction();
//    ionshared::OptPtr<BasicBlock> functionEntryBlock = function->getBody()->findEntryBasicBlock();
//
//    /**
//     * Entry basic block must be set in the bootstrapped function
//     * in order to perform the test.
//     */
//    EXPECT_TRUE(ionshared::util::hasValue(functionEntryBlock));
//
//    std::shared_ptr<AllocaInst> allocaInst = std::make_shared<AllocaInst>(AllocaInstOpts{
//        /**
//         * The alloca instruction needs it's parent to be set in order
//         * to be resolved.
//         */
//        *functionEntryBlock,
//
//        returnValueType
//    });
//
//    PtrRef<AllocaInst> allocaInstRef1 = std::make_shared<Ref<AllocaInst>>(
//        RefKind::Inst,
//        test::constant::foo,
//        nullptr,
//        allocaInst
//    );
//
//    std::shared_ptr<StoreInst> storeInst = std::make_shared<StoreInst>(StoreInstOpts{
//        // No need for parent to be set.
//        nullptr,
//
//        std::make_shared<IntegerLiteral>(returnValueType, 1)->staticCast<Value<>>(),
//        allocaInstRef1
//    });
//
//    allocaInstRef1->setOwner(storeInst);
//
//    PtrRef<AllocaInst> allocaInstRef2 = std::make_shared<Ref<AllocaInst>>(
//        RefKind::Inst,
//        test::constant::foo,
//        nullptr,
//        allocaInst
//    );
//
//    std::shared_ptr<ReturnInst> returnInst = std::make_shared<ReturnInst>(ReturnInstOpts{
//        /**
//         * The return instruction needs it's parent to be set in order
//         * for its return value reference to be resolved.
//         */
//        *functionEntryBlock,
//
//        allocaInstRef2
//    });
//
//    /**
//     * Associate the return instruction's return value reference with
//     * itself in order for it to be resolved.
//     */
//    allocaInstRef2->setOwner(returnInst);
//
//    std::vector<std::shared_ptr<Inst>> insts = {
//        allocaInst,
//        storeInst,
//        returnInst
//    };
//
//    // Resolve references first.
//    nameResolutionPass->visitRef(allocaInstRef1->staticCast<Ref<>>());
//    nameResolutionPass->visitRef(allocaInstRef2->staticCast<Ref<>>());
//
//    /**
//     * Associate the instructions with the function's body,
//     * and visit the function.
//     */
//    functionEntryBlock->get()->setInsts(insts);
//    llvmCodegenPass->visitFunction(function);
//
//    EXPECT_TRUE(test::compare::ir(llvmCodegenPass, "inst_alloca_store_return_ref"));
//}
//
//TEST(CodeGenTest, VisitBranchInst) {
//    PassManager passManager = PassManager();
//
//    passManager.registerPass(std::make_shared<NameResolutionPass>());
//
//    std::shared_ptr<BasicBlock> body = std::make_shared<BasicBlock>(BasicBlockOpts{
//        nullptr,
//        BasicBlockKind::Label,
//        "if_body",
//        {}
//    });
//
//    std::shared_ptr<BooleanLiteral> condition = std::make_shared<BooleanLiteral>(true);
//    PtrRef<BasicBlock> bodyBasicBlockRef = std::make_shared<Ref<BasicBlock>>(body);
//
//    // TODO: Use some sort of factory design pattern.
//    auto branchInst = std::make_shared<BranchInst>(BranchInstOpts{
//        body,
//        condition,
//
//        /**
//         * Point both body and otherwise sections to
//         * the same section for testing purposes. LLVM
//         * is smart enough to create a different label,
//         * even if both attempt to reference the same label.
//         * Unsure however how it would handle splitting of
//         * instructions if there were to be contained inside
//         * the section once it creates the new label.
//         */
//        bodyBasicBlockRef,
//        bodyBasicBlockRef
//    });
//
//    branchInst->getConsequentBasicBlockRef()->setOwner(branchInst);
//    branchInst->getAlternativeBasicBlockRef()->setOwner(branchInst);
//
//    std::shared_ptr<LlvmCodegenPass> llvmCodegenPass = test::bootstrap::llvmCodegenPass();
//
//    std::shared_ptr<Function> function = test::bootstrap::emptyFunction({
//        branchInst
//    });
//
//    passManager.run({
//        function
//    });
//
//    llvmCodegenPass->visitFunction(function);
//
//    EXPECT_TRUE(test::compare::ir(llvmCodegenPass, "inst_branch"));
//}
