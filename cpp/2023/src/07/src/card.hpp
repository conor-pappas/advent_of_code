//
// Created by Conor Pappas on 12/13/23.
//

#ifndef CARD_HPP
#define CARD_HPP

namespace camel {
    class Card {
    public:
        enum class Value: unsigned char {
            two = 2,
            three = 3,
            four = 4,
            five = 5,
            six = 6,
            seven = 7,
            eight = 8,
            nine = 9,
            ten = 10,
            jack = 11,
            queen = 12,
            king = 13,
            ace = 14
        };

        Value value;

        Card() = default;
        explicit Card(const Value& value);
        explicit Card(const unsigned char&);

        explicit operator char() const;

    private:
        struct Private;
    };
};

#endif //CARD_HPP
