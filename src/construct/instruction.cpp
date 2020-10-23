#include <ionir/construct/basic_block.h>
#include <ionir/passes/pass.h>

namespace ionir {
    Instruction::Instruction(InstKind kind) noexcept :
        ConstructWithParent<BasicBlock>(ConstructKind::Inst),
        instKind(kind) {
        //
    }

    bool Instruction::isTerminal() const noexcept {
        return this->instKind == InstKind::Return
            || this->instKind == InstKind::Branch;
    }

    size_t Instruction::fetchOrder() {
        std::optional<uint32_t> order =
            this->forceGetUnboxedParent()->locate(this->dynamicCast<Instruction>());

        if (!order.has_value()) {
            // TODO: Should be internal error diagnostic.
            throw std::runtime_error("Could not locate instruction in parent");
        }

        return *order;
    }
}
