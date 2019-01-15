#ifndef __ENTROPY_H__
#define __ENTROPY_H__

#include <random>

namespace alex::stochastic
{
    template <class E = std::mt19937>
    class Entropy
    {
    public:
        Entropy(unsigned long seed_value = 0)
        {
            seed(seed_value);
        };

        Entropy(Entropy&& src) :
            _eng(std::move(src._eng)),
            _rnd_device(std::move(src._rnd_device))
        {
        };

        Entropy& operator=(Entropy&& src)
        {
            if (this != &other)
                _eng = std::move(src._eng);
            return *this;
        };

        void seed(unsigned long seed_value = 0)
        {
            _eng.seed(seed_value);
        };

        bool flip()
        {
            return get() % 2 == 0;
        };

        unsigned int get()
        {
            return _eng();
        };

        int get_int(int min, int max)
        {
            return static_cast<int>(min + get() % (max - min + 1));
        };

        double get(double min, double max)
        {
            return min + get0_1() * (max - min);
        };

        double get0_1()
        {
            return static_cast<double>(get()) / _eng.max();
        };

        E& engine()
        {
            return _eng;
        };

    private:
        Entropy(const Entropy&) = delete;
        Entropy& operator=(const Entropy&) = delete;

        E _eng;
    };
}

#endif