#pragma once

#include <string_view>
#include <string>

namespace idragnev {
    std::string formatAddress(std::string_view address);
    std::string reverseOctets(std::string_view address);
} //namespace idragnev