#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <string>
#include <sstream>
#include <vector>
#include "RandomVariates.h"
#include "Bootstrap.h"
#include "EmpiricalDistribution.h"
#include "BarGraph.h"
#include "Entropy.h"
#include "StatTools.h"
#include "DiscreteDistribution.h"
#include "Histogram.h"

namespace dd = alex::statistics::distributions::discrete_distribution;
alex::statistics::Entropy<> entropy;
dd::DiscreteDistribution<double> make_distribution(int n, int m);
std::vector<std::pair<double, int>> simulate_adversary(int n, int m, int min_history, int max_history, int inc_history, double alpha);

dd::DiscreteDistribution<double> make_distribution(int n, int m)
{
    auto pdf = dd::zipf(n);
    return pdf;
    std::vector<double> samples; samples.reserve(m);
    for (int i = 0; i < m; ++i)
        samples.push_back(pdf.inverseCdf(entropy.get0_1()));

    return dd::fromFrequency(samples.begin(), samples.end());
};

std::vector<std::pair<double, int>> simulate_adversary(int n, int m, int min_history, int max_history, int inc_history, double alpha)
{
    auto pdf = make_distribution(n, m);

    std::vector<int> h2p;
    std::vector<int> p2h(n);

    h2p.reserve(n);
    for (int i = 0; i < n; ++i)
        h2p.push_back(i);

    std::shuffle(h2p.begin(), h2p.end(), entropy.engine());
    
    std::vector<std::pair<int, int>> freq;
    for (int h = 0; h < n; ++h)
    {
        int w = h2p[h];
        p2h[w] = h;
        freq.push_back(std::make_pair(h, 0));
    }

    std::vector<double> log_prob = pdf.toPdfVector();
    for (double& p : log_prob)
        p = std::log(p);      

    std::vector<std::pair<double, int>> sample;
    int history_size = 0;
    double maxPr = 0;
    size_t failed = 0;
    for (int history = min_history; history <= max_history; history += inc_history)
    {
        for (int i = 0; i < inc_history; ++i)
        {
            int w = pdf.inverseCdf(entropy.get0_1());
            ++freq[p2h[w]].second;
        }

        history_size += inc_history;

        auto hidden_counts = freq;
        std::sort(hidden_counts.begin(), hidden_counts.end(),
                  [](const std::pair<int, int>& x1, const std::pair<int, int>& x2) -> bool { return x1.second > x2.second; });

        std::vector<int> argmax(n);
        for (int i = 0; i < hidden_counts.size(); ++i)
            argmax[hidden_counts[i].first] = i;

        double pr = 0;
        for (int i = 0; i < n; ++i)
        {
            if (h2p[i] == argmax[i])
                pr += pdf.pdf(h2p[i]);
        }

        sample.push_back(std::make_pair(pr, history));

        if (pr > maxPr)
        {
            maxPr = pr;
            failed = 0;
        }
        else
            ++failed;

        if (pr > alpha || failed > 100000)
            break;
    }

    return sample;
}
