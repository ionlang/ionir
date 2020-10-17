#include <queue>
#include <ionir/misc/code_writer.h>

namespace ionir {
    void CodeWriter::createModule(const ionshared::Ptr<Module>& construct) {
        // TODO
    }

    void CodeWriter::createGlobal(const ionshared::Ptr<Global>& construct) {
        // TODO
    }

    CodeWriter::CodeWriter() noexcept = default;

    std::string CodeWriter::make(const ionshared::Ptr<Construct>& rootConstruct) const {
        std::queue<ionshared::Ptr<Construct>> queue = {};

        // Push initial items to the queue.
        for (const auto& item : rootConstruct->getChildrenNodes()) {
            queue.push(item);
        }

        while (!queue.empty()) {
            // TODO: Front or back? Not tested!
            ionshared::Ptr<Construct> item = queue.front();

            queue.pop();

            std::vector<ionshared::Ptr<Construct>> children = item->getChildrenNodes();

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
