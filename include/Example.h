#ifndef TwinPeaks_Example_h
#define TwinPeaks_Example_h

#include <string>
#include "Tools/RNG.hpp"
#include <vector>

namespace TwinPeaks
{

class Example
{
    private:
        static constexpr int size = 20;
        std::vector<double> xs;

    public:
        Example(Tools::RNG& rng);
        double perturb(Tools::RNG& rng);
        std::vector<double> scalars() const;
        std::string to_string() const;
};

} // namespace

#endif
