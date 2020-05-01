#pragma once

#include <string_view>
#include <string>
#include <optional>
#include <vector>

struct addrinfo;

namespace idragnev {
    struct LookupInfo
    {
        std::string ipAddress;
        std::optional<std::vector<std::string>> zones;
    };

    [[nodiscard]] std::optional<std::vector<LookupInfo>> spamhausLookup(const std::vector<std::string_view>& ipAddresses);
    
    namespace detail {
        [[nodiscard]] bool initialize();
        std::optional<std::vector<std::string>> spamhausLookup(std::string_view dnsName);
        std::vector<std::string> parseResponse(const addrinfo* const response);
        std::string_view ipAddress(const addrinfo* const info);
        std::optional<std::string_view> spamhausZone(const std::string_view ip) noexcept;
    } //namespace detail
} //namespace idragnev