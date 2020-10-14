#pragma once

namespace ionir {
    template<typename T>
    class Wrapper {
    protected:
        T value;

    public:
        explicit Wrapper(T value) noexcept :
            value(value) {
            //
        }

        [[nodiscard]] virtual T& unwrap() {
            return this->value;
        }

        [[nodiscard]] virtual const T& unwrapConst() const {
            return this->value;
        }
    };
}
