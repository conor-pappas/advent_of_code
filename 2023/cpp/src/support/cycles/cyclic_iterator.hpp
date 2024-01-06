//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

#include "cyclic_iterator_fwd.hpp"

namespace support {
    template <std::forward_iterator Iterator>
    class CyclicIterator {
    public:
        // TODO: this is false, we need to conditionally implement the remaining iterator operators. (RAI and Continuous)
        // In fact, we'd likely change the data stored to just be cycle + offset for random_access_iterators.
        // This class should be implemented entirely differently for them.
        using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
        using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
        using value_type        = typename std::iterator_traits<Iterator>::value_type;
        using pointer           = typename std::iterator_traits<Iterator>::pointer;
        using reference         = typename std::iterator_traits<Iterator>::reference;

        using CycleCountType = long;
        struct InfiniteSentinel {};
        struct cycle {
            CycleCountType count { 1 };
        };

        CyclicIterator();
        CyclicIterator(const Iterator& begin, const Iterator& end);
        CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor);
        CyclicIterator(const Iterator& begin, const Iterator& end, const CycleCountType&);
        CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor, const CycleCountType&);

        [[nodiscard]] CycleCountType get_cycle_count() const;
        void set_cycle_count(CycleCountType);

        // TODO: we should have a "cycle" static and allow adding and subtracting cycles with +/- operators.
        void inc_cycle_count();
        void dec_cycle_count();

        bool mod_equals(const CyclicIterator&) const;
        bool mod_not_equals(const CyclicIterator&) const;

        friend bool operator==<Iterator>(const CyclicIterator&, const CyclicIterator&);
        friend bool operator==<Iterator>(CyclicIterator, InfiniteSentinel);

        reference operator*() const;
        pointer operator->() const;

        CyclicIterator& operator++();
        CyclicIterator operator++(int) &;
        CyclicIterator& operator--() requires std::bidirectional_iterator<Iterator>;
        CyclicIterator operator--(int)& requires std::bidirectional_iterator<Iterator>;

        CyclicIterator& operator+=(cycle);
        CyclicIterator& operator-=(cycle);
    private:
        Iterator m_begin {};
        Iterator m_end {};
        Iterator m_cursor {};
        CycleCountType m_cycle_count = 0;
    };

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>::CyclicIterator() = default;

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end):
        m_begin(begin),
        m_end(end),
        m_cursor(begin) {}

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor):
        m_begin(begin),
        m_end(end),
        m_cursor(cursor) {}

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end, const CycleCountType& cycle_count):
        m_begin(begin),
        m_end(end),
        m_cursor(begin),
        m_cycle_count(cycle_count) {}

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor, const CycleCountType& cycle_count):
        m_begin(begin),
        m_end(end),
        m_cursor(cursor),
        m_cycle_count(cycle_count) {}

    template<std::forward_iterator Iterator>
    typename CyclicIterator<Iterator>::CycleCountType CyclicIterator<Iterator>::get_cycle_count() const {
        return m_cycle_count;
    }

    template<std::forward_iterator Iterator>
    void CyclicIterator<Iterator>::set_cycle_count(CycleCountType cycle_count) {
        this->m_cycle_count = cycle_count;
    }

    template<std::forward_iterator Iterator>
    void CyclicIterator<Iterator>::inc_cycle_count() {
        ++m_cycle_count;
    }

    template<std::forward_iterator Iterator>
    void CyclicIterator<Iterator>::dec_cycle_count() {
        --m_cycle_count;
    }

    template<std::forward_iterator Iterator>
    bool CyclicIterator<Iterator>::mod_equals(const CyclicIterator& other) const {
        return m_begin == other.m_begin
            && m_end == other.m_end
            && m_cursor == other.m_cursor;
    }

    template<std::forward_iterator Iterator>
    bool CyclicIterator<Iterator>::mod_not_equals(const CyclicIterator& other) const {
        return !mod_equals(other);
    }

    template<std::forward_iterator Iterator>
    typename CyclicIterator<Iterator>::reference CyclicIterator<Iterator>::operator*() const {
        return *m_cursor;
    }

    template<std::forward_iterator Iterator>
    typename CyclicIterator<Iterator>::pointer CyclicIterator<Iterator>::operator->() const {
        return m_cursor;
    }

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>& CyclicIterator<Iterator>::operator++() {
        ++m_cursor;
        if (m_cursor == m_end) {
            m_cursor = m_begin;
            ++m_cycle_count;
        }
        return *this;
    }

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator> CyclicIterator<Iterator>::operator++(int) & {
        CyclicIterator result = *this;
        ++*this;
        return result;
    }

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>& CyclicIterator<Iterator>::operator--()
        requires std::bidirectional_iterator<Iterator> {
        if (m_cursor == m_begin) {
            m_cursor = m_end;
            --m_cycle_count;
        }
        --m_cursor;
        return *this;
    }

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator> CyclicIterator<Iterator>::operator--(int)&
        requires std::bidirectional_iterator<Iterator> {
        CyclicIterator result = *this;
        --*this;
        return result;
    }

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>& CyclicIterator<Iterator>::operator+=(const cycle c) {
        m_cycle_count += c.count;
        return this;
    }

    template<std::forward_iterator Iterator>
    CyclicIterator<Iterator>& CyclicIterator<Iterator>::operator-=(const cycle c) {
        m_cycle_count -= c.count;
        return this;
    }

    template<std::forward_iterator Iterator>
    bool operator==(const CyclicIterator<Iterator>& lhs, const CyclicIterator<Iterator>& rhs) {
        return lhs.mod_equals(rhs) && lhs.m_cycle_count == rhs.m_cycle_count;
    }

    template<std::forward_iterator Iterator>
    bool operator==(CyclicIterator<Iterator>, typename CyclicIterator<Iterator>::InfiniteSentinel) {
        return false;
    }
};
