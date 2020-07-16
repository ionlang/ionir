#include <sstream>
#include <ionir/misc/util.h>
#include <ionir/construct/inst.h>
#include <ionir/construct/function.h>
#include <ionir/construct/extern.h>
#include <ionir/construct/global.h>
#include <ionir/construct/inst/alloca.h>

namespace ionir {
    bool Util::stringStartsWith(std::string subject, std::string value) {
        return subject.rfind(value, 0) == 0;
    }

    std::string Util::escapeRegex(std::string subject) {
        std::string result;

        for (char &character : subject) {
            // Determine if the character is denoted special.
            if (Util::specialChars.find(character) != Util::specialChars.npos) {
                // If so, escape the character and append it to the result.
                result += "\\" + std::string(1, character);

                continue;
            }

            // Otherwise, append it to the result directly.
            result += character;
        }

        return result;
    }

    std::regex Util::createPureRegex(std::string value) {
        return std::regex(Util::escapeRegex(value));
    }

    bool Util::withinRange(long value, long from, long to) {
        return value >= from && value <= to;
    }

    std::string Util::joinStringVector(std::vector<std::string> vector) {
        if (vector.empty()) {
            return "";
        }

        std::stringstream result;

        for (auto iterator = std::begin(vector); iterator != std::end(vector); ++iterator) {
            result << *iterator;

            if (std::next(iterator) != vector.end()) {
                result << " ";
            }
        }

        return result.str();
    }

    std::string Util::resolveIntegerKindName(IntegerKind kind) {
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

    TypeKind Util::resolveTypeKind(std::string id) {
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

        return TypeKind::UserDefined;
    }

    std::optional<std::string> Util::getConstructId(ionshared::Ptr<Construct> construct) {
        switch (construct->getConstructKind()) {
            case ConstructKind::Function: {
                return construct->dynamicCast<Function>()->getPrototype()->getId();
            }

            case ConstructKind::Extern: {
                return construct->dynamicCast<Extern>()->getPrototype()->getId();
            }

            case ConstructKind::Global: {
                return construct->dynamicCast<Global>()->getId();
            }

            case ConstructKind::Inst: {
                return Util::getInstId(construct->dynamicCast<Inst>());
            }

            default: {
                return std::nullopt;
            }
        }
    }

    std::optional<std::string> Util::getInstId(ionshared::Ptr<Inst> inst) noexcept {
        switch (inst->getInstKind()) {
            default: {
                return std::nullopt;
            }
        }
    }

    uint32_t Util::calculateBitLength(int64_t number) {
        if (number < 0) {
            throw std::runtime_error("Number cannot be less than zero");
        }
        else if (number == 0) {
            return 1;
        }

        /**
         * Formula has been taken from Wikipedia.
         */
        return floor(log2(number + 1));
    }

    std::optional<IntegerKind> Util::calculateIntegerKindFromBitLength(uint32_t bitLength) noexcept {
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
