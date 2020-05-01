#define _WINSOCK_DEPRECATED_NO_WARNINGS

#undef UNICODE

#pragma comment (lib, "Ws2_32.lib")

#include "lookup.hpp"
#include "format.hpp"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <assert.h>
#include <iostream>

namespace idragnev {
    template <typename... Args>
    inline void print(const Args&... args) noexcept {
        (std::cout << ... << args);
    }

    template <typename F>
    struct ScopedFn
    {
    public:
        ScopedFn(F f) : f{ f } { }
        ~ScopedFn() { f(); }
    private:
        F f;
    };

    std::variant<StartupFailedTag, std::vector<LookupInfo>> spamhausLookup(const std::vector<std::string_view>& ipAddresses) {
        if (!detail::initialize()) {
            return StartupFailedTag{};
        }

        const auto _ = ScopedFn{ [] { WSACleanup(); } };

        const auto dnsNames = toDNSNames(ipAddresses);
        assert(dnsNames.size() == ipAddresses.size());

        std::vector<LookupInfo> result;
        for (auto i = 0; i < dnsNames.size(); ++i) {
            result.push_back(LookupInfo{ 
                ipAddresses[i].data(),
                detail::spamhausLookup(dnsNames[i])
            });
        }

        return std::move(result);
    }

    bool detail::initialize()
    {
        WSADATA wsaData;
        const auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        return result == 0;
    }

    LookupResult detail::spamhausLookup(const std::string_view dnsName) {
        const auto hints = [] {
            addrinfo hints;
            ZeroMemory(&hints, sizeof(hints));

            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            return hints;
        }();
        constexpr auto port = std::string_view{"53"};

        addrinfo* response = nullptr;
        const auto ret = getaddrinfo(dnsName.data(), port.data(), &hints, &response);

        if (ret == 0) {
            return parseResponse(response);
        }
        else if (ret == WSAHOST_NOT_FOUND) {
            return NotASpammerTag{};
        }
        
        return SocketError{ret};
    }
    
    LookupResult detail::parseResponse(const addrinfo* const response) {
        std::vector<DSBLZone> result;

        for (auto ptr = response; ptr != nullptr; ptr = ptr->ai_next) {
            const auto ip = ipAddress(ptr);
            const auto zone = spamhausZone(ip);
            assert(zone.has_value());
            result.emplace_back(zone->data());
        }

        return result;
    }

    std::string_view detail::ipAddress(const addrinfo* const info) {
        assert(info != nullptr);
        assert(info->ai_family == AF_INET);

        const auto sockaddr_ipv4 = (sockaddr_in*)info->ai_addr;
        return inet_ntoa(sockaddr_ipv4->sin_addr);
    }

    std::optional<std::string_view> detail::spamhausZone(const std::string_view ip) noexcept {
        const auto* zone = [&ip] {
            if (ip == "127.0.0.2")  return "127.0.0.2 - SBL - Spamhaus SBL Data";
            if (ip == "127.0.0.3")  return "127.0.0.3 - SBL - Spamhaus SBL CSS Data";
            if (ip == "127.0.0.4")  return "127.0.0.4 - XBL - CBL Data";
            if (ip == "127.0.0.9")  return "127.0.0.9 - SBL - Spamhaus DROP / EDROP Data";
            if (ip == "127.0.0.10") return "127.0.0.10 - PBL - ISP Maintained";
            if (ip == "127.0.0.11") return "127.0.0.11 - PBL - Spamhaus Maintained";

            return "";
        }();

        return zone != "" ? std::make_optional(zone) : std::nullopt;
    }
 } //namespace idragnev
