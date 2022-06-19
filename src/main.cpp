#include "Example.h"
#include "Sampler.h"

using namespace TwinPeaks;

int main()
{
    Tools::RNG rng;
    Sampler<Example> sampler(rng);
    sampler.update(rng);

    return 0;
}
