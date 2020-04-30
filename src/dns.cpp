#include "dns.hpp"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <assert.h>

namespace idragnev {
    void detail::FreeAddressInfo::operator()(addrinfo* p) const {
        ::freeaddrinfo(p);
    }

    AddrInfoPtr spamhausLookup(const std::string_view dnsName) {
        const auto hints = [] {
            addrinfo hints;
            ZeroMemory(&hints, sizeof(hints));

            hints.ai_family = AF_UNSPEC;
            hints.ai_socktype = SOCK_STREAM;
            hints.ai_protocol = IPPROTO_TCP;

            return hints;
        }();
        constexpr auto port = std::string_view{"53"};

        addrinfo* result = nullptr;
        const auto ret = getaddrinfo(dnsName.data(), port.data(), &hints, &result);
        const auto d = detail::FreeAddressInfo{};

        return (ret == 0) ? AddrInfoPtr{result,  d} 
                          : AddrInfoPtr{nullptr, d};
    }
    
    std::string_view spamhausZone(const std::string_view ip) noexcept {
        if (ip == "127.0.0.2")  return "127.0.0.2 - SBL - Spamhaus SBL Data";
        if (ip == "127.0.0.3")  return "127.0.0.3 - SBL - Spamhaus SBL CSS Data";
        if (ip == "127.0.0.4")  return "127.0.0.4 - XBL - CBL Data";
        if (ip == "127.0.0.9")  return "127.0.0.9 - SBL - Spamhaus DROP / EDROP Data";
        if (ip == "127.0.0.10") return "127.0.0.10 - PBL - ISP Maintained";
        if (ip == "127.0.0.11") return "127.0.0.11 - PBL - Spamhaus Maintained";
        
        return "Unknown";
    }
} //namespace idragnev
