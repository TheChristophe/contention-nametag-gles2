#include "time.hpp"

#include <ctime>
#include <sys/time.h>
#include <unistd.h>

namespace util {
    namespace timing {
        int64_t Get()
        {
            struct timeval val;
            gettimeofday(&val, nullptr);
            return val.tv_sec * static_cast<int64_t>(1000000) + val.tv_usec;
        }

        int64_t Frequency()
        {
            return 1000000;
        }
    } // namespace timing
} // namespace util
