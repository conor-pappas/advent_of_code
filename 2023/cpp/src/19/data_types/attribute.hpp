//
// Created by Conor Pappas on 1/15/24.
//

#pragma once

namespace avalanche::data_types {
    using rating = unsigned long long;

    enum class AttributeType {
        X = 'x',
        M = 'm',
        A = 'a',
        S = 's'
    };

    template<typename T>
    struct AttributeBundle {
        T x {};
        T m {};
        T a {};
        T s {};
    };

    template<typename T>
    const T& read_attr(const AttributeBundle<T>& t, const AttributeType attr) {
        switch(attr) {
            case AttributeType::X: return t.x;
            case AttributeType::M: return t.m;
            case AttributeType::A: return t.a;
            case AttributeType::S: return t.s;
            default: throw std::runtime_error("Invalid attribute_type");
        }
    }

    template<typename T>
    T& read_attr(AttributeBundle<T>& t, const AttributeType attr) {
        return const_cast<T&>(read_attr(const_cast<const AttributeBundle<T>&>(t), attr));
    }
}
