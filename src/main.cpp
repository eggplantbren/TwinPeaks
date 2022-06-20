#include "Example.h"
#include "Sampler.h"

using namespace TwinPeaks;

int main()
{
    Tools::RNG rng;
    Sampler<Example> sampler(rng);

    for(int i=0; i<100; ++i)
        sampler.update(rng);

    return 0;
}
