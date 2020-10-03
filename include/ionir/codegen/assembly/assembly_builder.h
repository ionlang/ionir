#pragma once

#include <sstream>
#include <utility>
#include <ionshared/misc/util.h>
#include <ionshared/misc/helpers.h>

namespace ionir {
    template<typename T>
    class AssemblyBuilder : std::enable_shared_from_this<T> {
    private:
        std::stringstream buffer;

    public:
        const uint32_t instructionSpaces;

        explicit AssemblyBuilder(uint32_t instructionSpaces = 4) :
            instructionSpaces(instructionSpaces) {
            //
        }

        virtual ionshared::Ptr<T> makeSection(std::string name) {
            this->buffer
                << "."
                << name;
        }

        virtual ionshared::Ptr<T> makeInst(
            std::string name,
            std::vector<std::string> arguments
        ) {
            this->buffer
                << ionshared::util::makeSpaces(this->instructionSpaces)
                << name;

            bool prime = true;

            for (const auto &argument : arguments) {
                if (!prime) {
                    this->buffer << ", ";
                }
                else {
                    prime = false;
                }

                this->buffer << " " << argument;
            }

            return this->shared_from_this();
        }

        virtual ionshared::Ptr<T> makeInst(std::string name, std::string argument) {
            return this->makeInst(name, std::vector<std::string>({
                std::move(argument)
            }));
        }

        virtual ionshared::Ptr<T> makeComment(
            std::string text,
            bool spaces = true
        ) noexcept {
            this->buffer
                << (spaces ? " " : "")
                << ";"
                << (spaces ? " " : "")
                << text;

            return this->shared_from_this();
        }

        void clear() {
            this->buffer.clear();
        }
    };
}
