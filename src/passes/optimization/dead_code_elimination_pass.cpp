#include <ionir/passes/optimization/dead_code_elimination_pass.h>

namespace ionir {
    DeadCodeEliminationPass::DeadCodeEliminationPass(
        std::shared_ptr<ionshared::PassContext> context
    ) noexcept :
        Pass(std::move(context)) {
    }

    void DeadCodeEliminationPass::visitBasicBlock(std::shared_ptr<BasicBlock> node) {
        std::vector<std::shared_ptr<Instruction>> instructions = node->getInstructions();
        bool erase = false;

        for (auto iterator = instructions.begin(); iterator < instructions.end(); ++iterator) {
            std::shared_ptr<Instruction> inst = *iterator;

            /**
             * If applicable, erase all further instructions,
             * as they are considered dead code.
             */
            if (erase) {
                instructions.erase(iterator);

                continue;
            }
            /**
             * Determine whether the current instruction
             * is of kind return and it is not the last
             * item in the vector.
             */
            else if (inst->instKind == InstructionKind::Return && ++iterator == instructions.end()) {
                /**
                 * If so, erase all following instructions,
                 * as they are considered dead code at this
                 * point.
                 */
                erase = true;
            }
        }
    }
}
