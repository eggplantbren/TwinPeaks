#ifndef TwinPeaks_Sampler_h
#define TwinPeaks_Sampler_h

#include <vector>

namespace TwinPeaks
{

template<typename T>
class Sampler
{
    private:

        // Constants here for now
        static constexpr int num_particles = 101;
        static constexpr int mcmc_steps = 1000;

        std::vector<double> direction;
        std::vector<T> particles;
        std::vector<std::vector<double>> scalars;

    public:

        // Initialise
        Sampler(Tools::RNG& rng);

        // Update
        void update(Tools::RNG& rng);

};

} // namespace


#endif

#include "SamplerImpl.h"
