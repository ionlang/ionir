#pragma once

#include <sstream>
#include <vector>
#include <ionir/construct/construct.h>

namespace ionir {
    struct AstPrinterTreeNode {
        uint32_t depth;

        bool isLeaf;

        std::shared_ptr<Construct> construct;
    };

    class AstPrinter {
    private:
        const Ast &ast;

        std::stringstream tree;

        uint32_t depth;

    protected:
        void visitNode(const AstPrinterTreeNode &node);

    public:
        static std::string makeSpaces(uint32_t depth);

        explicit AstPrinter(const Ast &ast);

        std::optional<std::string> make();

        void reset();
    };
}
