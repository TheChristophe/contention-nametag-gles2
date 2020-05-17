#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cstdint>

namespace util {
    namespace timing {
        // Get current time units
        int64_t Get();

        // Get time units per second
        int64_t Frequency();
    } // namespace timing
} // namespace util

#endif
