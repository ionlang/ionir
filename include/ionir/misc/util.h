#pragma once

#include <optional>
#include <vector>
#include <algorithm>
#include <string>
#include <regex>
#include <string_view>
#include <cmath>
#include <ionir/construct/type/integer_type.h>
#include <ionir/construct/type/decimal_type.h>
#include <ionir/syntax/ast_result.h>
#include <ionir/const/const_name.h>

namespace ionir::util {
    struct IntegerType;

    template<typename T>
    struct ParentMixin {
        T parent;
    };

    [[nodiscard]] std::string resolveIntegerKindName(IntegerKind kind);

    [[nodiscard]] std::string resolveDecimalKindName(DecimalKind kind);

    [[nodiscard]] TypeKind resolveTypeKind(const std::string& id);

    [[nodiscard]] std::optional<std::string> findConstructId(const std::shared_ptr<Construct>& construct);

    [[nodiscard]] std::optional<std::string> findInstId(const std::shared_ptr<ionir::Instruction>& inst) noexcept;

    [[nodiscard]] std::optional<IntegerKind> findIntegerKindFromBitLength(uint32_t bitLength) noexcept;
}
