#pragma once

#include <string_view>
#include <string>
#include <optional>
#include <variant>
#include <vector>

struct addrinfo;

namespace idragnev {
    struct StartupFailedTag { };

    struct NotASpammerTag { };

    struct SocketError { int code = 0; };
    
    using DSBLZone = std::string;

    using LookupResult = std::variant<NotASpammerTag, SocketError, std::vector<DSBLZone>>;

    struct LookupInfo
    {
        std::string ipAddress;
        LookupResult result;
    };

    [[nodiscard]]
    std::variant<StartupFailedTag, std::vector<LookupInfo>> spamhausLookup(const std::vector<std::string_view>& ipAddresses);
    
    namespace detail {
        [[nodiscard]] bool initialize();
        LookupResult spamhausLookup(std::string_view dnsName);
        LookupResult parseResponse(const addrinfo* const response);
        std::string_view ipAddress(const addrinfo* const info);
        std::optional<std::string_view> spamhausZone(const std::string_view ip) noexcept;
    } //namespace detail
} //namespace idragnev