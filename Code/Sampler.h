#ifndef _Sampler_
#define _Sampler_

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

template<class Type>
class Sampler
{
	private:
		std::vector<Type> particles;
		std::vector< std::vector<double> > threshold;

		int num_iterations, mcmc_steps, thin;
		std::vector<int> iteration;
		int switch_time;

		int find_worst(int which_scalar) const;
		void update();

	public:
		Sampler(int num_particles, int num_iterations, int mcmc_steps,
						int thin);

		void initialise();
		void run();
};

#include "SamplerImpl.h"

#endif

