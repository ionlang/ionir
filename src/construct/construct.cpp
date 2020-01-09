#include <ionir/construct/construct.h>

namespace ionir {
    Construct::Construct(ConstructKind kind) : constructKind(kind) {
        //
    }

    Ast Construct::getChildrenNodes() const {
        // By default, construct contains no children.
        return {};
    }

    bool Construct::isLeafNode() const {
        return this->getChildrenNodes().empty();
    }

    ConstructKind Construct::getConstructKind() const {
        return this->constructKind;
    }

    bool Construct::verify() const {
        return true;
    }

    Ptr<Construct> Construct::getPtr() {
        return this->shared_from_this();
    }

    Ptr<Construct> Construct::nativeCast() {
        return this->cast<Construct>();
    }
}
