#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstdint>

namespace util::timing {
    // Get current time units
    [[nodiscard]] int64_t Get();

    // Get time units per second
    [[nodiscard]] int64_t Frequency();
} // namespace util::timing

#endif
