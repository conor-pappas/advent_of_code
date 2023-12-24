//
// Created by Conor Pappas on 12/22/23.
//

#pragma once

namespace support {
    template <typename R, typename T>
    concept range_of =
        std::ranges::range<R> &&
        std::convertible_to<std::ranges::range_value_t<R>, T>;

    template <typename R, typename T>
    concept input_range_of =
        range_of<R, T> &&
        std::ranges::input_range<R>;

    template <typename R, typename T>
    concept bidirectional_range_of =
        range_of<R, T> &&
        std::ranges::bidirectional_range<R>;

    template <typename R, typename T>
    concept random_access_range_of =
        range_of<R, T> &&
        std::ranges::random_access_range<R>;

    template <typename I, typename T>
    concept iterator_of =
        std::input_or_output_iterator<I> &&
        std::convertible_to<std::iter_value_t<I>, T>;

    template <typename I, typename T>
    concept input_iterator_of =
        iterator_of<I, T> &&
        std::input_iterator<I>;

    template<typename Itr, typename Range>
    concept range_around = std::bidirectional_iterator<Itr>
        && support::bidirectional_range_of<Range, typename std::iterator_traits<Itr>::value_type>;

    template<typename T>
    using iterator_traits = std::iterator_traits<T>;

    // TODO: missing const_iterator_t and const_sentinel_t in c++23
    template<std::ranges::range Range>
    struct range_traits {
        using iterator_t = std::ranges::iterator_t<Range>;
        using sentinel_t = std::ranges::sentinel_t<Range>;
        using range_size_t = std::ranges::range_size_t<Range>;
        using range_difference_t = std::ranges::range_difference_t<Range>;
        using range_value_t = std::ranges::range_value_t<Range>;
        using range_reference_t = std::ranges::range_reference_t<Range>;
    };
}
