#include <ionir/passes/pass.h>

namespace ionir {
    Identifier::Identifier(std::string name, std::vector<std::string> scopePath) :
        Construct(ConstructKind::Identifier),
        ionshared::Named{std::move(name)},
        scopePath(std::move(scopePath)) {
        //
    }

    Identifier::operator std::string() const {
        std::stringstream stream;

        for (const auto& scopePath : this->scopePath) {
            // TODO: Avoid having it hard-coded.
            stream << scopePath << "::";
        }

        stream << this->name;

        return stream.str();
    }

    std::string Identifier::operator*() const {
        return this->operator std::string();
    }

    void Identifier::accept(Pass& visitor) {
        visitor.visitIdentifier(this->dynamicCast<Identifier>());
    }
}
