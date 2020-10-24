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

    void AstBuilder::require(const ionshared::OptPtr<Construct>& construct) const {
        if (!construct.has_value()) {
            throw std::runtime_error("Required construct is null");
        }
    }

    void AstBuilder::requireModule() const {
        this->require(this->moduleBuffer);
    }

    void AstBuilder::requireFunction() const {
        this->requireModule();
        this->require(this->functionBuffer);
    }

    void AstBuilder::requireBasicBlock() const {
        this->require(this->basicBlockBuffer);
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

        std::shared_ptr<Type> returnType = std::make_shared<VoidType>();

        std::shared_ptr<Prototype> prototype = Construct::makeChild<Prototype>(
            *this->moduleBuffer,
            id,
            std::make_shared<Args>(),
            returnType
        );

        returnType->parent = prototype;

        std::shared_ptr<BasicBlock> entryBasicBlock = BasicBlock::make();

        std::shared_ptr<Function> function =
            Construct::makeChild<Function>(
                *this->moduleBuffer,
                prototype,
                std::vector<std::shared_ptr<BasicBlock>>{entryBasicBlock}
            );

        entryBasicBlock->parent = function;
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
        std::shared_ptr<Type> type
    ) {
        this->requireBasicBlock();
        this->instBuilder->get()->createAlloca(id, std::move(type));

        return *this;
    }
}
