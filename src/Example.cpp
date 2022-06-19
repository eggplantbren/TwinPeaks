#include "Example.h"

namespace TwinPeaks
{

Example::Example(Tools::RNG& rng)
:xs(size)
{
    for(double& x: xs)
        x = rng.rand();
}


} // namespace
