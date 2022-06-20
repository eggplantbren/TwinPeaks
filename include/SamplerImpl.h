#ifndef TwinPeaks_SamplerImpl_h
#define TwinPeaks_SamplerImpl_h

#include <algorithm>
#include <cassert>
#include <iostream>
#include "Misc.h"

namespace TwinPeaks
{

template<typename T>
Sampler<T>::Sampler(Tools::RNG& rng)
:iteration(0)
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
    threshold = std::vector<double>(direction.size(), -1E300);

    std::cout << '\n' << std::endl;
}


template<typename T>
void Sampler<T>::update(Tools::RNG& rng)
{
    ++iteration;
    std::cout << "Iteration " << iteration << ": Updating sampler." << std::endl;

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

    // Save all particles below the median
    for(int i=0; i<num_particles/2; ++i)
    {
        database.save_particle(particles[indices[i]].to_string(),
                               scalars[indices[i]]);
    }

    // Set threshold
    threshold[which_scalar] = scalars[indices[num_particles/2]][which_scalar];
    std::cout << "New threshold: ";
    for(double t: threshold)
        std::cout << t << ' ';
    std::cout << std::endl;

    // Refresh particles
    std::cout << "Refreshing particles..." << std::flush;
    for(int i=0; i<num_particles/2; ++i)
    {
        int copy = rng.rand_int(num_particles/2 + 1) + num_particles/2;
        copy = indices[copy];

        particles[indices[i]] = particles[copy];
        scalars[indices[i]] = scalars[copy];
        do_mcmc(indices[i], rng);
    }
    std::cout << "done." << std::endl;


}


template<typename T>
void Sampler<T>::do_mcmc(int k, Tools::RNG& rng)
{
    for(int i=0; i<mcmc_steps; ++i)
    {
        T proposal = particles[k];
        double logh = proposal.perturb(rng);
        if(rng.rand() <= exp(logh))
        {
            auto proposal_scalars = proposal.scalars();
            if(all_above(proposal_scalars, threshold))
            {
                particles[k] = proposal;
                scalars[k] = proposal_scalars;
            }
        }
    }
}

} // namespace


#endif
