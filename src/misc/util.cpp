#include <sstream>
#include <ionir/misc/util.h>
#include <ionir/construct/instruction.h>
#include <ionir/construct/function.h>
#include <ionir/construct/extern.h>
#include <ionir/construct/global.h>

namespace ionir::util {
    std::string resolveIntegerKindName(IntegerKind kind) {
        switch (kind) {
            case IntegerKind ::Int8: {
                return ConstName::typeInt8;
            }

            case IntegerKind::Int16: {
                return ConstName::typeInt16;
            }

            case IntegerKind::Int32: {
                return ConstName::typeInt32;
            }

            case IntegerKind::Int64: {
                return ConstName::typeInt64;
            }

            default: {
                throw std::runtime_error("Unknown integer kind");
            }
        }
    }

    std::string resolveDecimalKindName(DecimalKind kind) {
        switch (kind) {
            case DecimalKind::BitSize8: {
                return ConstName::typeInt8;
            }

            case DecimalKind::BitSize16: {
                return ConstName::typeInt16;
            }

            case DecimalKind::BitSize32: {
                return ConstName::typeInt32;
            }

            case DecimalKind::BitSize64: {
                return ConstName::typeInt64;
            }

            default: {
                throw std::runtime_error("Unknown integer kind");
            }
        }
    }

    TypeKind resolveTypeKind(const std::string& id) {
        // TODO: CRITICAL: Add support new/missing types.

        if (id == ConstName::typeInt8) {
            return TypeKind::Integer;
        }
        else if (id == ConstName::typeInt16) {
            return TypeKind::Integer;
        }
        else if (id == ConstName::typeInt32) {
            return TypeKind::Integer;
        }
        else if (id == ConstName::typeInt64) {
            return TypeKind::Integer;
        }
        else if (id == ConstName::typeVoid) {
            return TypeKind::Void;
        }
        else if (id == ConstName::typeString) {
            return TypeKind::String;
        }

        return TypeKind::Struct;
    }

    std::optional<std::string> findConstructId(const std::shared_ptr<Construct>& construct) {
        switch (construct->constructKind) {
            case ConstructKind::Function: {
                return construct->dynamicCast<Function>()->prototype->name;
            }

            case ConstructKind::Extern: {
                return construct->dynamicCast<Extern>()->prototype->name;
            }

            case ConstructKind::Global: {
                return construct->dynamicCast<Global>()->name;
            }

            case ConstructKind::Inst: {
                return util::findInstId(construct->dynamicCast<ionir::Instruction>());
            }

            default: {
                return std::nullopt;
            }
        }
    }

    std::optional<std::string> findInstId(const std::shared_ptr<ionir::Instruction>& inst) noexcept {
        switch (inst->instKind) {
            // TODO: Implement.

            default: {
                return std::nullopt;
            }
        }
    }

    std::optional<IntegerKind> findIntegerKindFromBitLength(uint32_t bitLength) noexcept {
        if (bitLength <= 8) {
            return IntegerKind::Int8;
        }
        else if (bitLength <= 16) {
            return IntegerKind::Int16;
        }
        else if (bitLength <= 32) {
            return IntegerKind::Int32;
        }
        else if (bitLength <= 64) {
            return IntegerKind::Int64;
        }
        else if (bitLength <= 128) {
            return IntegerKind::Int128;
        }

        return std::nullopt;
    }
}
