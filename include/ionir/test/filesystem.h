#pragma once

#include <optional>
#include <string>

namespace ionir::test::fs {
    /**
     * Resolves a path relative to the root directory of
     * the project.
     */
    [[nodiscard]] std::string resolvePath(const std::string& path);

    [[nodiscard]] std::string resolveTestPath(const std::string& path);

    /**
     * Joins two paths with a directory separator.
     */
    [[nodiscard]] std::string joinPaths(
        const std::string& pathA,
        const std::string& pathB
    );

    /**
     * Reads the contents of a file if it exists, otherwise
     * returns null.
     */
    [[nodiscard]] std::optional<std::string> readFileContents(const std::string& path);

    [[nodiscard]] std::optional<std::string> readTestFile(const std::string& filePath);

    /**
     * Determines whether a file path exists.
     */
    [[nodiscard]] bool exists(const std::string& filename);
}
