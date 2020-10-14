#include <ionir/llvm/llvm_emitted_entities.h>

namespace ionir {
    LlvmEmittedEntities::LlvmEmittedEntities() noexcept :
        entities() {
        //
    }

    void LlvmEmittedEntities::set(const ionshared::Ptr<Construct> &construct, llvm::Value *llvmValue) {
        this->entities.set(construct, llvmValue, true);
    }

    bool LlvmEmittedEntities::contains(ionshared::Ptr<Construct> construct) const {
        return this->entities.contains(std::move(construct));
    }
}
