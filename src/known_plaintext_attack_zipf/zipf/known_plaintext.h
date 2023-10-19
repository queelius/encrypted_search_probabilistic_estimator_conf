#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>
#include "zipf_distribution.h"
#include "entropy.h"

using std::pair;
using std::vector;

namespace alex::simulation
{
    struct KnownPlainTextAttackResult
    {
        vector<long double> proportion_guessed;
        vector<unsigned int> matched;
        long double entropy;                    // entropy of distribution
        double s;                               // Zipf parameter; determines entropy, smaller s -> larger entropy
        unsigned int N;                         // # unique words
        unsigned int m;                         // hidden query sample size

    };

    template <typename E>
    KnownPlainTextAttackResult
    known_plaintext_attack(
        unsigned int N,                         // Zipf parameter; # unique words
        double s,                               // Zipf parameter; determines entropy, smaller s -> larger entropy
        unsigned int m,                         // sample size
        alex::stochastic::Entropy<E>& entropy)
    {
        alex::stats::ZipfDistribution zipf(N, s);

        vector<unsigned int> h2p(N);
        vector<unsigned int> p2h(N);

        for (unsigned int i = 0; i < N; ++i)
            h2p[i] = i;

        std::shuffle(h2p.begin(), h2p.end(), entropy.engine());
        vector<pair<unsigned int, unsigned int>> freq;
        for (unsigned int h = 0; h < N; ++h)
        {
            auto w = h2p[h];
            p2h[w] = h;
            freq.push_back(std::make_pair(h, 0));
        }

        KnownPlainTextAttackResult result;
        result.entropy = zipf.entropy();
        result.m = m;
        result.s = s;
        result.N = N;
        result.matched.resize(m);
        result.proportion_guessed.resize(m);

        auto Q = zipf.get_inverse_cdf();
        auto f = zipf.get_pdf();

        for (unsigned int n = 0; n < m; ++n)
        {
            //auto w = zipf.inverse_cdf(entropy.get0_1()) - 1;
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
                    //auto p = zipf.pdf(w + 1);
                    auto p = f(w + 1);
                    pr += p;
                }
            }

            result.proportion_guessed[n] = pr();
            result.matched[n] = matched;
        }

        return result;
    }
}