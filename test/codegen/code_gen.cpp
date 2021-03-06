#include <vector>
#include <ionir/passes/lowering/llvm_lowering_pass.h>
#include <ionir/const/const.h>
#include <ionir/test/bootstrap.h>
#include <ionir/test/comparison.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

TEST(CodeGenTest, VisitExtern) {
    std::shared_ptr<Module> module = std::make_shared<Module>(
        std::make_shared<Identifier>(test::constant::bar)
    );

    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
        test::bootstrap::llvmLoweringPass(module);

    std::shared_ptr<Extern> externConstruct = Extern::make(
        Prototype::make(
            test::constant::foo,
            std::make_shared<Args>(),
            std::make_shared<VoidType>()
        )
    );

    externConstruct->parent = module;
    llvmLoweringPass->visitExtern(externConstruct);
    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "extern_simple"));
}

TEST(CodeGenTest, VisitEmptyFunction) {
    std::shared_ptr<Module> module = std::make_shared<Module>(
        std::make_shared<Identifier>(test::constant::bar)
    );

    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
        test::bootstrap::llvmLoweringPass(module);

    std::shared_ptr<Function> function = Function::make(
        Prototype::make(
            test::constant::foo,
            std::make_shared<Args>(),
            std::make_shared<VoidType>()
        ),

        std::vector<std::shared_ptr<BasicBlock>>{BasicBlock::make()}
    );

    function->parent = module;
    llvmLoweringPass->visitFunction(function);
    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "function_empty"));
}

TEST(CodeGenTest, VisitEmptyGlobal) {
    std::shared_ptr<Module> module = std::make_shared<Module>(
        std::make_shared<Identifier>(test::constant::bar)
    );

    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
        test::bootstrap::llvmLoweringPass(module);

    std::shared_ptr<Global> globalVar = Global::make(
        std::make_shared<IntegerType>(IntegerKind::Int32),
        test::constant::foo
    );

    globalVar->parent = module;
    llvmLoweringPass->visitGlobal(globalVar);
    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "global_empty"));
}

TEST(CodeGenTest, VisitGlobalWithValue) {
    std::shared_ptr<Module> module = std::make_shared<Module>(
        std::make_shared<Identifier>(test::constant::bar)
    );

    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
        test::bootstrap::llvmLoweringPass(module);

    std::shared_ptr<Global> globalVar = Global::make(
        std::make_shared<IntegerType>(IntegerKind::Int32),
        test::constant::foo,

        IntegerLiteral::make(
            std::make_shared<IntegerType>(IntegerKind::Int32),
            123
        )->staticCast<Value<>>()
    );

    globalVar->parent = module;
    llvmLoweringPass->visitGlobal(globalVar);
    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "global_init"));
}

TEST(CodeGenTest, VisitAllocaInst) {
    std::shared_ptr<Module> module = std::make_shared<Module>(
        std::make_shared<Identifier>(test::constant::bar)
    );

    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
        test::bootstrap::llvmLoweringPass(module);

    std::vector<std::shared_ptr<Instruction>> insts{
        AllocaInst::make(std::make_shared<IntegerType>(IntegerKind::Int32))
    };

    std::shared_ptr<Function> function = test::bootstrap::emptyFunction(insts);

    function->parent = module;
    llvmLoweringPass->visitFunction(function);
    EXPECT_TRUE(test::comparison::ir(llvmLoweringPass, "inst_alloca"));
}

TEST(CodeGenTest, VisitReturnInst) {
    std::shared_ptr<Module> module = std::make_shared<Module>(
        std::make_shared<Identifier>(test::constant::bar)
    );

    std::shared_ptr<LlvmLoweringPass> llvmLoweringPass =
        test::bootstrap::llvmLoweringPass(module);

    std::vector<std::shared_ptr<Instruction>> insts{
        ReturnInst::make(
            IntegerLiteral::make(
                std::make_shared<IntegerType>(IntegerKind::Int32),
                1
            )->flatten()
        )
    };

    std::shared_ptr<Function> function = test::bootstrap::emptyFunction(insts);

    function->parent = module;
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
//    std::shared_ptr<AllocaInst> allocaInst = AllocaInst::make(
//        /**
//         * The alloca instruction needs it's parent to be set in order
//         * to be resolved.
//         */
//        *functionEntryBlock,
//
//        returnValueType
//    );
//
//    PtrRef<AllocaInst> allocaInstRef1 = std::make_shared<Ref<AllocaInst>>(
//        RefKind::Inst,
//        test::constant::foo,
//        nullptr,
//        allocaInst
//    );
//
//    std::shared_ptr<StoreInst> storeInst = StoreInst::make(
//        // No need for parent to be set.
//        nullptr,
//
//        std::make_shared<IntegerLiteral>(returnValueType, 1)->staticCast<Value<>>(),
//        allocaInstRef1
//    );
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
//    std::shared_ptr<ReturnInst> returnInst = ReturnInst::make(ReturnInstOpts{
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
//    std::shared_ptr<BasicBlock> body = BasicBlock::make(
//        nullptr,
//        BasicBlockKind::Label,
//        "if_body",
//        {}
//    );
//
//    std::shared_ptr<BooleanLiteral> condition = std::make_shared<BooleanLiteral>(true);
//    PtrRef<BasicBlock> bodyBasicBlockRef = std::make_shared<Ref<BasicBlock>>(body);
//
//    // TODO: Use some sort of factory design pattern.
//    auto branchInst = BranchInst::make(
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
//    );
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
