#include <ionir/misc/inst_builder.h>
#include <ionir/misc/bootstrap.h>
#include "pch.h"

using namespace ionir;

TEST(InstBuilderTest, GetSection) {
    ionshared::Ptr<BasicBlock> section = Bootstrap::basicBlock();
    ionshared::Ptr<InstBuilder> builder = section->createBuilder();

    EXPECT_EQ(builder->basicBlock, section);
}

TEST(InstBuilderTest, CreateReturn) {
    // TODO
}
