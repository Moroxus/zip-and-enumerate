#ifndef UTILITY_HPP
#define UTILITY_HPP

template <typename Range>
auto begin_iterator(Range &&rng)
{
    return rng.begin();
}

template <typename Range>
auto end_iterator(Range &&rng)
{
    return rng.end();
}

#endif // UTILITY_HPP
