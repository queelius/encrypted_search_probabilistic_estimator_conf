#ifndef __MARGINAL_KNOWN_PLAINTEXT_ATTACK_H__
#define __MARGINAL_KNOWN_PLAINTEXT_ATTACK_H__

#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>
#include "entropy.h"

using std::pair;
using std::vector;
using std::make_pair;

namespace alex::simulation2
{
	struct KnownPlainTextAttackResult
	{
		vector<long double> gain;
		vector<long double> proportion;			// time series: proportion of time series correctly estimated
												//     proportion[j] = sum [0 to j] 
		vector<unsigned int> matched;			// time series: number of symbols corrected estimated.
												//     note: matched[j] != proportion[j] / N
												//           
		long double entropy;                    // entropy of distribution
		unsigned int N;                         // # unique words in alphabet
		unsigned int m;                         // size of time series
	};

	// A known-plaintext adversary simulation where the true underlying distribution
	// is given by discrete distribution dd
	// a unigram language model.
	template <typename E, typename D>
	KnownPlainTextAttackResult
		marginal_known_plaintext_attack2(
			
			unsigned int N,                         // # unique words
			unsigned int m,                         // # steps in time series 
			E& entropy,								// entropy source (for r.v. generation)
			unsigned int p = 0)						// record every p-th time series point
	{
		vector<unsigned int> h2p(N);
		vector<unsigned int> p2h(N);

		for (unsigned int i = 0; i < N; ++i)
			h2p[i] = i;

		std::shuffle(h2p.begin(), h2p.end(), entropy.engine());
		vector<pair<unsigned int, unsigned int>> freq(N);
		for (unsigned int h = 0; h < N; ++h)
		{
			auto w = h2p[h];
			p2h[w] = h;
			freq[h] = make_pair(h, 0);
		}

		KnownPlainTextAttackResult result;
		result.entropy = dd.entropy();
		result.m = m;
		result.N = N;
		result.gain.resize(m / skip);
		result.matched.resize(m / skip);
		result.proportion_guessed.resize(m / skip);

		// quantile function: map probability p to true symbol
		auto Q = dd.get_inverse_cdf();
		// probability mass function
		auto f = dd.get_pdf();

		for (unsigned int n = 0, k = 0; n < m; ++n)
		{
			// map probability p to the true symbol in {0,...,N-1}
			auto w = Q(entropy.get0_1()) - 1;
			++freq[p2h[w]].second;

			auto hidden_counts = freq;
			std::sort(hidden_counts.begin(), hidden_counts.end(),
				[](const pair<unsigned int, unsigned int>& x1,
					const pair<unsigned int, unsigned int>& x2) -> bool
			{ return x1.second > x2.second; });

			vector<unsigned int> argmax(N);
			for (unsigned int i = 0; i < hidden_counts.size(); ++i)
				argmax[hidden_counts[i].first] = i;

			unsigned int matched = 0;
			alex::math::kbn_sum<long double> pr;
			for (unsigned int i = 0; i < N; ++i)
			{
				if (argmax[i] == h2p[i])
				{
					++matched;
					auto w = h2p[i];
					auto p = f(w + 1);
					pr += p;
				}
			}

			if (p != 0 && n % p == 0)
			{
				result.gain[k] = n * (std::log2(N) - result.entropy);
				result.proportion_guessed[k] = pr();
				result.matched[k] = matched;
				++k;
			}
		}

		return result;
	}

	// A known-plaintext adversary simulation where the true underlying distribution
	// is given by discrete distribution dd
	// a unigram language model.
	template <typename E, typename D>
	KnownPlainTextAttackResult
		marginal_known_plaintext_attack(
			D dd,									// discrete distribution to generate time series from
			unsigned int N,                         // # unique words
			unsigned int m,                         // # steps in time series 
			E& entropy,								// entropy source (for r.v. generation)
			unsigned int p = 0)						// record every p-th time series point
	{
		vector<unsigned int> h2p(N);
		vector<unsigned int> p2h(N);

		for (unsigned int i = 0; i < N; ++i)
			h2p[i] = i;

		std::shuffle(h2p.begin(), h2p.end(), entropy.engine());
		vector<pair<unsigned int, unsigned int>> freq(N);
		for (unsigned int h = 0; h < N; ++h)
		{
			auto w = h2p[h];
			p2h[w] = h;
			freq[h] = make_pair(h, 0);
		}

		KnownPlainTextAttackResult result;
		result.entropy = dd.entropy();
		result.m = m;
		result.N = N;
		result.gain.resize(m / skip);
		result.matched.resize(m / skip);
		result.proportion_guessed.resize(m / skip);

		// quantile function: map probability p to true symbol
		auto Q = dd.get_inverse_cdf();
		// probability mass function
		auto f = dd.get_pdf();

		for (unsigned int n = 0, k = 0; n < m; ++n)
		{
			// map probability p to the true symbol in {0,...,N-1}
			auto w = Q(entropy.get0_1()) - 1;
			++freq[p2h[w]].second;

			auto hidden_counts = freq;
			std::sort(hidden_counts.begin(), hidden_counts.end(),
				[](const pair<unsigned int, unsigned int>& x1,
					const pair<unsigned int, unsigned int>& x2) -> bool
			{ return x1.second > x2.second; });

			vector<unsigned int> argmax(N);
			for (unsigned int i = 0; i < hidden_counts.size(); ++i)
				argmax[hidden_counts[i].first] = i;

			unsigned int matched = 0;
			alex::math::kbn_sum<long double> pr;
			for (unsigned int i = 0; i < N; ++i)
			{
				if (argmax[i] == h2p[i])
				{
					++matched;
					auto w = h2p[i];
					auto p = f(w + 1);
					pr += p;
				}
			}

			if (p != 0 && n % p == 0)
			{
				result.gain[k] = n * (std::log2(N) - result.entropy);
				result.proportion_guessed[k] = pr();
				result.matched[k] = matched;
				++k;
			}
		}

		return result;
	}
}

#endif