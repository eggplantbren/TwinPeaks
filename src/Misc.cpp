#include "Misc.h"
#include <cassert>

namespace TwinPeaks
{

bool all_above(const std::vector<double>& xs,
               const std::vector<double>& ys)
{
    assert(xs.size() == ys.size());

    for(std::size_t i=0; i<xs.size(); ++i)
        if(xs[i] <= ys[i])
            return false;

    return true;
}


} // namespace
