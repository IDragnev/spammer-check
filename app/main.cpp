#define _WINSOCK_DEPRECATED_NO_WARNINGS

#undef UNICODE

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <assert.h>

#include "format.hpp"
#include "dns.hpp"

#pragma comment (lib, "Ws2_32.lib")

template <typename F>
struct ScopedFn
{
public:
    ScopedFn(F f) : f{ f } { }
    ~ScopedFn() { f(); }
private:
    F f;
};

void printUsage(const char** argv) noexcept;

template <typename... Args>
void print(const Args&... args) noexcept;

bool initializeWinsock() noexcept;

std::vector<std::string> parseArgs(const int argc, const char** argv);

void lookup(const std::vector<std::string>& dnsNames);
void printResponse(addrinfo* const response);
std::string_view extractIPAddress(const addrinfo* const info);

int main(const int argc, const char** argv) {
    if (argc < 2) {
        printUsage(argv);
        return 1;
    }
    if (!initializeWinsock()) {
        return 1;
    }
    
    const auto _ = ScopedFn{ [] { WSACleanup(); } };

    const auto dnsNames = parseArgs(argc, argv);
    lookup(dnsNames);
}

void printUsage(const char** argv) noexcept {
    print("usage: ", argv[0], " { <hostname> }+ \n");
}

template <typename... Args>
void print(const Args&... args) noexcept {
    (std::cout << ... << args);
}

bool initializeWinsock() noexcept {
    WSADATA wsaData;
    const auto result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        print("WSAStartup failed: ", result, "\n");
        return false;
    }

    return true;
}

std::vector<std::string> parseArgs(const int argc, const char** argv) {
    std::vector<std::string_view> addresses;
    addresses.reserve(argc);

    for (auto i = 1; i < argc; ++i) {
        addresses.push_back({argv[i]});
    }

    return idragnev::toDNSNames(addresses);
}

void lookup(const std::vector<std::string>& dnsNames) {
    for (const auto& name : dnsNames) {
        const auto response = idragnev::spamhausLookup(name);
        if (response != nullptr) {
            printResponse(response.get());
        }
        else {
            print("Looking up ", name, " failed");
        }
    }
}

void printResponse(addrinfo* const response) {
    for (auto ptr = response; ptr != nullptr; ptr = ptr->ai_next) {
        const auto ip = extractIPAddress(ptr);
        const auto zone = idragnev::spamhausZone(ip);
        print("The IP address: ", ip, " was found in the following Spamhaus public IP zone: ", zone, "\n");
    }
}

std::string_view extractIPAddress(const addrinfo* const info) {
    assert(info->ai_family == AF_INET);
    const auto sockaddr_ipv4 = (sockaddr_in*)info->ai_addr;
    return inet_ntoa(sockaddr_ipv4->sin_addr);
}