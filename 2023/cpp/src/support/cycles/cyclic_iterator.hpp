//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

#include <ostream>

namespace support {
    template <typename Iterator>
    class CyclicIterator {
    public:
        using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
        using difference_type   = typename std::iterator_traits<Iterator>::difference_type;
        using value_type        = typename std::iterator_traits<Iterator>::value_type;
        using pointer           = typename std::iterator_traits<Iterator>::pointer;
        using reference         = typename std::iterator_traits<Iterator>::reference;

        using CycleCountType = long;

        CyclicIterator();
        CyclicIterator(const Iterator& begin, const Iterator& end);
        CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor);
        CyclicIterator(const Iterator& begin, const Iterator& end, const CycleCountType&);
        CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor, const CycleCountType&);

        [[nodiscard]] CycleCountType get_cycle_count() const;
        void set_cycle_count(CycleCountType);

        void inc_cycle_count();
        void dec_cycle_count();

        bool mod_equals(const CyclicIterator&) const;
        bool mod_not_equals(const CyclicIterator&) const;

        bool operator==(const CyclicIterator& other) const;
        bool operator!=(const CyclicIterator& other) const;

        reference operator*() const;
        pointer operator->() const;

        CyclicIterator& operator++();
        CyclicIterator operator++(int) &;
        // TODO: UB if the input iterator is not bidirectional. Should proly take a template param
        CyclicIterator& operator--();
        CyclicIterator operator--(int) &;

        friend std::ostream& operator<<(std::ostream& os, const CyclicIterator& cyclic_iterator) {
            os << "CyclicIterator(" << cyclic_iterator.m_cursor << ", " << cyclic_iterator.m_cycle_count << ")";
            return os;
        }

    private:
        Iterator m_begin {};
        Iterator m_end {};
        Iterator m_cursor {};
        CycleCountType m_cycle_count = 0;
    };

    template<typename Iterator>
    CyclicIterator<Iterator>::CyclicIterator() = default;

    template<typename Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end):
        m_begin(begin),
        m_end(end),
        m_cursor(begin) {}

    template<typename Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor):
        m_begin(begin),
        m_end(end),
        m_cursor(cursor) {}

    template<typename Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end, const CycleCountType& cycle_count):
        m_begin(begin),
        m_end(end),
        m_cursor(begin),
        m_cycle_count(cycle_count) {}

    template<typename Iterator>
    CyclicIterator<Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor, const CycleCountType& cycle_count):
        m_begin(begin),
        m_end(end),
        m_cursor(cursor),
        m_cycle_count(cycle_count) {}

    template<typename Iterator>
    typename CyclicIterator<Iterator>::CycleCountType CyclicIterator<Iterator>::get_cycle_count() const {
        return m_cycle_count;
    }

    template<typename Iterator>
    void CyclicIterator<Iterator>::set_cycle_count(CycleCountType cycle_count) {
        this->m_cycle_count = cycle_count;
    }

    template<typename Iterator>
    void CyclicIterator<Iterator>::inc_cycle_count() {
        ++m_cycle_count;
    }

    template<typename Iterator>
    void CyclicIterator<Iterator>::dec_cycle_count() {
        --m_cycle_count;
    }

    template<typename Iterator>
    bool CyclicIterator<Iterator>::mod_equals(const CyclicIterator& other) const {
        return m_begin == other.m_begin
            && m_end == other.m_end
            && m_cursor == other.m_cursor;
    }

    template<typename Iterator>
    bool CyclicIterator<Iterator>::mod_not_equals(const CyclicIterator& other) const {
        return !mod_equals(other);
    }

    template<typename Iterator>
    bool CyclicIterator<Iterator>::operator==(const CyclicIterator& other) const {
        return mod_equals(other) && m_cycle_count == other.m_cycle_count;
    }

    template<typename Iterator>
    bool CyclicIterator<Iterator>::operator!=(const CyclicIterator& other) const {
        return !(*this == other);
    }

    template<typename Iterator>
    typename CyclicIterator<Iterator>::reference CyclicIterator<Iterator>::operator*() const {
        return *m_cursor;
    }

    template<typename Iterator>
    typename CyclicIterator<Iterator>::pointer CyclicIterator<Iterator>::operator->() const {
        return m_cursor;
    }

    template<typename Iterator>
    CyclicIterator<Iterator>& CyclicIterator<Iterator>::operator++() {
        ++m_cursor;
        if (m_cursor == m_end) {
            m_cursor = m_begin;
            ++m_cycle_count;
        }
        return *this;
    }

    template<typename Iterator>
    CyclicIterator<Iterator> CyclicIterator<Iterator>::operator++(int) & {
        CyclicIterator result = *this;
        ++*this;
        return result;
    }

    template<typename Iterator>
    CyclicIterator<Iterator>& CyclicIterator<Iterator>::operator--() {
        if (m_cursor == m_begin) {
            m_cursor = m_end;
            --m_cycle_count;
        }
        --m_cursor;
        return *this;
    }

    template<typename Iterator>
    CyclicIterator<Iterator> CyclicIterator<Iterator>::operator--(int) & {
        CyclicIterator result = *this;
        --*this;
        return result;
    }
};
