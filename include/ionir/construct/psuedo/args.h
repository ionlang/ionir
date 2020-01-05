#pragma once

#include <vector>
#include <utility>
#include <string>
#include <ionir/misc/helpers.h>
#include <ionir/construct/type.h>

namespace ionir {
    typedef std::pair<Type &, std::string> Arg;

    class Args {
    private:
        std::vector<Arg> items;

        bool isInfinite;

    public:
        explicit Args(std::vector<Arg> items = {}, bool isInfinite = false);

        std::vector<Arg> getItems() const;

        bool getIsInfinite() const;

        void setIsInfinite(bool isInfinite);
    };
}
