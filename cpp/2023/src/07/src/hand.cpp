//
// Created by Conor Pappas on 12/13/23.
//

#include "hand.hpp"

#include <map>

namespace camel {

    struct Hand::Private {
        static signed char compare(const Hand& a, const Hand& b);
    };

    Hand::Type Hand::type() const {
        std::map<Card::Value, int> value_counts;
        for (const auto& card : this->cards) {
            value_counts[card.value]++;
        }
        int match_counts[4] = {};
        for (const auto& [value, count] : value_counts) {
            if(count > 1) {
                match_counts[count - 2]++;
            }
        }
        if(match_counts[3] == 1) { return Type::five_of_a_kind; }
        if(match_counts[2] == 1) { return Type::four_of_a_kind; }
        if(match_counts[1] == 1 && match_counts[0] == 1) { return Type::full_house; }
        if(match_counts[1] == 1) { return Type::three_of_a_kind; }
        if(match_counts[0] == 2) { return Type::two_pair; }
        if(match_counts[0] == 1) { return Type::one_pair; }
        return Type::high_card;
    }

    bool Hand::operator<(const Hand& other) const {
        return Private::compare(*this, other) < 0;
    }

    bool Hand::operator>(const Hand& other) const {
        return Private::compare(*this, other) > 0;
    }

    bool Hand::operator==(const Hand& other) const {
        return Private::compare(*this, other) == 0;
    }

    bool Hand::operator<=(const Hand& other) const {
        return Private::compare(*this, other) <= 0;
    }

    bool Hand::operator>=(const Hand& other) const {
        return Private::compare(*this, other) >= 0;
    }

    std::string Hand::type_to_string(const Type& type) {
        switch(type) {
            case Type::high_card: return "High Card";
            case Type::one_pair: return "One Pair";
            case Type::two_pair: return "Two Pair";
            case Type::three_of_a_kind: return "Three of a Kind";
            case Type::full_house: return "Full House";
            case Type::four_of_a_kind: return "Four of a Kind";
            case Type::five_of_a_kind: return "Five of a Kind";
            default: throw std::runtime_error("Invalid hand type");
        }
    }

    signed char Hand::Private::compare(const Hand& a, const Hand& b) {
        if (a.type() < b.type()) { return -1; }
        if (a.type() > b.type()) { return 1; }
        for (int i = 0; i < Hand::HAND_SIZE; i++) {
            if (a.cards[i].value < b.cards[i].value) { return -1; }
            if (a.cards[i].value > b.cards[i].value) { return 1; }
        }
        return 0;
    };

    std::ostream& operator<<(std::ostream& os, const camel::Hand& hand) {
        os << "[" << Hand::type_to_string(hand.type()) << "]" << " : ";
        for (const auto& card : hand.cards) {
            os << static_cast<char>(card);
        }
        os << " : $" << hand.bid;
        return os;
    }

}
