#include <utility>
#include <ionir/tracking/interface_descriptor.h>

namespace ionir {
    std::string InterfaceDescriptor::getName() const {
        return this->name;
    }

    void InterfaceDescriptor::setName(std::string name) {
        this->name = std::move(name);
    }

    PtrSymbolTable<FieldDescriptor> InterfaceDescriptor::getFields() const {
        return this->fields;
    }

    void InterfaceDescriptor::setFields(PtrSymbolTable<FieldDescriptor> fields) {
        this->fields = std::move(fields);
    }

    //    PtrSymbolTable<MethodDescriptor> InterfaceDescriptor::getMethods() const {
    //        return this->methods;
    //    }
    //
    //    void InterfaceDescriptor::setMethods(PtrSymbolTable<MethodDescriptor> methods) {
    //        this->methods = methods;
    //    }

    std::vector<std::shared_ptr<InterfaceDescriptor>> InterfaceDescriptor::getInterfaces() const {
        return this->interfaces;
    }

    void InterfaceDescriptor::setInterfaces(std::vector<std::shared_ptr<InterfaceDescriptor>> interfaces) {
        this->interfaces = std::move(interfaces);
    }

    std::optional<std::shared_ptr<FieldDescriptor>> InterfaceDescriptor::findField(const std::string &name) {
        if (this->fields->contains(name)) {
            return *this->fields->lookup(name);
        }

        for (const auto& interface : this->interfaces) {
            std::optional<std::shared_ptr<FieldDescriptor>> field = interface->findField(name);

            if (field.has_value()) {
                return field;
            }
        }

        return std::nullopt;
    }

    bool InterfaceDescriptor::containsField(const std::string &name) {
        return this->findField(name) != std::nullopt;
    }
}
