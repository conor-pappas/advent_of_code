//
// Created by Conor Pappas on 12/13/23.
//

#ifndef HAND_HPP
#define HAND_HPP

#include <array>
#include <ostream>

#include "card.hpp"

namespace camel {

    class Hand {
    public:
        enum class Type: unsigned char {
            high_card = 0,
            one_pair = 1,
            two_pair = 2,
            three_of_a_kind = 3,
            full_house = 4,
            four_of_a_kind = 5,
            five_of_a_kind = 6
        };

        constexpr static int HAND_SIZE = 5;
        using CardBuffer = std::array<Card, HAND_SIZE>;
        using bid_t = long;

        CardBuffer cards{};
        bid_t bid {};

        [[nodiscard]] Type type() const;

        [[nodiscard]] bool operator<(const Hand& other) const;
        [[nodiscard]] bool operator>(const Hand& other) const;
        [[nodiscard]] bool operator==(const Hand& other) const;
        [[nodiscard]] bool operator<=(const Hand& other) const;
        [[nodiscard]] bool operator>=(const Hand& other) const;

        static std::string type_to_string(const Type& type);

        friend std::ostream& operator<<(std::ostream& os, const Hand& hand);

    private:
        struct Private;
    };

};

#endif //HAND_HPP
