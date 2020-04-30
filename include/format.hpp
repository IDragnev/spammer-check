#pragma once

#include <string_view>
#include <string>
#include <vector>

namespace idragnev {
    std::vector<std::string> toDNSNames(const std::vector<std::string_view>& addresses);
    std::string toDNSName(std::string_view address);
    std::string reverseOctets(std::string_view address);
} //namespace idragnev