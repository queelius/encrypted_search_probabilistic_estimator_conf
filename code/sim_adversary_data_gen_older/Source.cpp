#include <iostream>
#include <algorithm>
#include <random>
#include <unordered_set>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
#include "DataAnalysis.h"
namespace dd = alex::statistics::distributions::discrete_distribution;

void makeDataCleaned(std::istream& is, std::ostream& os, size_t begin = 0, size_t incr = 1, size_t end = std::numeric_limits<size_t>::max(), const std::set<std::string> industries = {}, const std::set<std::string>& corps = {});
void plotPlainTextVsEncryptedHistograms();

auto ent = alex::statistics::Entropy<>();

std::pair<int, int> estimateN(int smallN, int largeN, double conf);
double likelihood(int N);

std::pair<int, int> estimateN(int smallN, int largeN, double conf)
{
    auto likelihoodOf2OutOf10 = [](int N) -> double
    {
        double logp = 0;
        for (int i = N - 17; i <= N - 10; ++i)
            logp += std::log(i);
        for (int i = N - 9; i <= N; ++i)
            logp -= std::log(i);
        return std::exp(logp);
    };

    std::vector<double> ps(largeN - smallN + 1);
    double total = 0;

    for (int i = smallN; i <= largeN; ++i)
    {
        double p = likelihoodOf2OutOf10(i);
        total += p;
        ps[i - smallN] = p;
    }

    std::pair<double, int> ci;
    int lower = 0;
    while (lower < ps.size() && 0 < conf / 2)
    {
        std::cout << ps[lower] / total << std::endl;
        ++lower;
    } 

    int upper = lower;
    while (upper < ps.size() && ps[upper] / total < 1 - conf / 2)
    {
        ++upper;
    } 


    return std::make_pair(lower, upper);
}


//std::vector<double> simulate(int N)
//{
//    const size_t TRIALS = 1000;
//    const size_t SAMPLE_SIZE = 10;
//    const size_t MAX_TAGGED = 10;
//
//    for (int j = 0; j < TRIALS; ++j)
//    {
//        size_t foundTagged = 0;
//        double n = N;
//        double logp = 0;
//
//        for (int i = 0; i < SAMPLE_SIZE; ++i)
//        {
//            double pTagged = (MAX_TAGGED - foundTagged) / n;
//            if (ent.get0_1() <= pTagged)
//            {
//                ++foundTagged;
//                logp += std::log(pTagged);
//            }
//            else
//            {
//                logp += std::log(1 - pTagged);
//            }
//            --n;
//        }
//    }
//}


int main(int ac, char* av[])
{
    //auto ci = estimateN(18, 1000, 0.05);
    //std::cout << ci.first << ", " << ci.second << std::endl;

    //auto x = alex::compression::HuffmanCharacterCode("Now is the time for all the good men to come to the aid of their country. To be, or not to be, that is the question. How many paths must a man walk?");
    //std::cout << x.toStringCodeMap() << std::endl;


    //auto pStars = { 0.15, 0.175, 0.2, 0.225, 0.25, 0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5 };
    //generate_N_star(std::ifstream("data2"), std::ofstream("data2_grouped_by_pstar", std::ios::app), pStars);

    //outputVariance(std::ifstream("data_grouped_by_pstar"), std::ofstream("data_pstar_vs_nstar"));

    auto recs = generate_N_star_stage_2(std::ifstream("data_grouped_by_pstar"));
    generate_n_star_analysis_8(recs);
    //generate_n_star_analysis_6(recs, std::ofstream("all_pstars"));


    //generate_n_star_analysis_7(recs, std::ofstream("pop_data"));
    generate_n_star_analysis_8(recs);

    // makeDataCleaned(std::ifstream("data2"), std::ofstream("data2_cleaned"), 0, 60, 300);
    /*makeDataCleaned(std::ifstream("data2"), std::ofstream("gov_nsa"), 0, 1, 10000000, {"gov"}, {"NSA"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("gov_irs"), 0, 1, 10000000, {"gov"}, {"IRS"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("gov_darpa"), 0, 1, 10000000, {"gov"}, {"DARPA"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("gov_nasa"), 0, 1, 10000000, {"gov"}, {"NASA"});

    makeDataCleaned(std::ifstream("data2"), std::ofstream("health_1"), 0, 1, 10000000, {"health"}, {"health-1"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("health_2"), 0, 1, 10000000, {"health"}, {"health-2"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("health_3"), 0, 1, 10000000, {"health"}, {"health-3"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("health_4"), 0, 1, 10000000, {"health"}, {"health-4"});

    makeDataCleaned(std::ifstream("data2"), std::ofstream("it_apple"), 0, 1, 10000000, {"it"}, {"Apple"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("it_microsoft"), 0, 1, 10000000, {"it"}, {"Microsoft"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("it_google"), 0, 1, 10000000, {"it"}, {"Google"});
    makeDataCleaned(std::ifstream("data2"), std::ofstream("it_bethesda"), 0, 1, 10000000, {"it"}, {"Bethesda"});*/
    
    return 0;
}

void makeDataCleaned(std::istream& is, std::ostream& os, size_t begin, size_t incr, size_t end, const std::set<std::string> industries, const std::set<std::string>& corps)
{
    size_t i, corpSize, n;
    std::string industry, corp, line;
    double maxP, p;

    for (i = 0; i < begin; ++i)
    {
        is.ignore('\n');
    }

    while (is && i <= end)
    {
        is >> industry >> corp >> corpSize >> maxP;

        getline(is, line);
        if (!industries.empty() && industries.count(industry) == 0)
        {
            std::cout << i << " -> skipping industry: " << industry << std::endl;
        }
        else if (!corps.empty() && corps.count(corp) == 0)
        {
            std::cout << i << " -> skipping corp: " << corp << std::endl;
        }
        else
        {
            std::cout << i << " -> outputting: " << industry << "\t" << corp << "\t" << std::endl;
            std::stringstream ss;
            ss << line;
            while (ss)
            {
                ss >> p >> n;
                os << p << "\t" << n << std::endl;
            }
            os << std::endl << std::endl;
        }

        for (size_t k = 0; k < incr - 1; ++k)
        {
            ++i;
            getline(is, line);
            std::cout << i << " -> incrementing over: " << line.substr(0, 20) << std::endl;
        }
        ++i;
    }
}

void plotPlainTextVsEncryptedHistograms()
{
    const int SAMPLE_SIZE = 400;
    const int DICTIONARY_SIZE = 20;

    auto zipf = alex::statistics::distributions::discrete_distribution::zipf(DICTIONARY_SIZE);
    auto plainSample = std::vector<int>();

    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        plainSample.push_back(zipf.inverseCdf(ent.get0_1()));
    }

    auto pdf = alex::statistics::distributions::discrete_distribution::fromFrequency(plainSample.begin(), plainSample.end());
    const size_t N = 1200;
    std::vector<int> hiddenSample; hiddenSample.reserve(N);
    for (int i = 0; i < N; ++i)
    {
        hiddenSample.push_back(pdf.inverseCdf(ent.get0_1()));
    }

    struct Bin
    {
        int x;
        int freq;
        std::string lbl;
        std::string word;
    };

    std::vector<Bin> pBins(DICTIONARY_SIZE);
    std::vector<Bin> hBins(DICTIONARY_SIZE);

    for (int i = 0; i < DICTIONARY_SIZE; ++i)
    {
        pBins[i].freq = 0;
        hBins[i].freq = 0;
    }
    
    for (int i = 0; i < SAMPLE_SIZE; ++i)
    {
        pBins[plainSample[i]].x = plainSample[i];
        pBins[plainSample[i]].freq++;
    }

    for (int i = 0; i < N; ++i)
    {
        hBins[hiddenSample[i]].x = hiddenSample[i];
        hBins[hiddenSample[i]].freq++;
    }

    std::sort(pBins.begin(), pBins.end(), [](Bin x1, Bin x2) { return x1.freq > x2.freq; });
    std::sort(hBins.begin(), hBins.end(), [](Bin x1, Bin x2) { return x1.freq > x2.freq; });

    auto wordVec = std::vector<std::string>{"the","be","to","of","and","a","in","that","have","i","it","for","not","on","with","he","as","you","do","at"};

    for (int i = 0; i < DICTIONARY_SIZE; ++i)
    {
        for (int j = 0; j < 3; ++j)
            hBins[i].word += (char)ent.getInt(97, 122);
        pBins[i].word = wordVec[i];

        if (pBins[i].x == hBins[i].x)
        {
            pBins[i].lbl = hBins[i].lbl = "G";
        }
        else
        {
            pBins[i].lbl = hBins[i].lbl = "B";
        }
    }

    auto outf = std::ofstream("compare_zipf_encrypted_versus_plain.dat");

    for (int i = 0; i < DICTIONARY_SIZE; ++i)
    {
        auto p = pBins[i];
        outf << p.word << "\t" << p.freq << "\t" << p.lbl << std::endl;
    }

    outf << "\n\n";
    for (int i = 0; i < DICTIONARY_SIZE; ++i)
    {
        auto p = hBins[i];
        outf << p.word << "\t" << p.freq << "\t" << p.lbl << std::endl;
    }
}

