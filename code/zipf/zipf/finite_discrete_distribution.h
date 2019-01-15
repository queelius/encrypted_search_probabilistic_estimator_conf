#ifndef __FINITE_DISCRETE_DISTRIBUTION_H__
#define __FINITE_DISCRETE_DISTRIBUTION_H__

#include <functional>
#include "kbn_sum.h"
using std::sqrt;
using std::pow;
using std::function;
using std::log2;

namespace alex::stochastic
{
	// A random variable is said to be discrete if the support
	// is finite or countablly infinite. This is the interface
	// for discrete random variables with finite support.
	class finite_discrete_distribution
	{
	public:
		typedef long double prob_t;
		typedef int value_t;
		typedef long double scalar_t;

		// The distribution can be characterized through a function called
		// cumulative distribution function. Let the random variable be
		// denoted by X and the cumulative distribution function denoted by
		// F : [X] -> [0,1]. The probability that X <= x is given by F(x).
		virtual function<prob_t(int)> get_cdf() const = 0;

		// The probability mass function f : [X] -> [0,1] is given by
		//     f(x) = F(x) - F(x-1) if x < supremum of support
		//		    = F(x) if x = 0
		//          = 1 if x >= supremum of support
		//          = 0 if x < infinum of support
		virtual function<prob_t(int)> get_pdf() const = 0;
		virtual function<int(prob_t)> get_inverse_cdf() const = 0;
		virtual int get_max_support() const = 0;

		virtual scalar_t expectation(function<scalar_t(int)> g) const
		{
			auto f = get_pdf();
			alex::math::kbn_sum<scalar_t> sum;
			for (int i = 0; i <= get_max_support(); ++i)
				sum += f(i);
			return sum();
		};

		virtual scalar_t skewness() const
		{
			return expectation([u = mean(), s = sqrt(variance())](int x)
			{
				return pow((x - u) / s, 3.);
			});
		};

		virtual scalar_t kurtosis() const
		{
			return expectation([u = mean(), s = sqrt(variance())](int x)
			{
				return pow((x - u) / s, 4.);
			});
		};

		virtual scalar_t mean() const
		{
			return expectation([](int x)
			{
				return x;
			});
		};

		virtual scalar_t variance() const
		{
			return expectation([u = mean()](int x)
			{
				return (x - u) * (x - u);
			});
		};
	
		virtual scalar_t entropy() const
		{
			return expectation([pdf = get_pdf()](int x)
			{
				return -log2(pdf(x));
			});
		};
	};
}

#endif