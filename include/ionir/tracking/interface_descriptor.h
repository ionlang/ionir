#pragma once

#include <optional>
#include <vector>
#include <string>
#include "field_descriptor.h"
#include "symbol_table.h"

namespace ionir {
    class InterfaceDescriptor {
    private:
        std::string name;

        PtrSymbolTable<FieldDescriptor> fields;

        //        PtrSymbolTable<MethodDescriptor> methods;

        std::vector<std::shared_ptr<InterfaceDescriptor>> interfaces;

    public:
        std::string getName() const;

        void setName(std::string name);

        PtrSymbolTable<FieldDescriptor> getFields() const;

        void setFields(PtrSymbolTable<FieldDescriptor> fields);

        //PtrSymbolTable<MethodDescriptor> getMethods() const;

        //void setMethods(PtrSymbolTable<MethodDescriptor> methods);

        std::vector<std::shared_ptr<InterfaceDescriptor>> getInterfaces() const;

        void setInterfaces(std::vector<std::shared_ptr<InterfaceDescriptor>> interfaces);

        std::optional<std::shared_ptr<FieldDescriptor>> findField(const std::string &name);

        bool containsField(const std::string &name);
    };
}
