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

        NameOfIntrinsic,

        TypeOfIntrinsic
    };

    struct Construct;

    typedef ionshared::Ast<Construct> Ast;

    struct Construct : ionshared::BaseConstruct<Construct, ConstructKind> {
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
         * Create a shared pointer to a construct, while setting its
         * parent. Should be preferred instead of creating the shared
         * pointer and then setting its parent manually for consistency
         * and error-preventing measures.
         */
        template<typename TConstruct, typename ...TArgs>
            requires std::derived_from<TConstruct, Construct>
        static std::shared_ptr<TConstruct> makeChild(
            const std::shared_ptr<Construct>& parent,
            TArgs&&... args
        ) {
            std::shared_ptr<TConstruct> construct =
                std::make_shared<TConstruct>(std::forward<TArgs>(args)...);

            construct->parent = parent;

            // TODO: Consider having a virtual default-empty 'acceptChild(<ConstructTypeHere> child)' which inserts it into a corresponding list (if any)

            return construct;
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
            std::optional<ionshared::SourceLocation> sourceLocation = std::nullopt,
            ionshared::OptPtr<Construct> parent = std::nullopt
        ) noexcept;

        virtual void accept(Pass& visitor) = 0;

        [[nodiscard]] virtual bool equals(const std::shared_ptr<Construct>& other);

        // TODO: Move to BaseConstruct<> in ionshared.
        /**
         * Verify the members and properties of the node, and it's children to
         * ensure that this construct is well-formed. Without an implementation
         * by the derived class (or without being called by it), this will return
         * true if all the child nodes are successfully verified. If there are no
         * child nodes, the result will be true by default. Verification should
         * not be delegated to the parent (if any), to avoid circular, endless
         * calls.
         */
        [[nodiscard]] virtual bool verify();

        [[nodiscard]] std::optional<std::string> findConstructKindName();

        [[nodiscard]] std::shared_ptr<Construct> fetchRootConstruct();
    };

    template<typename T>
    using ConstructBuilder = ionshared::ConstructBuilder<Construct, ConstructKind, T>;
}
