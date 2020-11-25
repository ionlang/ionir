#include <ionir/const/const.h>
#include <ionir/tracking/name_mangler.h>

namespace ionir {
    std::string NameMangler::joinStrings(
        const std::string& stringA,
        const std::string& stringB
    ) noexcept {
        return stringA + "." + stringB;
    }

    std::string NameMangler::mangle(const std::string& name) noexcept {
        return NameMangler::joinStrings(Const::nameManglerIonPrefix, name);
    }

    std::string NameMangler::mangle(
        const std::shared_ptr<Module>& module,
        const std::string& name
    ) noexcept {
        return NameMangler::mangle(
            NameMangler::joinStrings(**module->identifier, name)
        );
    }

    std::string NameMangler::mangle(
        const std::shared_ptr<TypeStruct>& structType,
        const std::string& name
    ) noexcept {
        return NameMangler::mangle(
            NameMangler::joinStrings(structType->typeName, name)
        );
    }
}
