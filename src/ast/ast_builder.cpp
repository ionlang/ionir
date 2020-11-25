#include <ionir/construct/type/void_type.h>
#include <ionir/construct/identifier.h>
#include <ionir/syntax/ast_builder.h>
#include <ionir/const/const.h>

namespace ionir {
    void AstBuilder::setBasicBlockBuffer(const ionshared::OptPtr<BasicBlock>& basicBlockBuffer) {
        if (!basicBlockBuffer.has_value()) {
            this->basicBlockBuffer = std::nullopt;
            this->instBuilder = std::nullopt;

            return;
        }

        this->basicBlockBuffer = basicBlockBuffer;
        this->instBuilder = std::make_shared<InstBuilder>(*this->basicBlockBuffer);
    }

    std::shared_ptr<Construct> AstBuilder::require(const ionshared::OptPtr<Construct>& construct) {
        if (!construct.has_value()) {
            throw std::runtime_error("Required construct is null");
        }

        return *construct;
    }

    std::shared_ptr<Module> AstBuilder::requireModule() const {
        AstBuilder::require(this->moduleBuffer);

        return *this->moduleBuffer;
    }

    std::shared_ptr<Function> AstBuilder::requireFunction() const {
        this->requireModule();
        AstBuilder::require(this->functionBuffer);

        return *this->functionBuffer;
    }

    std::shared_ptr<BasicBlock> AstBuilder::requireBasicBlock() const {
        AstBuilder::require(this->basicBlockBuffer);

        return *this->basicBlockBuffer;
    }

    void AstBuilder::clearBuffers() {
        this->moduleBuffer = std::nullopt;
        this->functionBuffer = std::nullopt;
        setBasicBlockBuffer(std::nullopt);
    }

    AstBuilder::AstBuilder() :
        ast({}) {
        //
    }

    Ast AstBuilder::make() const noexcept {
        return this->ast;
    }

    AstBuilder& AstBuilder::module(const std::string& id) {
        std::shared_ptr<Module> module =
            std::make_shared<Module>(std::make_shared<Identifier>(id));

        this->clearBuffers();
        this->moduleBuffer = module;
        this->ast.push_back(module);

        return *this;
    }

    AstBuilder& AstBuilder::function(const std::string& id) {
        this->requireModule();

        std::shared_ptr<Type> returnType = std::make_shared<TypeVoid>();

        std::shared_ptr<Prototype> prototype = std::make_shared<Prototype>(
            id,
            std::make_shared<Args>(),
            returnType
        );

        prototype->setModule(this->requireModule());

        std::shared_ptr<BasicBlock> entryBasicBlock = std::make_shared<BasicBlock>();

        std::shared_ptr<Function> function =
            std::make_shared<Function>(
                prototype,
                std::vector<std::shared_ptr<BasicBlock>>{entryBasicBlock}
            );

        function->setModule(*this->moduleBuffer);

        // TODO: CRITICAL: Assign module.

        this->setBasicBlockBuffer(entryBasicBlock);
        this->functionBuffer = function;

        return *this;
    }

    AstBuilder& AstBuilder::functionReturnType(std::shared_ptr<Type> returnType) {
        this->requireFunction();
        this->functionBuffer->get()->prototype->returnType = std::move(returnType);

        return *this;
    }

    AstBuilder& AstBuilder::instAlloca(
        const std::string& id,
        const std::shared_ptr<Type>& type
    ) {
        this->requireBasicBlock();
        this->instBuilder->get()->makeAlloca(id, type);

        return *this;
    }
}
