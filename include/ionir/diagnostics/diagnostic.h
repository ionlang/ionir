#pragma once

#include <ionshared/diagnostics/diagnostic_builder.h>

#define IONIR_NOTICE_DEFINE(name, kind, message, example) \
    const static inline ionshared::Diagnostic name =\
        ionshared::Diagnostic{\
            kind,\
            message,\
            std::nullopt,\
            diagnostic::errorCounter++,\
            example\
        }

namespace ionir::diagnostic {
    static uint32_t errorCounter = 0;

    IONIR_NOTICE_DEFINE(
        externRedefinition,
        ionshared::DiagnosticKind::Error,
        "Re-definition of extern not allowed",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        entryPointRedefinition,
        ionshared::DiagnosticKind::Error,
        "Entry point function had already previously been declared",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        entryPointMissing,
        ionshared::DiagnosticKind::Error,
        "No suitable entry point could be found",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        entryPointInvalidSignature,
        ionshared::DiagnosticKind::Error,
        "Entry point function has an invalid signature",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        functionMissingEntryBasicBlock,
        ionshared::DiagnosticKind::Error,
        "Function '%s' is missing an entry basic block",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        functionReturnValueMissing,
        ionshared::DiagnosticKind::Error,
        "Function '%s' must return a value",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        functionReturnValueTypeMismatch,
        ionshared::DiagnosticKind::Error,
        "Function '%s' return type '%s' and return value type '%s' mismatch",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        functionRedefinitionDiffArgs,
        ionshared::DiagnosticKind::Error,
        "Re-definition of function with a different amount arguments",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        structWrongValueCount,
        ionshared::DiagnosticKind::Error,
        "Defining struct '%s' with %s values, but the declaration has %s fields",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        structIncompatibleValueType,
        ionshared::DiagnosticKind::Error,
        "Defining struct '%s' with incompatible value type '%s' at index %s with declaration type '%s'",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        instStoreTypeMismatch,
        ionshared::DiagnosticKind::Error,
        "Cannot store value type '%s' into target with type '%s'",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        instCallWrongArgCount,
        ionshared::DiagnosticKind::Error,
        "Calling function '%' with the wrong amount of arguments: expected %s, but got %s",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        instCallIncompatibleArg,
        ionshared::DiagnosticKind::Error,
        "Argument type mismatch when calling function '%s': expected '%s', but got '%s'",
        std::nullopt
    );

    IONIR_NOTICE_DEFINE(
        constructFailedVerification,
        ionshared::DiagnosticKind::InternalError,
        "Construct failed verification",
        std::nullopt
    );
}

// Misc.
#define IONIR_NOTICE_MISC_UNEXPECTED_TOKEN "Encountered an unexpected token"
#define IONIR_NOTICE_MISC_UNEXPECTED_EOF "Unexpected end of input"
#define IONIR_NOTICE_MISC_ASSERTION_FAILED "Assertion failed"

// Function.
#define IONIR_NOTICE_FUNCTION_CALL_UNDEFINED "Call to undefined function '%s'"
#define IONIR_NOTICE_FUNCTION_CALL_LEADING_COMMA "Leading comma in argument list is disallowed"
#define IONIR_NOTICE_FUNCTION_RETURN_TYPE_MISMATCH "Function '%s' return type and return value type mismatch"
#define IONIR_NOTICE_FUNCTION_REDEFINED "Cannot redefine existing function '%s'"

// Instruction.
#define IONIR_NOTICE_INST_ALLOCA_VOID "Cannot allocate void type"
#define IONIR_NOTICE_INST_STORE_UNRESOLVED_REF "Cannot store value to an unresolved reference '%s'"
#define IONIR_NOTICE_INST_STORE_TYPE_MISMATCH "Cannot store value with different type than what was allocated to '%s'"

// Value.
#define IONIR_NOTICE_VALUE_CHARACTER_MAX_ONE "Character value length must be at most a single character"
#define IONIR_NOTICE_VALUE_CONVERT_STRING_TO_INT_FAILED "Could not convert string to value, integer may be invalid or too large"
