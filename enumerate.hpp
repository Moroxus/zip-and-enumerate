
#ifndef ENUMERATE_HPP
#define ENUMERATE_HPP

#include <iterator>
#include <tuple>
#include <optional>
#include <utility>
#include <type_traits>
#include <functional>
#include <algorithm>

#include "utility.hpp"

template <typename... T>
struct enumerate_tuple : std::tuple<T...> {
    using base = std::tuple<T...>;
    using base::base;

    template <typename... U,
              std::enable_if_t<(std::is_constructible_v<T, U&&> &&...), int> = 0>
    enumerate_tuple(std::tuple<U...>&& rhs):base(std::move(rhs)) {}

    template <std::size_t I>
    auto& get() & {
        return std::get<I>(*this);
    }

    template <std::size_t I>
    auto& get() const & {
        return std::get<I>(*this);
    }

    template <std::size_t I>
    auto get() && {
        return std::get<I>(*this);
    }

    template <std::size_t I>
    auto get() const && {
        return std::get<I>(*this);
    }
};

namespace std {
    template <typename... T>
    struct tuple_size<enumerate_tuple<T...>>
        : tuple_size<tuple<T...>>
    { };

    template <size_t I, typename... T>
    struct tuple_element<I, enumerate_tuple<T...>>
        : tuple_element<I, tuple<remove_reference_t<T>...>>
    { };
}

template <typename... T>
class enumerate_helper {

public:
    class iterator :
            std::iterator <std::forward_iterator_tag,
                           enumerate_tuple <const std::size_t, decltype(*begin_iterator(std::declval<T>()))...>> {
    private:
        std::size_t current_;
        std::tuple<decltype(begin_iterator(std::declval<T>()))...> iterators_;
        mutable std::optional<typename iterator::value_type> value_;

        template <std::size_t... I>
        typename iterator::value_type deref(std::index_sequence<I...>) const {
            return enumerate_tuple<const std::size_t, decltype (*std::get<I>(iterators_))...>(std::tuple(current_, std::ref(*std::get<I>(iterators_))...));
        }

        template <std::size_t... I>
        void increment(std::index_sequence<I...>) {
            current_++;
            (++std::get<I>(iterators_), ...);
        }

        template <typename Tuple, std::size_t... I>
        bool any_equals(const Tuple& lhs, const Tuple& rhs, std::index_sequence<I...>) const {
            return ((std::get<I>(lhs.iterators_) == std::get<I>(rhs.iterators_)) || ...);
        }

    public:
        explicit iterator(std::size_t current, decltype (iterators_) iterators):current_(current), iterators_(std::move(iterators)) {}

        iterator& operator++() {
            increment(std::index_sequence_for<T...>{});
            return *this;
        }

        iterator operator++(int) {
            auto saved(*this);
            increment(std::index_sequence_for<T...>{});
            return saved;
        }

        bool operator == (const iterator& other) const {
            return any_equals(*this, other, std::index_sequence_for<T...>{});
        }

        bool operator != (const iterator& other) const {
            return !(*this == other);
        }

        typename iterator::reference operator*() const {
            value_.emplace(deref(std::index_sequence_for<T...>{}));
            return *value_;
        }
    };

    template <typename... U>
    explicit enumerate_helper(U&&... containers):containers_{std::forward<U>(containers)...},
                                                 begin_{enumerate_helper::make_begin(std::index_sequence_for<U...>{}, containers_)},
                                                 end_{enumerate_helper::make_end(std::index_sequence_for<U...>{}, containers_)} {}

    iterator& begin() { return begin_; }
    iterator& end() {return end_; }

private:

    template <std::size_t... Indices>
    static iterator make_begin(std::index_sequence<Indices...>, std::tuple<T...>& containers) {
        return iterator(0, enumerate_tuple<decltype (std::get<Indices>(containers).begin())...>(std::make_tuple(std::get<Indices>(containers).begin()...)));
    }

    template <std::size_t... Indices>
    static iterator make_end(std::index_sequence<Indices...>, std::tuple<T...>& containers) {
        return iterator(std::min({std::get<Indices>(containers).size()...}),
                        enumerate_tuple<decltype (std::get<Indices>(containers).end())...>
                            (std::make_tuple(std::get<Indices>(containers).end()...)));
    }

private:
    std::tuple<T...> containers_;
    iterator begin_;
    iterator end_;
};

template <typename... T>
auto enumerate(T&&... containers) {
    return enumerate_helper<T...>{std::forward<T>(containers)...};
}


#endif // ENUMERATE_HPP
