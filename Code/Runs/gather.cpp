#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;


double logsumexp(double x1, double x2)
{
	if(x1 > x2)
		return x1 + log(1. + exp(x2 - x1));
	return x2 + log(1. + exp(x1 - x2));
}


int main()
{
	// Temperatures
	double T1 = 0.1; double T2 = 1.;

	// Prior weights (relative) and 'likelihoods'
	vector< vector<double> > logw, logL;

	// Directories to get runs from
	vector<string> directories;
	directories.push_back(string(".."));

	// Output precision
	cout<<setprecision(8);

	int k = 0;
	for(size_t i=0; i<directories.size(); i++)
	{
		string filename1 = directories[i] + string("/logw.txt");
		fstream fin1(filename1.c_str(), ios::in);

		string filename2 = directories[i] + string("/scalars.txt");
		fstream fin2(filename2.c_str(), ios::in);

		double temp1, temp2, temp3;
		while(fin1>>temp1 && fin2>>temp2 && fin2>>temp3)
		{
			if(logw.size() == 0 || temp1 == logw[0][0])
			{
				logw.push_back(vector<double>());
				logL.push_back(vector<double>());
			}

			logw.back().push_back(temp1);
			logL.back().push_back(temp2/T1 + temp3/T2);

			k++;
		}

		fin1.close();
		fin2.close();

		cout<<"# Loaded "<<k<<" points."<<endl;
	}

	// Normalise prior weights for each run
	for(size_t i=0; i<logw.size(); i++)
	{
		double tot = -1E300;
		for(size_t j=0; j<logw[i].size(); j++)
			tot = logsumexp(tot, logw[i][j]);
		for(size_t j=0; j<logw[i].size(); j++)
			logw[i][j] -= tot;
	}

	// Use each run to calculate logZ
	vector<double> logZ(logw.size(), -1E300);
	for(size_t i=0; i<logw.size(); i++)
	{
		for(size_t j=0; j<logw[i].size(); j++)
			logZ[i] = logsumexp(logZ[i], logw[i][j] + logL[i][j]);
	}

	// Use each run to calculate H
	vector<double> H(logw.size(), 0.);
	for(size_t i=0; i<logw.size(); i++)
	{
		for(size_t j=0; j<logw[i].size(); j++)
		{
			H[i] += exp(logw[i][j] + logL[i][j] - logZ[i])
					*(logL[i][j] - logZ[i]);
		}
	}

	// Print estimates
	for(size_t i=0; i<logw.size(); i++)
		cout<<logZ[i]<<' '<<H[i]<<endl;

	return 0;
}

