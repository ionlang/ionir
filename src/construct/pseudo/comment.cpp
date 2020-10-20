#include <ionir/construct/pseudo/comment.h>

namespace ionir {
    Comment::Comment(std::optional<std::string> text) noexcept :
        text(std::move(text)) {
        //
    }
}
