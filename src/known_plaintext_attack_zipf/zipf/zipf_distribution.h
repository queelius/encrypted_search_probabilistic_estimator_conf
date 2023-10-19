#ifndef __ZIPF_DISTRIBUTION_H__
#define __ZIPF_DISTRIBUTION_H__

#include "kbn_sum.h"
#include "finite_discrete_distribution.h"
#include <functional>
using std::function;

namespace alex::stochastic
{

    class zipf_distribution: public finite_discrete_distribution
    {
    public:
		using finite_discrete_distribution::prob_t;
		using finite_discrete_distribution::value_t;
		using finite_discrete_distribution::scalar_t;

		// N: number of unique words
		// s: shape of distribution.
		//     note: smaller s, larger entropy (flatter).
		//           s = 0 <=> uniform distribution
		//           s -> infinity <=> degenerate distribution
        zipf_distribution(value_t N, scalar_t s) :
            N(N), s(s)
        {
        };

        function<prob_t(value_t)> get_cdf() const
        {
            return [N = N, s = s,  Hns = harmonic(N, s)](value_t k) -> prob_t
            {
                if (k == 0)
                    return 0;
                if (k >= N)
                    return 1;
                return zipf_distribution::harmonic(k, s) / Hns;
            };
        };

        function<prob_t(value_t)> get_pdf() const
        {
            return [N = N, s = s, Hns = harmonic(N, s)](value_t k)
            {
                if (k == 0 || k > N)
                    return 0.0L;
                return static_cast<value_t>(1) / (std::pow(k, s) * Hns);
            };
        };

        function<value_t(prob_t)> get_inverse_cdf() const
        {
            return [N = N, s = s, d = 1 / harmonic(N, s)](prob_t p)
            {
                alex::math::kbn_sum<prob_t> sum;
                for (value_t k = 1; k < N; ++k)
                {
                    sum += std::pow(k, -s) * d;
                    if (p <= sum())
                        return k;
                }
                return N;
            };
        };

		scalar_t expectation(std::function<scalar_t(value_t)> g) const
		{
			alex::math::kbn_sum<scalar_t> sum;
			for (value_t k = N; k >= static_cast<value_t>(1); --k)
			{
				sum += g(k) / std::pow(k, s);
			}
			return sum() / harmonic(N, s);
		};

		static scalar_t harmonic(value_t N, scalar_t s)
		{
			alex::math::kbn_sum<scalar_t> sum;
			for (value_t n = N; n != 0u; --n)
			{
				sum += 1. / std::pow(n, s);
			}
			return sum();
		};

    private:
        value_t N;
        long double s;
    };
}

#endif