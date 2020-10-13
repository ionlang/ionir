#pragma once

#include <ionir/tracking/local_var_descriptor.h>
#include <ionir/tracking/symbol_table.h>
#include "construct.h"
#include "prototype.h"
#include "function_body.h"

namespace ionir {
    struct Pass;

    struct Function : Construct {
        ionshared::Ptr<Prototype> prototype;

        ionshared::Ptr<FunctionBody> body;

        Function(
            ionshared::Ptr<Prototype> prototype,
            ionshared::Ptr<FunctionBody> body
        );

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildrenNodes() override;

        [[nodiscard]] bool verify() override;
    };
}
