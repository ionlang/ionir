#pragma once

#include "interface_descriptor.h"

namespace ionir {
    class ClassDescriptor : public InterfaceDescriptor {
    private:
        std::optional<std::shared_ptr<ClassDescriptor>> parent;

    public:
        //        std::optional<std::shared_ptr<MethodDescriptor>> findMethod(std::string name);

        //        bool containsMethod(std::string name);
    };
}
