#include <ionir/misc/type_factory.h>

namespace ionir {
    std::shared_ptr<IntegerType> TypeFactory::typeInteger(IntegerKind integerKind, bool isSigned) {
        return std::make_shared<IntegerType>(integerKind, isSigned);
    }

    std::shared_ptr<BooleanType> TypeFactory::typeBoolean() {
        return std::make_shared<BooleanType>();
    }

    std::shared_ptr<IntegerType> TypeFactory::typeChar() {
        return TypeFactory::typeInteger(IntegerKind::Int8, false);
    }

    std::shared_ptr<Type> TypeFactory::typeString() {
        // TODO: Awaiting array types support.
        return TypeFactory::typeChar();
    }

    std::shared_ptr<VoidType> TypeFactory::typeVoid() {
        return std::make_shared<VoidType>();
    }
}
