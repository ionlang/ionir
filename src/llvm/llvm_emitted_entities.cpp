#include <ionir/llvm/llvm_emitted_entities.h>

namespace ionir {
    LlvmEmittedEntities::LlvmEmittedEntities() noexcept :
        entities() {
        //
    }

    void LlvmEmittedEntities::set(const std::shared_ptr<Construct>& construct, llvm::Value *llvmValue) {
        this->entities.set(construct, llvmValue, true);
    }

    bool LlvmEmittedEntities::contains(std::shared_ptr<Construct> construct) const {
        return this->entities.contains(std::move(construct));
    }
}
