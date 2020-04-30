#include "format.hpp" 
#include <algorithm>

namespace idragnev {
    std::vector<std::string> toDNSNames(const std::vector<std::string_view>& addresses) {
        auto result = std::vector<std::string>{};
        result.reserve(addresses.size());

        for (const auto& a : addresses) {
            result.push_back(toDNSName(a));
        }

        return result;
    }

    std::string toDNSName(std::string_view address) {
        auto result = reverseOctets(address);
        result.append(".zen.spamhaus.org");
        
        return result;
    }

    std::string reverseOctets(std::string_view address) {
        auto result = std::string{address.rbegin(), address.rend()};
        
        const auto end = result.end();
        auto from = result.begin();
        auto to = std::find(from, end, '.');

        while (from != end) {
            std::reverse(from, to);

            from = (to != end) ? to + 1 : to;
            to = std::find(from, end, '.');
        }

        return result;
    }
} //namespace idragnev