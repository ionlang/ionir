#include <ionir/construct/construct.h>
#include <ionir/construct/value.h>
#include <ionir/construct/inst.h>
#include <ionir/const/const.h>

namespace ionir {
    Construct::operator ionshared::Ptr<Value<>>() {
        // TODO: Test.
        return this->dynamicCast<Value<>>();
    }

    Construct::operator ionshared::Ptr<Inst>() {
        // TODO: Test.
        return this->dynamicCast<Inst>();
    }

    Construct::Construct(ConstructKind kind) : ionshared::BaseConstruct<Construct, ConstructKind>(kind) {
        //
    }

    void Construct::accept(ionshared::BasePass<Construct> visitor) {
        // TODO
    }

    Ast Construct::getChildNodes() {
        // By default, construct contains no children.
        return {};
    }

    bool Construct::equals(const ionshared::Ptr<Construct> &other) {
        return other == this->shared_from_this();
    }

    bool Construct::verify() {
        return true;
    }

    std::optional<std::string> Construct::getConstructName() {
        return Const::getConstructKindName(this->getConstructKind());
    }
}
