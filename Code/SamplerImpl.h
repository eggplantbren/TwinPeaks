
/*********************************************************************
 *			IMPLEMENTATIONS BEGIN			     *
 *********************************************************************/

template<class Type>
Sampler<Type>::Sampler(int num_particles, int num_iterations,
				int mcmc_steps, int thin)
:particles(num_particles)
,threshold(particles[0].get_scalars().size(), std::vector<double>(2))
,num_iterations(num_iterations)
,mcmc_steps(mcmc_steps)
,thin(thin)
,iteration(2)
{

}

template<class Type>
void Sampler<Type>::initialise()
{
	for(size_t i=0; i<particles.size(); i++)
	{
		particles[i].from_prior();
		particles[i].from_prior_tiebreakers();
	}

	// Set initial threshold
	for(size_t i=0; i<threshold.size(); i++)
	{
		threshold[i][0] = -1E300;
		threshold[i][1] = 0.;
	}

	iteration[0] = 0; iteration[1] = 0;
	switch_time = DNest3::randInt(num_iterations);
}

template<class Type>
void Sampler<Type>::update()
{
	// Open files
	std::fstream sample_info_file("sample_info.txt", std::ios::out|std::ios::app);
	std::fstream scalars_file("scalars.txt", std::ios::out|std::ios::app);
	std::fstream scalars_thinned_file("scalars_thinned.txt", std::ios::out|std::ios::app);
	std::fstream sample_file("sample.txt", std::ios::out|std::ios::app);

	// Choose a scalar
	int which_scalar = (iteration[0] < switch_time)?(0):(1);

	iteration[which_scalar] += 1;

	// Find worst particle
	int worst = find_worst(which_scalar);

	// Write out its info and scalars
	sample_info_file<<particles.size()<<' ';
	sample_info_file<<iteration[0]<<' '<<iteration[1]<<std::endl;

	// Save to thinned files with probability 1/thin
	if(DNest3::randomU() <= 1./thin)
	{
		for(size_t i=0; i<particles[worst].get_scalars().size();  i++)
			scalars_thinned_file<<particles[worst].get_scalars()[i]<<' ';
		scalars_thinned_file<<std::endl;
		sample_file<<particles[worst]<<std::endl;
	}

	// Close files
	sample_info_file.close();
	scalars_thinned_file.close(); sample_file.close();

	// Set the new threshold
	threshold[which_scalar][0] = particles[worst].get_scalars()[which_scalar];
	threshold[which_scalar][1] = particles[worst].get_tiebreakers()[which_scalar];

	std::cout<<"# Iteration "<<(iteration[0] + iteration[1])<<"/"<<num_iterations<<".";
	std::cout<<std::endl;
	
	for(size_t i=0; i<particles[worst].get_scalars().size();  i++)
		scalars_file<<particles[worst].get_scalars()[i]<<' ';
	scalars_file<<std::endl;
	scalars_file.close();

	double logX = -(double)(iteration[0])/particles.size();
	double logY = -(double)(iteration[1])/particles.size();

	std::cout<<"# (logX, logY) ~= ("<<logX<<", "<<logY<<"), scalars = (";
	std::cout<<particles[worst].get_scalars()[0]<<", ";
	std::cout<<particles[worst].get_scalars()[1]<<")."<<std::endl;
	std::cout<<"# Evolving...";

	// Copy a survivor
	if(particles.size() != 1)
	{
		int which;
		do
		{
			which = DNest3::randInt(particles.size());
		}while(which == worst);
		particles[worst] = particles[which];
	}

	// Evolve the particle
	int accepts = 0;
	for(int i=0; i<mcmc_steps; i++)
	{
		Type proposal = particles[worst];
		double logH = proposal.perturb();
		logH += proposal.perturb_tiebreakers();
		if(proposal.is_above(threshold) &&
				DNest3::randomU() <= exp(logH))
		{
			particles[worst] = proposal;
			accepts++;
		}
	}
	std::cout<<"done. Accepted "<<accepts<<"/"<<mcmc_steps<<".";
	std::cout<<std::endl<<std::endl<<std::endl;
}

template<class Type>
void Sampler<Type>::run()
{
	for(int i=0; i<num_iterations; i++)
		update();
}

template<class Type>
int Sampler<Type>::find_worst(int which_scalar) const
{
	int worst = 0;
	for(size_t i=0; i<particles.size(); i++)
	{
		bool is_worse = false;
		if(particles[i].get_scalars()[which_scalar] <
				particles[worst].get_scalars()[which_scalar])
			is_worse = true;

		if(particles[i].get_scalars()[which_scalar] ==
				particles[worst].get_scalars()[which_scalar])
			if(particles[i].get_tiebreakers()[which_scalar] <
				particles[worst].get_tiebreakers()[which_scalar])
			is_worse = true;

		if(is_worse)
			worst = i;
	}
	return worst;
}

