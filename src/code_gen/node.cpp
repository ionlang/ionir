#include "node.h"

namespace ionir
{
Node::Node(NodeKind type) : type(type)
{
    //
}

NodeKind Node::getNodeKind() const
{
    return this->type;
}

bool Node::verify() const
{
    return true;
}

std::shared_ptr<Node> Node::getPtr()
{
    return this->shared_from_this();
}
} // namespace ionir
