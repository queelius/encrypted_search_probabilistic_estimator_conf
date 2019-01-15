#include <iostream>
#include "known_plaintext_attack.h"
#include "zipf_distribution.h"
//#include "data_smoother.h"
#include <vector>
#include <fstream>
#include <sstream>

void main()
{
    alex::stochastic::Entropy<std::mt19937> entropy(41334);

	alex::stats::zipf_distribution zipf(1000, .5);
	auto results = alex::simulation::known_plaintext_attack(zipf, 1000, 5000000, entropy, 5000);
	for (size_t i = 0; i < results.proportion_guessed.size(); ++i)
		std::cout << results.proportion_guessed[i] << "\t" << results.gain[i] << std::endl;


	/*
    unsigned int N = 10000;      // unique words in plain-text queries
    unsigned int M = 1000000;     // sample size
    long double ALPHA = 0.25L;
    {
		// for s = 3
        auto sim = alex::simulation::known_plaintext_attack(N, 3, M, entropy);
        DataSmoother<long double> probs(sim.proportion_guessed);
        auto smoothed_probs = probs.exponential_smoothing(ALPHA);
        std::stringstream ss;
        ss << 3 << "_" << sim.entropy << "_" << ALPHA << "_" << N << "_" << M;
        std::ofstream f(ss.str());
        f << "t\tp\n";
        for (std::size_t i = 1; i < smoothed_probs.size(); i *= 2)
        {
            f << i << '\t' << smoothed_probs[i-1] << '\n';
        }
        f << '\n';
    }
    for (long double s = 0; s < 1; s += .3333334L)
    {
        auto sim = alex::simulation::known_plaintext_attack(N, s, M, entropy);
        DataSmoother<long double> probs(sim.proportion_guessed);
        auto smoothed_probs = probs.exponential_smoothing(ALPHA);
        std::stringstream ss;
        ss << s << "_" << sim.entropy << "_" << ALPHA << "_" << N << "_" << M;
        std::ofstream f(ss.str());
        f << "t\tp\n";
        for (std::size_t i = 1; i < smoothed_probs.size(); i *= 2)
        {
            f << i << '\t' << smoothed_probs[i-1] << '\n';
        }
        f << '\n';
    }
    {
        auto sim = alex::simulation::known_plaintext_attack(N, 1.5, M, entropy);
        DataSmoother<long double> probs(sim.proportion_guessed);
        auto smoothed_probs = probs.exponential_smoothing(ALPHA);
        std::stringstream ss;
        ss << 1.5 << "_" << sim.entropy << "_" << ALPHA << "_" << N << "_" << M;
        std::ofstream f(ss.str());
        f << "t\tp\te\n";
        for (std::size_t i = 1; i < smoothed_probs.size(); i *= 2)
        {
            f << i << '\t' << smoothed_probs[i-1] << '\n';
        }
        f << '\n';
    }
    for (double s = 1; s <= 16; s *= 2)
    {
        auto sim = alex::simulation::known_plaintext_attack(N, s, M, entropy);
        DataSmoother<long double> probs(sim.proportion_guessed);
        auto smoothed_probs = probs.exponential_smoothing(ALPHA);
        std::stringstream ss;
        ss << s << "_" << sim.entropy << "_" << ALPHA << "_" << N << "_" << M;
        std::ofstream f(ss.str());
        f << "t\tp\te\n";
        for (std::size_t i = 1; i < smoothed_probs.size(); i *= 2)
        {
            f << i << '\t' << smoothed_probs[i-1] << '\n';
        }
        f << '\n';
    }
    {
        auto sim = alex::simulation::known_plaintext_attack(N, 512, M, entropy);
        DataSmoother<long double> probs(sim.proportion_guessed);
        auto smoothed_probs = probs.exponential_smoothing(ALPHA);
        std::stringstream ss;
        ss << 512 << "_" << sim.entropy << "_" << ALPHA << "_" << N << "_" << M;
        std::ofstream f(ss.str());
        f << "t\tp\te\n";
        for (std::size_t i = 1; i < smoothed_probs.size(); i *= 2)
        {
            f << i << '\t' << smoothed_probs[i-1] << '\n';
        }
        f << '\n';
    }
	*/
}