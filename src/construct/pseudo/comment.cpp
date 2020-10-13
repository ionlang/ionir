#include <ionir/construct/pseudo/comment.h>

namespace ionir {
    Comment::Comment(bool isMultiline, std::optional<std::string> text) :
        isMultiline(isMultiline),
        text(std::move(text)) {
        //
    }
}
