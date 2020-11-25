#include <ionir/test/bootstrap.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

TEST(ConstructBasicBlockTest, HasTerminalInstruction) {
    std::shared_ptr<BasicBlock> basicBlock = std::make_shared<BasicBlock>(
        std::vector<std::shared_ptr<Instruction>>{std::make_shared<InstReturn>()}
    );

    EXPECT_TRUE(basicBlock->hasTerminalInst());
}

TEST(ConstructBasicBlockTest, FindTerminalInstruction) {
    std::shared_ptr<BasicBlock> basicBlock = std::make_shared<BasicBlock>(
        std::vector<std::shared_ptr<Instruction>>{std::make_shared<InstReturn>()}
    );

    EXPECT_TRUE(ionshared::util::hasValue(basicBlock->findTerminalInstruction()));
}

TEST(ConstructBasicBlockTest, LinkTo) {
    std::shared_ptr<BasicBlock> basicBlockA =
        std::make_shared<BasicBlock>();

    std::shared_ptr<BasicBlock> basicBlockB =
        std::make_shared<BasicBlock>();

    basicBlockA->linkTo(basicBlockB);
    ASSERT_FALSE(basicBlockA->getInstructions().empty());
    EXPECT_EQ(basicBlockA->getInstructions().front()->instKind, InstructionKind::Jump);
}

TEST(ConstructBasicBlockTest, FindFirstInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        std::make_shared<BasicBlock>();

    std::shared_ptr<InstReturn> returnInstA =
        std::make_shared<InstReturn>();

    std::shared_ptr<InstReturn> returnInstB =
        std::make_shared<InstReturn>();

    basicBlock->appendInstruction(returnInstA);
    EXPECT_EQ(basicBlock->findFirstInstruction(), returnInstA);
    basicBlock->appendInstruction(returnInstB);
    EXPECT_EQ(basicBlock->findFirstInstruction(), returnInstA);
}

TEST(ConstructBasicBlockTest, FindLastInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        std::make_shared<BasicBlock>();

    std::shared_ptr<InstReturn> returnInstA =
        std::make_shared<InstReturn>();

    std::shared_ptr<InstReturn> returnInstB =
        std::make_shared<InstReturn>();

    basicBlock->appendInstruction(returnInstA);
    EXPECT_EQ(basicBlock->findLastInstruction(), returnInstA);
    basicBlock->appendInstruction(returnInstB);
    EXPECT_EQ(basicBlock->findLastInstruction(), returnInstB);
}
