#pragma once

#include <string>
#include <utility>
#include <ionshared/misc/util.h>
#include <ionir/tracking/scope_anchor.h>
#include <ionir/tracking/context.h>
#include <ionir/misc/identifiable.h>
#include "type.h"
#include "construct.h"

namespace ionir {
    struct Pass;

    struct Function;

    struct Extern;

    struct TypeStruct;

    struct Global;

    /**
     * The root construct. Contains top-level constructs, such as
     * functions, global variables, struct types, among others.
     */
    class Module : public Construct, public Identifiable {
    private:
        std::shared_ptr<Context> context;

        bool insertChild(
            const std::string& name,
            const std::shared_ptr<Construct>& childConstruct
        ) noexcept;

    public:
        friend class LlvmLoweringPass;

        explicit Module(
            std::shared_ptr<Identifier> identifier,
            std::shared_ptr<Context> context = std::make_shared<Context>()
        ) noexcept;

        void accept(Pass& visitor) override;

        [[nodiscard]] Ast getChildren() override;

        bool insert(const std::shared_ptr<Function>& function);

        bool insert(const std::shared_ptr<Extern>& externConstruct);

        bool insert(const std::shared_ptr<Global>& global);

        bool insert(const std::shared_ptr<TypeStruct>& structType);

        template<typename T = Construct>
            requires std::derived_from<T, Construct>
        std::optional<std::shared_ptr<T>> lookup(std::string name) {
            std::optional<std::shared_ptr<Construct>> lookupResult =
                this->context->globalScope->lookup(std::move(name));

            if (!ionshared::util::hasValue(lookupResult)) {
                return std::nullopt;
            }

            return lookupResult->get()->dynamicCast<T>();
        }

        /**
         * Attempt to move context scopes (including global scope) from
         * this module to another. Will return false if there are conflicts
         * (keys which already exist on the target) or true otherwise. No
         * changes will be committed if any conflict occurs (or the process
         * fails).
         */
        bool mergeInto(const std::shared_ptr<Module>& module);
    };
}
