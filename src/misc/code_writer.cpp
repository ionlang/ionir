#include <queue>
#include <ionir/misc/code_writer.h>

namespace ionir {
    void CodeWriter::createModule(const std::shared_ptr<Module>& construct) {
        // TODO
    }

    void CodeWriter::createGlobal(const std::shared_ptr<Global>& construct) {
        // TODO
    }

    CodeWriter::CodeWriter() noexcept :
        buffer() {
        //
    }

    std::string CodeWriter::make(const std::shared_ptr<Construct>& rootConstruct) const {
        std::queue<std::shared_ptr<Construct>> queue = {};

        // Push initial items to the queue.
        for (const auto& item : rootConstruct->getChildren()) {
            queue.push(item);
        }

        while (!queue.empty()) {
            // TODO: Front or back? Not tested!
            std::shared_ptr<Construct> item = queue.front();

            queue.pop();

            std::vector<std::shared_ptr<Construct>> children = item->getChildren();

            for (const auto& child : children) {
                queue.push(child);
            }
        }

        return this->getBuffer();
    }

    std::string CodeWriter::getBuffer() const noexcept {
        return this->buffer;
    }

    void CodeWriter::clear() noexcept {
        this->buffer.clear();
    }

    std::string CodeWriter::consumeBuffer() noexcept {
        std::string buffer = this->buffer;

        this->clear();

        return buffer;
    }
}
