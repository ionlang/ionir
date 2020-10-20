#include <queue>
#include <ionir/construct/construct.h>
#include <ionir/construct/value.h>
#include <ionir/const/const.h>

namespace ionir {
    bool Construct::verify(const std::shared_ptr<Construct>& construct) noexcept {
        return construct != nullptr
            && construct->verify();
    }

    Construct::Construct(
        ConstructKind kind,
        std::optional<ionshared::SourceLocation> sourceLocation,
        ionshared::OptPtr<Construct> parent
    ) noexcept :
        ionshared::BaseConstruct<Construct, ConstructKind>(
            kind,
            sourceLocation,
            std::move(parent)
        ) {
        //
    }

    bool Construct::equals(const std::shared_ptr<Construct>& other) {
        return other == this->shared_from_this();
    }

    bool Construct::verify() {
        Ast children = this->getChildrenNodes();

        /**
         * Loop through the children and verify them. If the verification
         * function returns false, this construct's verification fails.
         * If all the children's verification functions return true,
         * this construct's verification passes, and returns true.
         */
        for (auto& child : children) {
            // NOTE: The verification function is not constant.
            if (!Construct::verify(child)) {
                return false;
            }
        }

        return true;
    }

    std::optional<std::string> Construct::findConstructKindName() {
        return Const::getConstructKindName(this->constructKind);
    }

    std::shared_ptr<Construct> Construct::fetchRootConstruct() {
        if (!ionshared::util::hasValue(this->parent)) {
            this->shared_from_this();
        }

        std::queue<std::shared_ptr<Construct>> parentQueue{};

        parentQueue.push(*this->parent);

        while (!parentQueue.empty()) {
            std::shared_ptr<Construct> parent = parentQueue.front();

            parentQueue.pop();

            if (!ionshared::util::hasValue(parent->parent)) {
                return parent;
            }

            parentQueue.push(*parent->parent);
        }

        // NOTE: Code execution shouldn't reach this point.
        throw std::runtime_error("Precaution point reached");
    }
}
