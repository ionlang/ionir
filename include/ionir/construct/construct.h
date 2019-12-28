#pragma once

#include <ionir/misc/helpers.h>

namespace ionir {
    class Pass;

    enum class ConstructKind {
        Type,

        /**
         * A block, possibly containing statements.
         */
            Block,

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

        Instruction,

        Global,

        Section,

        Reference,

        Expr
    };

    class Construct : public std::enable_shared_from_this<Construct> {
    protected:
        ConstructKind constructKind;

    public:
        explicit Construct(ConstructKind kind);

        virtual void accept(Pass *visitor) = 0;

        ConstructKind getConstructKind() const;

        /**
         * Verify the members and properties of the node.
         * Without an implementation by the derived class,
         * this will always return true.
         */
        virtual bool verify() const;

        Ptr<Construct> getPtr();

        template<class T>
        Ptr<T> cast() {
            return std::static_pointer_cast<T>(this->shared_from_this());
        }
    };
}