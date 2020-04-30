#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "format.hpp"

TEST_CASE("reverse octets")
{
    const auto address = "127.123.121.22";

    const auto result = idragnev::reverseOctets(address);

    CHECK(result == "22.121.123.127");
}

TEST_CASE("format address")
{
    const auto address = "222.109.135.16";

    const auto result = idragnev::formatAddress(address);

    CHECK(result == "16.135.109.222.zen.spamhaus.org");
}