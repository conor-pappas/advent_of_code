//
// Created by Conor Pappas on 12/15/23.
//

#pragma once

template <typename T, typename Iterator>
class CyclicIterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type   = size_t;
    using value_type        = T;
    using pointer           = value_type*;
    using reference         = value_type&;
public:

    CyclicIterator(const Iterator& begin, const Iterator& end);
    CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor);

    bool operator==(const CyclicIterator& other) const;
    bool operator!=(const CyclicIterator& other) const;

    reference operator*() const;
    pointer operator->() const;

    CyclicIterator& operator++();
    CyclicIterator operator++(int) &;
    CyclicIterator& operator--();
    CyclicIterator operator--(int) &;

private:
    Iterator begin;
    Iterator end;
    Iterator cursor;
};

template<typename T, typename Iterator>
CyclicIterator<T, Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end):
    begin(begin),
    end(end),
    cursor(begin) {}

template<typename T, typename Iterator>
CyclicIterator<T, Iterator>::CyclicIterator(const Iterator& begin, const Iterator& end, const Iterator& cursor):
    begin(begin),
    end(end),
    cursor(cursor) {}

template<typename T, typename Iterator>
bool CyclicIterator<T, Iterator>::operator==(const CyclicIterator& other) const {
    return cursor == other.cursor;
}

template<typename T, typename Iterator>
bool CyclicIterator<T, Iterator>::operator!=(const CyclicIterator& other) const {
    return cursor != other.cursor;
}

template<typename T, typename Iterator>
typename CyclicIterator<T, Iterator>::reference CyclicIterator<T, Iterator>::operator*() const {
    return *cursor;
}

template<typename T, typename Iterator>
typename CyclicIterator<T, Iterator>::pointer CyclicIterator<T, Iterator>::operator->() const {
    return cursor;
}

template<typename T, typename Iterator>
CyclicIterator<T, Iterator>& CyclicIterator<T, Iterator>::operator++() {
    ++cursor;
    if (cursor == end) cursor = begin;
    return *this;
}

template<typename T, typename Iterator>
CyclicIterator<T, Iterator> CyclicIterator<T, Iterator>::operator++(int) & {
    CyclicIterator result = *this;
    ++*this;
    return result;
}

template<typename T, typename Iterator>
CyclicIterator<T, Iterator>& CyclicIterator<T, Iterator>::operator--() {
    if (cursor == begin) cursor = end;
    --cursor;
    return *this;
}

template<typename T, typename Iterator>
CyclicIterator<T, Iterator> CyclicIterator<T, Iterator>::operator--(int) & {
    CyclicIterator result = *this;
    --*this;
    return result;
}
