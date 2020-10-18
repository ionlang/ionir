#pragma once

#include <llvm/IR/Value.h>
#include <ionshared/container/map.h>
#include <ionshared/misc/helpers.h>
#include <ionir/construct/construct.h>

namespace ionir {
    class LlvmEmittedEntities {
    private:
        ionshared::Map<std::shared_ptr<Construct>, llvm::Value*> entities;

    public:
        LlvmEmittedEntities() noexcept;

        void set(const std::shared_ptr<Construct>& construct, llvm::Value* llvmValue);

        [[nodiscard]] bool contains(std::shared_ptr<Construct> construct) const;

        template<typename T = llvm::Value>
        [[nodiscard]] std::optional<T*> find(
            const std::shared_ptr<Construct>& construct,
            bool useDynamicCast = true
        ) {
            if (this->contains(construct)) {
                T *castResult = llvm::dyn_cast<T>(*this->entities.lookup(construct));

                if (castResult != nullptr) {
                    return castResult;
                }

                // TODO: Should be internal error diagnostic.
                throw std::runtime_error("LLVM value cast failed");
            }

            return std::nullopt;
        }
    };
}
