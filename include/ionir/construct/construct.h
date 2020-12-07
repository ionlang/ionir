#pragma once

#include <optional>
#include <string>
#include <vector>
#include <functional>
#include <ionshared/construct/base_construct.h>
#include <ionshared/construct/construct_builder.h>
#include <ionir/tracking/symbol_table.h>

namespace ionir {
    struct Pass;

    struct Instruction;

    class Module;

    struct Type;

    template<typename T>
    struct Value;

    enum struct ConstructKind {
        Type,

        /**
         * A function body, possibly containing statements.
         */
        FunctionBody,

        /**
         * Function prototype used to declare externs
         * or actual functions.
         */
        Prototype,

        Function,

        /**
         * An external function definition.
         */
        Extern,

        Inst,

        Global,

        BasicBlock,

        VariableRef,

        VariableDecl,

        StructDecl,

        Module,

        Ref,

        Value,

        RegisterAssign,

        ErrorMarker,

        Identifier,

        IntrinsicNameOf,

        IntrinsicTypeOf,

        Comment,

        Method,

        Constructor,

        Destructor
    };

    class Construct;

    typedef ionshared::Ast<Construct> Ast;

    /**
     * A language construct.
     */
    class Construct : public ionshared::BaseConstruct<Construct, ConstructKind> {
    private:
        std::optional<std::shared_ptr<Module>> module;

    public:
        /**
         * Map children from a vector to an ast.
         */
        template<typename T>
            requires std::derived_from<T, Construct>
        static Ast convertChildren(std::vector<std::shared_ptr<T>> vector) {
            Ast children{};

            for (const auto item : vector) {
                children.push_back(item);
            }

            return children;
        }

        template<typename T>
            requires std::derived_from<T, Construct>
        static Ast convertChildren(ionshared::PtrSymbolTable<T> symbolTable) {
            Ast children = {};
            auto symbolTableEntries = symbolTable->unwrap();

            for (const auto& [id, construct] : symbolTableEntries) {
                children.push_back(construct);
            }

            return children;
        }

        template<typename TFirst, typename TSecond>
            // TODO: Require for-loop iteration capability.
        static Ast mergeChildren(TFirst first, TSecond second) {
            Ast children = {};

            for (const auto item : first) {
                children.push_back(item);
            }

            for (const auto item : second) {
                children.push_back(item);
            }

            return children;
        }

        /**
         * Serves as a proxy to safely invoke the verification method
         * of a possibly nullptr construct.
         */
        [[nodiscard]] static bool verify(
            const std::shared_ptr<Construct>& construct
        ) noexcept;

        explicit Construct(
            ConstructKind kind,
            std::optional<ionshared::SourceLocation> sourceLocation = std::nullopt
        ) noexcept;

        virtual void accept(Pass& visitor) = 0;

        [[nodiscard]] virtual std::optional<std::shared_ptr<Module>> getModule();

        virtual void setModule(std::optional<std::shared_ptr<Module>> module);

        /**
         * Determine whether a construct is equivalent to another,
         * depending on its properties.
         */
        [[nodiscard]] virtual bool equals(const std::shared_ptr<Construct>& other);

        /**
         * Attempt to retrieve the name of the construct from a constant
         * map.
         */
        [[nodiscard]] std::optional<std::string> findConstructKindName();
    };

    template<typename T>
    using ConstructBuilder = ionshared::ConstructBuilder<Construct, ConstructKind, T>;
}
