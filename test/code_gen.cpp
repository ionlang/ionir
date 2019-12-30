#include <vector>
#include <ionir/llvm/codegen/llvm_visitor.h>
#include <ionir/const/const.h>
#include <ionir/const/const_name.h>
#include <ionir/llvm/module.h>
#include "test_api/bootstrap.h"
#include "test_api/compare.h"
#include "test_api/constant.h"

using namespace ionir;

TEST(CodeGenTest, VisitExtern) {
    Ptr<LlvmVisitor> visitor = test::bootstrap::llvmVisitor();
    Ptr<Type> returnType = std::make_shared<Type>(ConstName::typeVoid);
    Ptr<Args> args = std::make_shared<Args>();
    Ptr<Prototype> prototype = std::make_shared<Prototype>(test::constant::foobar, args, returnType);
    Ptr<Extern> externConstruct = std::make_shared<Extern>(prototype);

    visitor->visitExtern(externConstruct);

    Module module = Module(visitor->getModule());

    EXPECT_TRUE(test::compare::ir(module.getAsString(), "extern_simple"));
}

TEST(CodeGenTest, VisitEmptyFunction) {
    Ptr<LlvmVisitor> visitor = test::bootstrap::llvmVisitor();
    Ptr<Type> returnType = std::make_shared<Type>(ConstName::typeVoid);

    Ptr<Prototype> prototype =
        std::make_shared<Prototype>(test::constant::foobar, std::make_shared<Args>(), returnType);

    Ptr<Block> body = std::make_shared<Block>(nullptr);

    std::vector<Ptr<Section>> sections = {
        std::make_shared<Section>(SectionOpts{
            body,
            SectionKind::Entry,
            Const::sectionEntryId
        })
    };

    body->setSections(sections);

    Ptr<Function> function = std::make_shared<Function>(prototype, body);

    body->setParent(function);
    visitor->visitFunction(function);

    Module module = Module(visitor->getModule());

    EXPECT_TRUE(test::compare::ir(module.getAsString(), "function_empty"));
}

TEST(CodeGenTest, VisitEmptyGlobal) {
    Ptr<LlvmVisitor> visitor = test::bootstrap::llvmVisitor();
    Ptr<Type> type = std::make_shared<Type>(ConstName::typeInt32, false);
    Ptr<Global> globalVar = std::make_shared<Global>(type, test::constant::foobar);

    visitor->visitGlobal(globalVar);

    Module module = Module(visitor->getModule());

    EXPECT_TRUE(test::compare::ir(module.getAsString(), "global_empty"));
}

TEST(CodeGenTest, VisitGlobalWithValue) {
    Ptr<LlvmVisitor> visitor = test::bootstrap::llvmVisitor();
    Ptr<Type> type = std::make_shared<Type>(ConstName::typeInt32, false);

    Ptr<Global> globalVar = std::make_shared<Global>(type, test::constant::foobar,
        std::make_shared<IntegerValue>(IntegerKind::Int32, 123));

    visitor->visitGlobal(globalVar);

    Module module = Module(visitor->getModule());

    EXPECT_TRUE(test::compare::ir(module.getAsString(), "global_init"));
}

TEST(CodeGenTest, VisitAllocaInst) {
    Ptr<LlvmVisitor> visitor = test::bootstrap::llvmVisitor();

    std::vector<Ptr<Inst>> insts = {std::make_shared<AllocaInst>(
        AllocaInstOpts{
            nullptr,
            test::constant::foobar,
            std::make_shared<Type>(ConstName::typeInt32)
        })
    };

    Ptr<Function> function = test::bootstrap::emptyFunction(insts);

    visitor->visitFunction(function);

    Module module = Module(visitor->getModule());

    EXPECT_TRUE(test::compare::ir(module.getAsString(), "inst_alloca"));
}

// TODO
// TEST(CodeGenTest, VisitBranchInst)
// {
//     Ptr<LlvmVisitor> visitor = test::bootstrap::llvmVisitor();
//     Ptr<Section> body = std::make_shared<Section>(SectionKind::Label, "ifbody");
//     Ptr<BooleanValue> condition = std::make_shared<BooleanValue>(true);

//     std::vector<Ptr<Inst>> insts = {
//         std::make_shared<BranchInst>(condition, body),
//     };

//     Ptr<Function> function = test::bootstrap::emptyFunction(insts);

//     visitor->visitFunction(function);

//     Ptr<Module> module = std::make_shared<Module>(visitor->getModule());

//     EXPECT_TRUE(test::compare::ir(module->getAsString(), "inst_branch"));
// }
