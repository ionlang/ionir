#pragma once

#include <ionshared/misc/helpers.h>
#include <ionir/construct/module.h>
#include <ionir/construct/global.h>

namespace ionir {
    class CodeWriter {
    private:
        std::string buffer;

        void createModule(const ionshared::Ptr<Module>& construct);

        void createGlobal(const ionshared::Ptr<Global>& construct);

    public:
        CodeWriter();

        [[nodiscard]] std::string make(const ionshared::Ptr<Construct>& rootConstruct) const;

        [[nodiscard]] std::string getBuffer() const noexcept;

        void clear() noexcept;

        std::string consumeBuffer() noexcept;
    };
}
