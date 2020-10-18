#include <ionir/misc/inst_builder.h>
#include <ionir/misc/bootstrap.h>
#include "pch.h"

using namespace ionir;

TEST(InstBuilderTest, GetSection) {
    std::shared_ptr<BasicBlock> section = Bootstrap::basicBlock();
    std::shared_ptr<InstBuilder> builder = section->createBuilder();

    EXPECT_EQ(builder->basicBlock, section);
}

TEST(InstBuilderTest, CreateReturn) {
    // TODO
}
