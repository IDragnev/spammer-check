#include <iostream>

#include "lookup.hpp"

template <typename... Args>
inline void print(const Args&... args) noexcept {
    (std::cout << ... << args);
}

void printUsage(const std::string_view progname) noexcept;

[[nodiscard]] std::vector<std::string_view> parseArgs(const int argc, const char** argv);

void printResponse(const std::vector<idragnev::LookupInfo>& infos) noexcept;

template <typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template <typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main(const int argc, const char** argv) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
   
    const auto ipdAddresses = parseArgs(argc, argv);
    const auto result = idragnev::spamhausLookup(ipdAddresses);

    std::visit(overloaded{
        [](const idragnev::StartupFailedTag) {
            print("Startup failed\n");
        },
        [](const std::vector<idragnev::LookupInfo>& v) {
            printResponse(v);
        },
    }, result);
}

void printUsage(const std::string_view progname) noexcept {
    print("usage: ", progname, " { <ipv4_address> }+ \n");
}

std::vector<std::string_view> parseArgs(const int argc, const char** argv) {
    std::vector<std::string_view> addresses;
    addresses.reserve(argc);

    for (auto i = 1; i < argc; ++i) {
        addresses.push_back({argv[i]});
    }

    return addresses;
}

void printResponse(const std::vector<idragnev::LookupInfo>& response) noexcept {
    for (const auto& addrInfo : response) {
        std::visit(overloaded{
            [&](const idragnev::NotASpammerTag&) { 
                print("The IP address: ", addrInfo.ipAddress, " is not a spammer\n\n"); 
            },
            [&](const std::vector<idragnev::DSBLZone>& zones) {
                for (const auto& zone : zones) {
                    print("The IP address: ", addrInfo.ipAddress,
                          " was found in the following Spamhaus public IP zone: ", zone, "\n");
                }
                print("\n\n");
            },
            [&](const idragnev::SocketError err) {
                print("Lookup for ", addrInfo.ipAddress, " failed with error code ", err.code, "\n\n");
            }, 
        }, addrInfo.result);
    }
}