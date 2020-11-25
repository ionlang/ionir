#include <ionir/construct/basic_block.h>
#include <ionir/passes/pass.h>

namespace ionir {
    Instruction::Instruction(InstructionKind kind) noexcept :
        Construct(ConstructKind::Inst),
        instKind(kind) {
        //
    }

    bool Instruction::isTerminal() const noexcept {
        return this->instKind == InstructionKind::Return
            || this->instKind == InstructionKind::Branch;
    }
}
