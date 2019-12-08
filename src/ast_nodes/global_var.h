#pragma once

#include <optional>
#include <string>
#include "code_gen/node.h"
#include "code_gen/node_kind.h"
#include "type.h"
#include "value.h"

namespace ionir
{
class Pass;

class GlobalVar : public Node
{
protected:
    Type *type;

    std::string id;

    std::optional<Value *> value;

public:
    GlobalVar(Type *type, std::string id, std::optional<Value *> value = std::nullopt);

    ~GlobalVar();

    Node *accept(Pass *visitor) override;

    Type *getType() const;

    std::string getId() const;

    std::optional<Value *> getValue() const;
};
} // namespace ionir