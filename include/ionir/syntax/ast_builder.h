#pragma once

#include <string>
#include <ionir/construct/function.h>
#include <ionir/construct/module.h>
#include "ionir/misc/inst_builder.h"

namespace ionir {
    class AstBuilder {
    private:
        Ast ast;

        ionshared::OptPtr<Module> moduleBuffer;

        ionshared::OptPtr<Function> functionBuffer;

        ionshared::OptPtr<BasicBlock> basicBlockBuffer;

        ionshared::OptPtr<InstBuilder> instBuilder;

        void setBasicBlockBuffer(const ionshared::OptPtr<BasicBlock>& basicBlockBuffer);

        void require(const ionshared::OptPtr<Construct>& construct) const;

        void requireModule() const;

        void requireFunction() const;

        void requireBasicBlock() const;

        void clearBuffers();

    public:
        AstBuilder();

        [[nodiscard]] Ast make() const noexcept;

        AstBuilder& module(const std::string& id);

        AstBuilder& function(const std::string& id);

        AstBuilder& functionReturnType(std::shared_ptr<Type> returnType);

        AstBuilder& instAlloca(const std::string& id, std::shared_ptr<Type> type);
    };
}
