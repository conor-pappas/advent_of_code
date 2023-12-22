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

}
