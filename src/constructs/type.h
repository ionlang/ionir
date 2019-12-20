#pragma once

#include <memory>
#include <string>
#include "constructs/construct.h"
#include "constructs/construct_kind.h"
#include "misc/helpers.h"

namespace ionir
{
class Pass;

class Type : public Construct
{
protected:
    std::string id;

    bool isPointer;

public:
    Type(std::string id, bool isPointer = false);

    void accept(Pass *visitor) override;

    std::string getId() const;

    bool getIsPointer() const;
};
} // namespace ionir