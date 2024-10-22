#include <type_traits>
#ifdef INSTRUMENTATION_HEADER
#pragma once
#endif

#include <atomic>
#include <filesystem>
#include <probe.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

#ifndef INSTRUMENTATION_HEADER
#define inline
#endif

#define __INSTRUMENTATION_CONCAT(a, b) a##b
#define INSTRUMENTATION_CONCAT(a, b) __INSTRUMENTATION_CONCAT(a, b)
#define INSTRUMENTATION_DECLARE(ret_type, name)                                                                        \
    __attribute__((used)) __attribute__((visibility("default"))) inline ret_type INSTRUMENTATION_CONCAT(               \
        INSTRUMENTATION_PREFIX, name)

namespace
{
thread_local bool valid = true;
}; // namespace
namespace INSTRUMENTATION_PREFIX
{

using namespace std;

template <typename T>
inline void probe(int64_t ptr, T val, int mode)
{
    printf("prt:  %ld\n", ptr);
}

#define INSTRUMENTATION_BUILD(T)                                                                                       \
    INSTRUMENTATION_DECLARE(void, _probe_##T)                                                                          \
    (int64_t ptr, T val, int type) { INSTRUMENTATION_PREFIX::probe(ptr, val, type); }

} // namespace INSTRUMENTATION_PREFIX
using int1_t = bool;
extern "C"
{
    INSTRUMENTATION_BUILD(int64_t)
    INSTRUMENTATION_BUILD(int32_t)
    INSTRUMENTATION_BUILD(int8_t)
    INSTRUMENTATION_BUILD(int1_t)
    INSTRUMENTATION_BUILD(float)
    INSTRUMENTATION_BUILD(double)
}

#undef INSTRUMENTATION_BUILD
#undef INSTRUMENTATION_PREFIX
#undef INSTRUMENTATION_CONCAT
#undef __INSTRUMENTATION_CONCAT
#undef INSTRUMENTATION_DECLARE
