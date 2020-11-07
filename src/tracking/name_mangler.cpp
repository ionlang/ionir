#include <ionir/const/const.h>
#include <ionir/tracking/name_mangler.h>

namespace ionir {
    std::string NameMangler::join(
        const std::string &stringA,
        const std::string &stringB
    ) noexcept {
        return stringA + "." + stringB;
    }

    std::string NameMangler::mangle(const std::string& name) noexcept {
        return NameMangler::join(Const::nameManglerIonPrefix, name);
    }

    std::string NameMangler::mangle(
        const std::shared_ptr<Module>& module,
        const std::string& name
    ) noexcept {
        return NameMangler::mangle(
            NameMangler::join(**module->identifier, name)
        );
    }
}
