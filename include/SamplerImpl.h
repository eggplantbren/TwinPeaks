#ifndef TwinPeaks_SamplerImpl_h
#define TwinPeaks_SamplerImpl_h

#include <iostream>

namespace TwinPeaks
{

template<typename T>
Sampler<T>::Sampler(Tools::RNG& rng)
{
    std::cout << "Initialising sampler..." << std::flush;
    for(int i=0; i<num_particles; ++i)
    {
        T t(rng);
        std::vector<double> ss = t.scalars();
        particles.emplace_back(std::move(t));
        scalars.emplace_back(std::move(ss));
    }


    std::cout << "done." << std::endl;
}

} // namespace


#endif
