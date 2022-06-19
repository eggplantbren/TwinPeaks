#include "Example.h"
#include "Tools/Misc.hpp"
#include <sstream>

namespace TwinPeaks
{

Example::Example(Tools::RNG& rng)
:xs(size)
{
    for(double& x: xs)
        x = rng.rand();
}


double Example::perturb(Tools::RNG& rng)
{
    int k = rng.rand_int(size);
    xs[k] += rng.randh();
    Tools::wrap(xs[k]);
    return 0.0;
}

std::vector<double> Example::scalars() const
{
    double f = 0.0;
    double g = 0.0;

    for(double x: xs)
    {
        f += -0.5*1E4*pow(x - 0.5, 2);
        g += -1E2*x;
    }

    return {f, g};
}

std::string Example::to_string() const
{
    std::stringstream ss;
    for(double x: xs)
        ss << x << ' ';
    return ss.str();
}


} // namespace
