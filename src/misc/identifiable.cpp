#include <ionir/construct/identifier.h>

namespace ionir {
    Identifiable::Identifiable(std::shared_ptr<Identifier> identifier) noexcept :
        identifier(std::move(identifier)) {
        //
    }
}
