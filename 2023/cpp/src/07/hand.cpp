//
// Created by Conor Pappas on 12/13/23.
//

#include "hand.hpp"

#include <cassert>
#include <map>
#include <ranges>

namespace camel {

    struct Hand::Private {
        static signed char compare(const Hand& a, const Hand& b);
        static Hand::Type lookup(const std::array<int, Hand::HAND_SIZE>& match_counts, int wild_counts);
    };

    Hand::Type Hand::type() const {
        std::map<Card::Value, int> value_counts;
        for (const auto& card : this->cards) {
            value_counts[card.value]++;
        }
        int wild_counts = 0;
        if (wild_jokers) { wild_counts = value_counts[Card::Value::jack]; }
        if(wild_counts == Hand::HAND_SIZE) { return Type::five_of_a_kind; }
        std::array<int, Hand::HAND_SIZE> match_counts = {};
        for (const auto& [value, count] : value_counts) {
            assert(count <= Hand::HAND_SIZE);
            if(count > 0) {
                if(!wild_jokers || value != Card::Value::jack) {
                    match_counts[count - 1]++;
                }
            }
        }
        return Private::lookup(match_counts, wild_counts);
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
        assert(a.wild_jokers == b.wild_jokers);
        if (a.type() < b.type()) { return -1; }
        if (a.type() > b.type()) { return 1; }
        for (int i = 0; i < Hand::HAND_SIZE; i++) {
            auto a_value = static_cast<signed char>(a.cards[i].value);
            auto b_value = static_cast<signed char>(b.cards[i].value);
            if (a.wild_jokers) {
                if(a.cards[i].value == Card::Value::jack) { a_value = 1; }
                if(b.cards[i].value == Card::Value::jack) { b_value = 1; }
            }
            if (a_value < b_value) { return -1; }
            if (a_value > b_value) { return 1; }
        }
        return 0;
    }

    Hand::Type Hand::Private::lookup(const std::array<int, Hand::HAND_SIZE>& match_counts, const int wild_counts) {
        switch(wild_counts) {
            case 5: // JJJJJ
            case 4: return Type::five_of_a_kind; // JJJJA
            case 3: {
                if(match_counts[1] == 1) { return Type::five_of_a_kind; } // JJJAA
                return Type::four_of_a_kind; // JJJAB
            }
            case 2: {
                if(match_counts[2] == 1) { return Type::five_of_a_kind; } // JJAAA
                if(match_counts[1] == 1) { return Type::four_of_a_kind; } // JJAAB
                return Type::three_of_a_kind; // JJABC
            }
            case 1: {
                if(match_counts[3] == 1) { return Type::five_of_a_kind; } // JAAAA
                if(match_counts[2] == 1) { return Type::four_of_a_kind; } // JAAAB
                if(match_counts[1] == 2) { return Type::full_house; } // JAABB
                if(match_counts[1] == 1) {return Type::three_of_a_kind; } // JAABC
                return Type::one_pair; // JABCD
            }
            case 0: {
                if(match_counts[4] == 1) { return Type::five_of_a_kind; } // AAAAA
                if(match_counts[3] == 1) { return Type::four_of_a_kind; } // AAAAB
                if(match_counts[2] == 1 && match_counts[1] == 1) { return Type::full_house; } // AAABB
                if(match_counts[2] == 1) { return Type::three_of_a_kind; } // AAACC
                if(match_counts[1] == 2) { return Type::two_pair; } // AABBC
                if(match_counts[1] == 1) { return Type::one_pair; } // AABCD
                return Type::high_card; // ABCDE
            }
            default: throw std::runtime_error("Invalid hand");
        }
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
