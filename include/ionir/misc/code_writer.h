#pragma once

#include <ionshared/misc/helpers.h>
#include <ionir/construct/module.h>
#include <ionir/construct/global.h>

namespace ionir {
    class CodeWriter {
    private:
        std::string buffer;

        void createModule(const std::shared_ptr<Module>& construct);

        void createGlobal(const std::shared_ptr<Global>& construct);

    public:
        CodeWriter() noexcept;

        [[nodiscard]] std::string make(const std::shared_ptr<Construct>& rootConstruct) const;

        [[nodiscard]] std::string getBuffer() const noexcept;

        void clear() noexcept;

        std::string consumeBuffer() noexcept;
    };
}
