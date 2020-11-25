#include <ionir/passes/pass.h>

namespace ionir {
    Comment::Comment(std::optional<std::string> text) noexcept :
        Construct(ConstructKind::Comment),
        text(std::move(text)) {
        //
    }

    void Comment::accept(Pass& visitor) {
        visitor.visitComment(this->dynamicCast<Comment>());
    }
}
