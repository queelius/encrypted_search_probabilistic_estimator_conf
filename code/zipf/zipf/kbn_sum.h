#ifndef __KBN_SUM_H__
#define __KBN_SUM_H__

#include <algorithm>

namespace alex::math
{
	// kbn_sum<T> implements the SumAccumulator<T> ADT.
	//
	// Given a data structure X that implements the SumAccumulator<T> ADT,
	// the following operations for X are available:

	// (1) X::X()
	//
	//     A constructor. That is, objects of type X
	//     must be able to be constructed.
	//
	// (2) X::approximately_equal(const T&) -> bool
	//
	//     
	//     
	// (2) X::accumulate(const T&) -> T
	//
	//     The accumlation operator.
	//
	// (4)  
	// (5) template <class Iterator> X::accumulate(Iterator, Iterator) -> T
	// (6)  template <class Iterable> X::accumulate(Iterable) -> T
	// (7)   X::operator T() const -> T
	// (8)  X::operator=(const X&) -> X&
	//
	// (1) if the assignment S = x is invoked, then
	//     static_cast<T>(S) approximately equals x.
	// (2) suppose static_cast<T>(S) == s. if the
	//     summation operator S += x is invoked,
	//     then static_cast<T>(S) approximately
	//     equals s + x.
	// (3) suppose static_cast<T>(S) == s. if the
	//     summation operator S + x is invoked,
	//     then static_cast<T>(S) approximately
	//     equals s + x.

    template <typename T>
    class kbn_sum
    {
    public:
		kbn_sum(const T& x = 0) :
			_s(static_cast<T>(0)), _c(static_cast<T>(0))
		{
			add(x);
		};

        template <typename Iter>
        explicit kbn_sum(Iter begin, Iter end) :
			_s(static_cast<T>(0)), _c(static_cast<T>(0))
        {
            add(begin, end);
        };

        kbn_sum(const kbn_sum<T>& copy)
			: _s(copy._s), _c(copy._c)
		{
			// do nothing else
		};

		operator T() const
		{
			return _s + _c;
		};

        T operator()() const
        {
            return _s + _c;
		};

        kbn_sum<T>& operator+=(const kbn_sum<T>& other)
        {
			add(other._s);
			add(other._c);
            return *this;
		};

        kbn_sum<T>& operator+=(T v)
        {
            add(v);
            return *this;
		};

        kbn_sum<T> operator+(const kbn_sum<T>& other)
        {
            auto s = *this;
            s += other._s;
            s += other._c;
            return s;
		};

        kbn_sum<T>& operator=(const kbn_sum<T>& other)
        {
            _s = other._s;
            _c = other._c;
            return *this;
		};

        T sum(T v)
        {
            auto t = _s + v;
            if (std::abs(_s) >= std::abs(v))
                _c += ((_s - t) + v);
            else
                _c += ((v - t) + _s);
            _s = t;

			return _s + _c;
		};

        template <typename Iter>
        T sum(Iter begin, Iter end)
        {
            T t;
            for (auto i = begin; i != end; ++i)
            {
                t = _s + *i;
                if (std::abs(_s) >= std::abs(*i))
                   _c += ((_s - t) + *i);
                else
                   _c += ((*i - t) + _s);
                _s = t;
            }
			return _s + _c;
		};

    private:
        T _s, _c;
    };
}

#endif