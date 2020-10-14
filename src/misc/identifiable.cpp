#include <ionir/construct/identifier.h>

namespace ionir {
    Identifiable::Identifiable(ionshared::Ptr<Identifier> identifier) noexcept :
        identifier(std::move(identifier)) {
        //
    }
}
