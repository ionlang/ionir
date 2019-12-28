#pragma once

#include <optional>
#include <string>
#include <vector>
#include <ionir/misc/helpers.h>
#include "psuedo/args.h"
#include "insts/inst.h"
#include "child_construct.h"
#include "type.h"

namespace ionir {
    class Pass;

    class Block;

    enum class SectionKind {
        /**
         * The entry section of a block.
         */
            Entry,

        /**
         * A section defined by the user. Can be jumped to
         * using a goto instruction.
         */
            Label,

        /**
         * A section which forms part of a construct. Cannot be
         * directly accessed by the user.
         */
            Internal
    };

    struct SectionOpts : ChildConstructOpts<Block> {
        SectionKind kind;

        std::string id;

        std::vector<Ptr < Inst>> insts = {};
    };

    class Section : public ChildConstruct<Block> {
    private:
        SectionKind kind;

    protected:
        std::string id;

        std::vector<Ptr < Inst>> insts;

    public:
        explicit Section(SectionOpts opts);

        void accept(Pass *visitor) override;

        SectionKind getKind() const;

        std::string getId() const;

        void setId(std::string id);

        std::vector<Ptr < Inst>> &

        getInsts();

        void setInsts(std::vector<Ptr < Inst>>

        insts);

        uint32_t relocateInsts(Ptr <Section> target, uint32_t from = 0);

        /**
         * Attempt to find the index location
         * of an instruction. Returns null if
         * not found.
         */
        std::optional<uint32_t> locate(Ptr <Inst> inst);
    };
}