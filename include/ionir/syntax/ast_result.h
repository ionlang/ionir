#pragma once

#include <variant>
#include <ionshared/misc/helpers.h>
#include <ionir/construct/pseudo/error_marker.h>

namespace ionir {
    template<typename T = Construct>
    using AstResult = std::variant<T, std::shared_ptr<ErrorMarker>>;

    template<typename T = Construct>
    using AstPtrResult = AstResult<std::shared_ptr<T>>;
}
