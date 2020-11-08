#include <ionir/test/bootstrap.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

TEST(ConstructBasicBlockTest, HasTerminalInstruction) {
    std::shared_ptr<BasicBlock> basicBlock = BasicBlock::make(
        std::vector<std::shared_ptr<Instruction>>{ReturnInst::make()}
    );

    EXPECT_TRUE(basicBlock->hasTerminalInst());
}

TEST(ConstructBasicBlockTest, FindTerminalInstruction) {
    std::shared_ptr<BasicBlock> basicBlock = BasicBlock::make(
        std::vector<std::shared_ptr<Instruction>>{ReturnInst::make()}
    );

    EXPECT_TRUE(ionshared::util::hasValue(basicBlock->findTerminalInst()));
}

TEST(ConstructBasicBlockTest, Link) {
    std::shared_ptr<BasicBlock> basicBlockA =
        BasicBlock::make();

    std::shared_ptr<BasicBlock> basicBlockB =
        BasicBlock::make();

    basicBlockA->link(basicBlockB);

    ASSERT_FALSE(basicBlockA->instructions.empty());
    EXPECT_EQ(basicBlockA->instructions.front()->instKind, InstructionKind::Jump);
}

TEST(ConstructBasicBlockTest, FindFirstInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        BasicBlock::make();

    std::shared_ptr<ReturnInst> returnInstA =
        ReturnInst::make();

    std::shared_ptr<ReturnInst> returnInstB =
        ReturnInst::make();

    basicBlock->instructions.push_back(returnInstA);

    EXPECT_EQ(basicBlock->findFirstInst(), returnInstA);

    basicBlock->instructions.push_back(returnInstB);

    EXPECT_EQ(basicBlock->findFirstInst(), returnInstA);
}

TEST(ConstructBasicBlockTest, FindLastInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        BasicBlock::make();

    std::shared_ptr<ReturnInst> returnInstA =
        ReturnInst::make();

    std::shared_ptr<ReturnInst> returnInstB =
        ReturnInst::make();

    basicBlock->instructions.push_back(returnInstA);

    EXPECT_EQ(basicBlock->findLastInst(), returnInstA);

    basicBlock->instructions.push_back(returnInstB);

    EXPECT_EQ(basicBlock->findLastInst(), returnInstB);
}
