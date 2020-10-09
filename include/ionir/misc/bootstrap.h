#pragma once

#include <ionir/construct/function.h>
#include <ionir/const/const_name.h>

namespace ionir {
    struct Bootstrap {
        [[nodiscard]] static Ast functionAst(
            const std::string& id = ConstName::anonymous
        );

        [[nodiscard]] static ionshared::Ptr<Function> function(
            const std::string &id = ConstName::anonymous
        );

        [[nodiscard]] static ionshared::Ptr<BasicBlock> basicBlock(
            ionshared::Ptr<FunctionBody> parent = nullptr,
            std::string id = ConstName::anonymous,
            BasicBlockKind basicBlockKind = BasicBlockKind::Entry
        );
    };
}
