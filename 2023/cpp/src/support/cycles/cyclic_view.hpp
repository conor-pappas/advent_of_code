//
// Created by Conor Pappas on 12/17/23.
//

#pragma once

#include "cyclic_iterator.hpp"

namespace support {
    template <std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count = 0>
    class CyclicView: std::ranges::view_interface<CyclicView<Iterator, cycle_count>> {
    public:
        using iterator = CyclicIterator<Iterator>;
        using difference_type = typename std::iterator_traits<iterator>::difference_type;

        CyclicView();
        explicit CyclicView(const Iterator& begin);
        CyclicView(const Iterator& begin, const Iterator& end);

        [[nodiscard]] iterator begin() const;
        [[nodiscard]] iterator end() const requires (cycle_count != 0);
        [[nodiscard]] iterator::InfiniteSentinel end() const requires (cycle_count == 0);

        [[nodiscard]] difference_type size() const;

        template<typename CyclicIterator<Iterator>::CycleCountType new_cycle_count>
        explicit operator CyclicView<Iterator, new_cycle_count>() const;

    private:
        Iterator m_underlying_begin {};
        Iterator m_underlying_end {};
    };

    template<std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    CyclicView<Iterator, cycle_count>::CyclicView() = default;

    /**
     * \brief NB this constructor is only valid if the given iterator returns back to itself eventually.
     */
    template <std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    CyclicView<Iterator, cycle_count>::CyclicView(const Iterator& begin):
        m_underlying_begin(begin),
        m_underlying_end(begin) {}

    /**
     * \brief Builds a cyclic view over the given iterators. Once end is reached, the view will return back to begin.
     */
    template <std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    CyclicView<Iterator, cycle_count>::CyclicView(const Iterator& begin, const Iterator& end):
        m_underlying_begin(begin),
        m_underlying_end(end) {}

    /**
     * \brief Returns an iterator pointing to the begining of your cycle.
     */
    template <std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    CyclicIterator<Iterator> CyclicView<Iterator, cycle_count>::begin() const {
        return CyclicIterator(m_underlying_begin, m_underlying_end);
    }

    /**
     * \brief Returns an iterator referring to the past-the-end element for the requested number of cycles.
     */
    template <std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    CyclicIterator<Iterator> CyclicView<Iterator, cycle_count>::end() const
    requires (cycle_count != 0) {
        return CyclicIterator(m_underlying_begin, m_underlying_end, cycle_count);
    }

    template <std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    typename CyclicIterator<Iterator>::InfiniteSentinel CyclicView<Iterator, cycle_count>::end() const
    requires (cycle_count == 0) {
        return iterator::InfiniteSentinel();
    }

    template<std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    typename CyclicView<Iterator, cycle_count>::difference_type CyclicView<Iterator, cycle_count>::size() const {
        return std::distance(begin(), end());
    }

    template<std::forward_iterator Iterator, typename CyclicIterator<Iterator>::CycleCountType cycle_count>
    template<typename CyclicIterator<Iterator>::CycleCountType new_cycle_count>
    CyclicView<Iterator, cycle_count>::operator CyclicView<Iterator, new_cycle_count>() const {
        return CyclicView<Iterator, new_cycle_count>(m_underlying_begin, m_underlying_end);
    }
}

