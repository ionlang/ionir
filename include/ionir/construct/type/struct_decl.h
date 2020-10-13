#pragma once

namespace ionir {
    struct Pass;

    struct StructDecl : Construct {
        StructDecl();

        void accept(Pass& visitor) override;
    };
}
