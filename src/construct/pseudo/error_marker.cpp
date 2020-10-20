#include <utility>
#include <ionir/passes/pass.h>

namespace ionir {
    ErrorMarker::ErrorMarker(std::string message) noexcept :
        Construct(ConstructKind::ErrorMarker),
        message(std::move(message)) {
        //
    }

    void ErrorMarker::accept(Pass& visitor) {
        visitor.visitErrorMarker(this->dynamicCast<ErrorMarker>());
    }
}
