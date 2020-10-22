#include <optional>
#include <ionir/construct/instruction/return.h>
#include <ionir/test/const.h>
#include "pch.h"

using namespace ionir;

// TODO: Ref<>s do not default to Construct anymore.
//TEST(RefTest, Initialize) {
//    // TODO: Just passing RefKind::Inst as a placeholder, doesn't really matter.
//    Ref<> ref = Ref<>(RefKind::Inst, test::constant::foobar, nullptr);
//
//    EXPECT_EQ(ref.getId(), test::constant::foobar);
//    EXPECT_EQ(ref.getOwner(), nullptr);
//    EXPECT_FALSE(ref.getValue().has_value());
//}
//
//TEST(RefTest, SetId) {
//    // TODO: Just passing RefKind::Inst as a placeholder, doesn't really matter.
//    Ref<> ref = Ref<>(RefKind::Inst, test::constant::foo, nullptr);
//
//    ref.setId(test::constant::bar);
//
//    EXPECT_EQ(ref.getId(), test::constant::bar);
//}
//
//TEST(RefTest, SetOwner) {
//    // TODO: Just passing RefKind::Inst as a placeholder, doesn't really matter.
//    Ref<> ref = Ref<>(RefKind::Inst, test::constant::foobar, nullptr);
//    std::shared_ptr<ReturnInst> returnInst = std::make_shared<ReturnInst>(ReturnInstOpts{});
//
//    ref.setOwner(returnInst);
//
//    EXPECT_EQ(ref.getOwner(), returnInst);
//}
//
//TEST(RefTest, SetValue) {
//    // TODO: Just passing RefKind::Inst as a placeholder, doesn't really matter.
//    Ref<> ref = Ref<>(RefKind::Inst, test::constant::foobar, nullptr);
//    std::shared_ptr<ReturnInst> returnInst = std::make_shared<ReturnInst>(ReturnInstOpts{});
//
//    ref.setOwner(returnInst);
//
//    EXPECT_EQ(ref.getOwner(), returnInst);
//}
