//
// Created by Conor Pappas on 12/13/23.
//

#include "card.hpp"

#include <optional>
#include <stdexcept>

namespace camel {
    struct Card::Private {
        static std::optional<camel::Card::Value> char_to_card(const unsigned char& c) {
            if (c >= '2' && c <= '9') { return static_cast<Value>(c - '2' + 2); }
            switch (c) {
                case 'T': return Value::ten;
                case 'J': return Value::jack;
                case 'Q': return Value::queen;
                case 'K': return Value::king;
                case 'A': return Value::ace;
                default: return {};
            }
        }
    };



    Card::Card(const Value& value):
        value(value) {}

    Card::Card(const unsigned char& value):
        value() {
        if(const auto card = Private::char_to_card(value)) {
            this->value = *card;
        } else {
            throw std::runtime_error("Invalid card");
        }
    }

    // TODO: I wonder if it's actually faster to switch the 8 values
    Card::operator char() const {
        if (value >= Value::two && value <= Value::nine) { return static_cast<char>(value) + '2' - 2; }
        switch (value) {
            case Value::ten: return 'T';
            case Value::jack: return 'J';
            case Value::queen: return 'Q';
            case Value::king: return 'K';
            case Value::ace: return 'A';
            default: throw std::runtime_error("Invalid card");
        }
    }
}
