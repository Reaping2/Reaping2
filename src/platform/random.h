#pragma once
#ifndef INCLUDED_PLATFORM_RANDOM_H
#define INCLUDED_PLATFORM_RANDOM_H

#include "rstdint.h"
#include <memory>

namespace platform {
namespace detail {
class RandomGeneratorImpl;
}
class RandomGenerator
{
    std::unique_ptr<detail::RandomGeneratorImpl> mImpl;
public:
    RandomGenerator();
    ~RandomGenerator();
    uint32_t operator()();
    void seed( uint32_t seed );
    static RandomGenerator& global();
};

}

#endif // INCLUDED_PLATFORM_RANDOM_H

