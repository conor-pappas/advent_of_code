//
// Created by Conor Pappas on 1/5/24.
//

#pragma once

#include <cycles/cyclic_view.hpp>
#include <utility>

#include "platform.hpp"
#include "roll_iterator.hpp"

namespace dish::data_types {
    template<std::forward_iterator Iterator>
    class RollIterator;

    template<std::forward_iterator Iterator>
    bool operator==(const RollIterator<Iterator>& lhs, const RollIterator<Iterator>& rhs);

    template<std::forward_iterator Iterator>
    class RollIterator {
        using direction_view = support::CyclicView<Iterator, 0>;
        using direciton_iterator = typename direction_view::iterator;
    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = ptrdiff_t;
        using value_type        = const Platform;
        using pointer           = value_type*;
        using reference         = value_type&;

        RollIterator();
        RollIterator(Platform, const Iterator& dir_begin, const Iterator& dir_end);
        RollIterator(Platform, const direction_view&);
        RollIterator(Platform, const direciton_iterator&);

        // TODO: This is not quite an iterator. We aren't a light pointer wrapper. Instead this is a "time-evolution"
        // type object. It has ++ operators, but after incrementing we are destroying the data that existed before.
        // We could make some concepts to highlight this difference. (mostly going to be around asserting that
        // incrementing after dereferencing doesn't affect the dereferenced object)
        [[nodiscard]] reference operator*() const;
        [[nodiscard]] pointer operator->() const;
        RollIterator& operator++();
        RollIterator operator++(int) &;

        friend bool operator==<Iterator>(const RollIterator&, const RollIterator&);

        private:
            Platform m_platform {};
            direciton_iterator m_directions;
    };

    template<std::forward_iterator Iterator>
    RollIterator<Iterator>::RollIterator() = default;

    template<std::forward_iterator Iterator>
    RollIterator<Iterator>::RollIterator(Platform platform, const Iterator& dir_begin, const Iterator& dir_end):
        RollIterator(std::move(platform), direction_view(dir_begin, dir_end)) {}

    template<std::forward_iterator Iterator>
    RollIterator<Iterator>::RollIterator(Platform platform, const direction_view& directions):
        RollIterator(std::move(platform), directions.begin()) {}

    template<std::forward_iterator Iterator>
    RollIterator<Iterator>::RollIterator(Platform  platform, const direciton_iterator& directions):
        m_platform(std::move(platform)),
        m_directions(directions) {}

    template<std::forward_iterator Iterator>
    typename RollIterator<Iterator>::reference RollIterator<Iterator>::operator*() const {
        return m_platform;
    }

    template<std::forward_iterator Iterator>
    typename RollIterator<Iterator>::pointer RollIterator<Iterator>::operator->() const {
        return &m_platform;
    }

    template<std::forward_iterator Iterator>
    RollIterator<Iterator>& RollIterator<Iterator>::operator++() {
        m_platform.roll(*m_directions);
        ++m_directions;
        return *this;
    }

    template<std::forward_iterator Iterator>
    RollIterator<Iterator> RollIterator<Iterator>::operator++(int) & {
        RollIterator copy = *this;
        ++(*this);
        return copy;
    }

    template<std::forward_iterator Iterator>
    bool operator==(const RollIterator<Iterator>& lhs, const RollIterator<Iterator>& rhs) {
        return lhs.m_platform == rhs.m_platform && lhs.m_directions.mod_equals(rhs.m_directions);
    }
}
