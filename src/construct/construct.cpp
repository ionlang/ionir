#include <queue>
#include <utility>
#include <ionir/construct/construct.h>
#include <ionir/construct/value.h>
#include <ionir/const/const.h>

namespace ionir {
    bool Construct::verify(
        const std::shared_ptr<Construct>& construct
    ) noexcept {
        if (construct == nullptr) {
            return false;
        }

        // TODO: Isn't Construct::verify() supposed to be used here?

        bool verified = construct->constructKind != ConstructKind::Module
            ? ionshared::util::hasValue(construct->getModule())
            : true;

        // TODO: Simplify.
        if (verified) {
            Ast children = construct->getChildren();

            /**
             * Loop through the children and verify them. If the verification
             * function returns false, this construct's verification fails.
             * If all the children's verification functions return true,
             * this construct's verification passes, and returns true.
             */
            for (auto& child : children) {
                // NOTE: The verification function is not constant.
                if (!Construct::verify(child)) {
                    verified = false;

                    break;
                }
            }
        }

        return verified;
    }

    Construct::Construct(
        ConstructKind kind,
        std::optional<ionshared::SourceLocation> sourceLocation
    ) noexcept :
        ionshared::BaseConstruct<Construct, ConstructKind>(
            kind,
            sourceLocation
        ) {
        //
    }

    std::optional<std::shared_ptr<Module>> Construct::getModule() {
        return this->module;
    }

    void Construct::setModule(std::optional<std::shared_ptr<Module>> module) {
        this->module = std::move(module);
    }

    bool Construct::equals(const std::shared_ptr<Construct>& other) {
        return other == this->shared_from_this();
    }

    std::optional<std::string> Construct::findConstructKindName() {
        return Const::getConstructKindName(this->constructKind);
    }
}
