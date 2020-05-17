#ifndef UTILITY_IO_HPP
#define UTILITY_IO_HPP

#include <optional>
#include <string>
#include <string_view>

namespace util {
    // get the contents of a file as a string
    std::optional<std::string> StringFromFile(std::string_view filename);
} // namespace util

#endif