#include <iostream>

#include "lookup.hpp"

template <typename... Args>
inline void print(const Args&... args) noexcept {
    (std::cout << ... << args);
}

void printUsage(const char** argv) noexcept;

[[nodiscard]] std::vector<std::string_view> parseArgs(const int argc, const char** argv);

void printResponse(const std::vector<idragnev::LookupInfo>& infos) noexcept;

int main(const int argc, const char** argv) {
    if (argc < 2) {
        printUsage(argv);
        return 1;
    }
   
    const auto ipdAddresses = parseArgs(argc, argv);
    const auto result = idragnev::spamhausLookup(ipdAddresses);

    if (result.has_value()) {
        printResponse(result.value());
    }
    else {
        print("Startup failed\n");
    }
}

void printUsage(const char** argv) noexcept {
    print("usage: ", argv[0], " { <hostname> }+ \n");
}

std::vector<std::string_view> parseArgs(const int argc, const char** argv) {
    std::vector<std::string_view> addresses;
    addresses.reserve(argc);

    for (auto i = 1; i < argc; ++i) {
        addresses.push_back({argv[i]});
    }

    return addresses;
}

void printResponse(const std::vector<idragnev::LookupInfo>& infos) noexcept {
    for (const auto& addrInfo : infos) {
        if (addrInfo.zones.has_value()) {
            for (const auto& zone : addrInfo.zones.value()) {
                print("The IP address: ", addrInfo.ipAddress,
                      " was found in the following Spamhaus public IP zone: ", zone, "\n");
            }
            print("\n\n");
        }
        else {
            print("Lookup for: ", addrInfo.ipAddress, " failed\n");
        }
    }
}