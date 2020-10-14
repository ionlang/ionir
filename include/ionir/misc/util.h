#pragma once

#include <optional>
#include <vector>
#include <algorithm>
#include <string>
#include <regex>
#include <string_view>
#include <cmath>
#include <ionir/construct/type/integer_type.h>
#include <ionir/syntax/ast_result.h>
#include <ionir/const/const_name.h>

namespace ionir::util {
    struct Inst;

    struct IntegerType;

    template<typename T>
    struct ParentMixin {
        T parent;
    };

    template<typename T = Construct>
//        requires std::derived_from<T, Construct> // TODO: Cannot work in the current system because ConstructParentMixin<T> is used where T is a forward decl.
    using ConstructParentMixin = ParentMixin<ionshared::Ptr<T>>;

    [[nodiscard]] std::string resolveIntegerKindName(IntegerKind kind);

    [[nodiscard]] TypeKind resolveTypeKind(const std::string& id);

    [[nodiscard]] std::optional<std::string> findConstructId(const ionshared::Ptr<Construct>& construct);

    [[nodiscard]] std::optional<std::string> findInstId(const ionshared::Ptr<ionir::Inst>& inst) noexcept;

    [[nodiscard]] std::optional<IntegerKind> findIntegerKindFromBitLength(uint32_t bitLength) noexcept;
}
