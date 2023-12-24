//
// Created by Conor Pappas on 12/23/23.
//

#include "finder.hpp"

namespace spring::arrangement_finder {
    Result Finder::find(const Request& request) {
        return find(request.record, request.pattern);
    }

    const Result& Finder::cache_result(const Request& request, const Result& result) {
        return m_request_cache[request] = result;
    }

    std::optional<const Result> Finder::cache_lookup(const Request&request) {
        if(m_request_cache.contains(request)) {
            auto result = m_request_cache[request];
            return result;
        }
        return {};
    }

    Finder::result_count Finder::cache_result_count(const Request& request, result_count result) {
        return m_request_count_cache[request] = result;
    }

    std::optional<Finder::result_count> Finder::cache_lookup_count(const Request& request) {
        if(m_request_count_cache.contains(request)) {
            auto result = m_request_count_cache[request];
            return result;
        }
        return {};
    }
}
