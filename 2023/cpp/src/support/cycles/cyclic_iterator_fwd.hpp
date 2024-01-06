//
// Created by Conor Pappas on 1/6/24.
//

#pragma once

namespace support {

    template <std::forward_iterator Iterator>
    class CyclicIterator;

    template <std::forward_iterator Iterator>
    bool operator==(const CyclicIterator<Iterator>&, const CyclicIterator<Iterator>&);

    template <std::forward_iterator Iterator>
    bool operator==(CyclicIterator<Iterator>, typename CyclicIterator<Iterator>::InfiniteSentinel);

}
