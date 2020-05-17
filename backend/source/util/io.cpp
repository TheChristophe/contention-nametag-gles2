#include "io.hpp"

#include <fstream>

namespace util {
    std::optional<std::string> StringFromFile(std::string_view filename)
    {
        std::ifstream in(filename.data(), std::ios::in | std::ios::binary);
        if (!in) {
            return std::nullopt;
        }

        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return std::make_optional<std::string>(contents);
    }
} // namespace util