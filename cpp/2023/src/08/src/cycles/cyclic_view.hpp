//
// Created by Conor Pappas on 12/17/23.
//

#pragma once

#include "cyclic_iterator.hpp"


template <typename Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count = 1>
class CyclicView {
    using iterator_traits = std::iterator_traits<Iterator>;

    public:
        CyclicView();
        explicit CyclicView(const Iterator& begin);
        CyclicView(const Iterator& begin, const Iterator& end);

        [[nodiscard]] CyclicIterator<Iterator> begin() const;
        [[nodiscard]] CyclicIterator<Iterator> end() const;

        [[nodiscard]] typename std::iterator_traits<CyclicIterator<Iterator>>::difference_type size() const;

    private:
        Iterator m_underlying_begin {};
        Iterator m_underlying_end {};
};

template<typename Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
CyclicView<Iterator, cycle_count>::CyclicView() = default;

/**
 * \brief NB this constructor is only valid if the given iterator returns back to itself eventually.
 */
template <typename Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
CyclicView<Iterator, cycle_count>::CyclicView(const Iterator& begin):
    m_underlying_begin(begin),
    m_underlying_end(begin) {}

/**
 * \brief Builds a cyclic view over the given iterators. Once end is reached, the view will return back to begin.
 */
template <typename Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
CyclicView<Iterator, cycle_count>::CyclicView(const Iterator& begin, const Iterator& end):
    m_underlying_begin(begin),
    m_underlying_end(end) {}

/**
 * \brief Returns an iterator pointing to the begining of your cycle.
 */
template <typename Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
CyclicIterator<Iterator> CyclicView<Iterator, cycle_count>::begin() const {
    return CyclicIterator(m_underlying_begin, m_underlying_end);
}

/**
 * \brief Returns an iterator referring to the past-the-end element for the requested number of cycles.
 */
template <typename Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
CyclicIterator<Iterator> CyclicView<Iterator, cycle_count>::end() const {
    return CyclicIterator(m_underlying_begin, m_underlying_end, cycle_count);
}

template<typename Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
typename std::iterator_traits<CyclicIterator<Iterator>>::difference_type CyclicView<Iterator, cycle_count>::size() const {
    // return 0;
    return std::distance(begin(), end());
}
    // TODO: std::distance doesn't work here. Why?
//     auto ittr = begin();
//     typename std::iterator_traits<CyclicIterator<Iterator>>::difference_type distance = 0;
//     while(ittr != end() && distance < 200) {
//         ++ittr;
//         ++distance;
//         std::cout << ittr << std::endl;
//     }
//     return distance;
// }

