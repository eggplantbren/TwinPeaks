#ifndef TwinPeaks_SamplerImpl_h
#define TwinPeaks_SamplerImpl_h

#include <algorithm>
#include <cassert>
#include <iostream>

namespace TwinPeaks
{

template<typename T>
Sampler<T>::Sampler(Tools::RNG& rng)
{
    assert(num_particles % 2 == 1);

    std::cout << "Initialising sampler." << std::endl;
    std::cout << "    Generating " << num_particles;
    std::cout << " particles from the prior..." << std::flush;
    for(int i=0; i<num_particles; ++i)
    {
        T t(rng);
        std::vector<double> ss = t.scalars();
        particles.emplace_back(std::move(t));
        scalars.emplace_back(std::move(ss));
    }
    std::cout << "done." << std::endl;

    std::cout << "    Setting direction..." << std::flush;
    direction = std::vector<double>(scalars[0].size());
    for(double& d: direction)
        d = exp(5.0*rng.randn());
    double max = *max_element(direction.begin(), direction.end());
    for(double& d: direction)
    {
        d /= max;
        std::cout << d << ' ';
    }

    std::cout << '\n' << std::endl;
}


template<typename T>
void Sampler<T>::update(Tools::RNG& rng)
{
    std::cout << "Updating sampler." << std::endl;

    // Choose the relevant scalar
    std::cout << "Choosing scalar..." << std::flush;
    int which_scalar;
    while(true)
    {
        which_scalar = rng.rand_int(direction.size());
        if(rng.rand() <= direction[which_scalar])
            break;
    }
    std::cout << which_scalar << std::endl;

    // Sort the particles
    std::vector<double> s(num_particles);
    std::vector<double> indices(num_particles);
    for(int i=0; i<num_particles; ++i)
    {
        indices[i] = i;
        s[i] = scalars[i][which_scalar];
    }
    std::sort(indices.begin(), indices.end(),
                [&](int i, int j){ return(s[i] < s[j]); });
}

} // namespace


#endif
