#pragma once

namespace ionir
{
enum class InstKind
{
    Alloca,

    Store,

    Call,

    Return,

    Break,

    Branch
};
} // namespace ionir