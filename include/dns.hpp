#pragma once

#include <string_view>
#include <memory>

struct addrinfo;

namespace idragnev {
    namespace detail {
        struct FreeAddressInfo
        {
            void operator()(addrinfo* p) const;
        };
    } //namespace detail

    using AddrInfoPtr = std::unique_ptr<addrinfo, detail::FreeAddressInfo>;
    
    AddrInfoPtr spamhausLookup(const std::string_view dnsName);
    std::string_view spamhausZone(const std::string_view ip) noexcept;
} //namespace idragnev