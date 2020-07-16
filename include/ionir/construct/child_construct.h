#pragma once

#include <ionir/misc/helpers.h>
#include "construct.h"

namespace ionir {
    template<typename T>
    struct ChildConstructOpts {
        ionshared::Ptr<T> parent;
    };

    template<class T = Construct>
    class ChildConstruct : public Construct {
    private:
        ionshared::Ptr<T> parent;

    public:
        ChildConstruct(ionshared::Ptr<T> parent, ConstructKind kind) : Construct(kind), parent(parent) {
            //
        }

        ionshared::Ptr<T> getParent() const {
            return this->parent;
        }

        void setParent(ionshared::Ptr<T> parent) {
            this->parent = parent;
        }
    };
}
