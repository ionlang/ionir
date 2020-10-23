#include <ionir/test/bootstrap.h>
#include <ionir/test/const.h>
#include "../pch.h"

using namespace ionir;

TEST(ConstructBasicBlockTest, HasTerminalInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        std::make_shared<BasicBlock>(nullptr);

    basicBlock->instructions.push_back(std::make_shared<ReturnInst>(basicBlock));

    EXPECT_TRUE(basicBlock->hasTerminalInst());
}

TEST(ConstructBasicBlockTest, FindTerminalInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        std::make_shared<BasicBlock>(nullptr);

    basicBlock->instructions.push_back(std::make_shared<ReturnInst>(basicBlock));

    EXPECT_TRUE(ionshared::util::hasValue(basicBlock->findTerminalInst()));
}

TEST(ConstructBasicBlockTest, Link) {
    std::shared_ptr<BasicBlock> basicBlockA =
        std::make_shared<BasicBlock>(nullptr);

    std::shared_ptr<BasicBlock> basicBlockB =
        std::make_shared<BasicBlock>(nullptr);

    basicBlockA->link(basicBlockB);

    ASSERT_FALSE(basicBlockA->instructions.empty());
    EXPECT_EQ(basicBlockA->instructions[0]->instKind, InstKind::Jump);
}

TEST(ConstructBasicBlockTest, FindFirstInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        std::make_shared<BasicBlock>(nullptr);

    std::shared_ptr<ReturnInst> returnInstA =
        std::make_shared<ReturnInst>(basicBlock);

    std::shared_ptr<ReturnInst> returnInstB =
        std::make_shared<ReturnInst>(basicBlock);

    basicBlock->instructions.push_back(returnInstA);

    EXPECT_EQ(basicBlock->findFirstInst(), returnInstA);

    basicBlock->instructions.push_back(returnInstB);

    EXPECT_EQ(basicBlock->findFirstInst(), returnInstA);
}

TEST(ConstructBasicBlockTest, FindLastInstruction) {
    std::shared_ptr<BasicBlock> basicBlock =
        std::make_shared<BasicBlock>(nullptr);

    std::shared_ptr<ReturnInst> returnInstA =
        std::make_shared<ReturnInst>(basicBlock);

    std::shared_ptr<ReturnInst> returnInstB =
        std::make_shared<ReturnInst>(basicBlock);

    basicBlock->instructions.push_back(returnInstA);

    EXPECT_EQ(basicBlock->findLastInst(), returnInstA);

    basicBlock->instructions.push_back(returnInstB);

    EXPECT_EQ(basicBlock->findLastInst(), returnInstB);
}
