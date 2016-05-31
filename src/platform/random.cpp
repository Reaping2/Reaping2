#include "random.h"
#include "singleton.h"
#include <random>

namespace platform {

namespace detail {
class RandomGeneratorImpl
{
public:
    std::mt19937 mGenerator;
};
typedef  Singleton<RandomGenerator> RandomGeneratorGlobal;
}

RandomGenerator::RandomGenerator()
    : mImpl( new detail::RandomGeneratorImpl )
{
}

RandomGenerator::~RandomGenerator()
{
}

uint32_t RandomGenerator::operator()()
{
    return mImpl->mGenerator();
}

void RandomGenerator::seed( uint32_t seed )
{
    mImpl->mGenerator.seed( seed );
}

RandomGenerator& RandomGenerator::global()
{
    return detail::RandomGeneratorGlobal::Get();
}

}

