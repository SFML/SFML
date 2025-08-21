#include <SFML/Network/Sftp.hpp>

// Other 1st party headers
#include <SFML/Network/IpAddress.hpp>

#include <catch2/catch_test_macros.hpp>

#include <type_traits>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#else
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

TEST_CASE("[Network] sf::Sftp")
{
    SECTION("Type traits")
    {
        static_assert(!std::is_copy_constructible_v<sf::Sftp>);
        static_assert(!std::is_copy_assignable_v<sf::Sftp>);
        static_assert(!std::is_nothrow_move_constructible_v<sf::Sftp>);
        static_assert(!std::is_nothrow_move_assignable_v<sf::Sftp>);
    }

    SECTION("Result")
    {
        SECTION("Type traits")
        {
            static_assert(std::is_copy_constructible_v<sf::Sftp::Result>);
            static_assert(std::is_copy_assignable_v<sf::Sftp::Result>);
            static_assert(std::is_nothrow_move_constructible_v<sf::Sftp::Result>);
            static_assert(std::is_nothrow_move_assignable_v<sf::Sftp::Result>);
        }

        SECTION("Construction")
        {
            SECTION("Default constructor")
            {
                const sf::Sftp::Result response;
                CHECK_FALSE(response.isOk());
                CHECK(response.getValue() == sf::Sftp::Result::Value::Error);
                CHECK(response.getMessage().empty());
            }

            SECTION("Status constructor")
            {
                const sf::Sftp::Result response(sf::Sftp::Result::Value::SftpNoSuchFile);
                CHECK_FALSE(response.isOk());
                CHECK(response.getValue() == sf::Sftp::Result::Value::SftpNoSuchFile);
                CHECK(response.getMessage().empty());
            }

            SECTION("Status and message constructor")
            {
                const sf::Sftp::Result response(sf::Sftp::Result::Value::Success, "Success");
                CHECK(response.isOk());
                CHECK(response.getValue() == sf::Sftp::Result::Value::Success);
                CHECK(response.getMessage() == "Success");
            }
        }

        SECTION("isOk()")
        {
            CHECK(sf::Sftp::Result(sf::Sftp::Result::Value::Success).isOk());
            CHECK_FALSE(sf::Sftp::Result(sf::Sftp::Result::Value::SshErrorAuthenticationFailed).isOk());
            CHECK_FALSE(sf::Sftp::Result(sf::Sftp::Result::Value::SshErrorKeyExchangeFailure).isOk());
            CHECK_FALSE(sf::Sftp::Result(sf::Sftp::Result::Value::SftpNoSuchFile).isOk());
            CHECK_FALSE(sf::Sftp::Result(sf::Sftp::Result::Value::SftpPermissionDenied).isOk());
        }
    }

    SECTION("PathResult")
    {
        SECTION("Construction")
        {
            const sf::Sftp::PathResult pathResult(sf::Sftp::Result(sf::Sftp::Result::Value::Success, ""), {});
            CHECK(pathResult.isOk());
            CHECK(pathResult.getValue() == sf::Sftp::Result::Value::Success);
            CHECK(pathResult.getMessage().empty());
            CHECK(pathResult.getPath().empty());
        }

        SECTION("getPath()")
        {
            const sf::Sftp::PathResult pathResult(sf::Sftp::Result(sf::Sftp::Result::Value::Success, ""), "foo");
            CHECK(pathResult.getPath() == "foo");
        }
    }

    SECTION("AttributesResult")
    {
        SECTION("Construction")
        {
            const sf::Sftp::AttributesResult attributesResult(sf::Sftp::Result(sf::Sftp::Result::Value::Success, ""), {});
            CHECK(attributesResult.isOk());
            CHECK(attributesResult.getValue() == sf::Sftp::Result::Value::Success);
            CHECK(attributesResult.getMessage().empty());
            CHECK(attributesResult.getAttributes().path.empty());
        }

        SECTION("getAttributes()")
        {
            const auto                       now = std::filesystem::file_time_type::clock::now();
            const sf::Sftp::AttributesResult attributesResult(sf::Sftp::Result(sf::Sftp::Result::Value::Success, ""),
                                                              sf::Sftp::Attributes{"foo",
                                                                                   std::filesystem::file_type::regular,
                                                                                   1234,
                                                                                   std::filesystem::perms::group_exec,
                                                                                   4321,
                                                                                   5678,
                                                                                   now,
                                                                                   now});
            CHECK(attributesResult.getAttributes().path == "foo");
            CHECK(attributesResult.getAttributes().type == std::filesystem::file_type::regular);
            CHECK(attributesResult.getAttributes().size == 1234);
            CHECK(attributesResult.getAttributes().permissions == std::filesystem::perms::group_exec);
            CHECK(attributesResult.getAttributes().userId == 4321);
            CHECK(attributesResult.getAttributes().groupId == 5678);
            CHECK((attributesResult.getAttributes().accessTime == now));
            CHECK((attributesResult.getAttributes().modificationTime == now));
        }
    }

    SECTION("ListingResult")
    {
        SECTION("Construction")
        {
            const sf::Sftp::ListingResult listingResult(sf::Sftp::Result(sf::Sftp::Result::Value::Success, "Success"), {});
            CHECK(listingResult.isOk());
            CHECK(listingResult.getValue() == sf::Sftp::Result::Value::Success);
            CHECK(listingResult.getMessage() == "Success");
            CHECK(listingResult.getListing().empty());
        }

        SECTION("getListing()")
        {
            const auto now = std::filesystem::file_time_type::clock::now();
            CHECK(sf::Sftp::ListingResult(sf::Sftp::Result{}, {}).getListing().empty());
            const sf::Sftp::ListingResult
                listingResult(sf::Sftp::Result{sf::Sftp::Result::Value::Success, "Success"},
                              {sf::Sftp::Attributes{"foo",
                                                    std::filesystem::file_type::regular,
                                                    1234,
                                                    std::filesystem::perms::group_exec,
                                                    4321,
                                                    5678,
                                                    now,
                                                    now},
                               sf::Sftp::Attributes{"bar",
                                                    std::filesystem::file_type::block,
                                                    1235,
                                                    std::filesystem::perms::owner_exec,
                                                    5321,
                                                    5679,
                                                    now,
                                                    now}});
            CHECK(listingResult.getListing()[0].path == "foo");
            CHECK(listingResult.getListing()[0].type == std::filesystem::file_type::regular);
            CHECK(listingResult.getListing()[0].size == 1234);
            CHECK(listingResult.getListing()[0].permissions == std::filesystem::perms::group_exec);
            CHECK(listingResult.getListing()[0].userId == 4321);
            CHECK(listingResult.getListing()[0].groupId == 5678);
            CHECK((listingResult.getListing()[0].accessTime == now));
            CHECK((listingResult.getListing()[0].modificationTime == now));
            CHECK(listingResult.getListing()[1].path == "bar");
            CHECK(listingResult.getListing()[1].type == std::filesystem::file_type::block);
            CHECK(listingResult.getListing()[1].size == 1235);
            CHECK(listingResult.getListing()[1].permissions == std::filesystem::perms::owner_exec);
            CHECK(listingResult.getListing()[1].userId == 5321);
            CHECK(listingResult.getListing()[1].groupId == 5679);
            CHECK((listingResult.getListing()[1].accessTime == now));
            CHECK((listingResult.getListing()[1].modificationTime == now));
        }
    }

    SECTION("TimeoutPredicate")
    {
        SECTION("Construction using timeout")
        {
            const sf::Sftp::TimeoutPredicate timeoutPredicate(sf::milliseconds(1234));
            CHECK(timeoutPredicate.getPeriod() == sf::milliseconds(1234));
            REQUIRE(timeoutPredicate.getPredicate());
            CHECK(timeoutPredicate.getPredicate()() == false);
        }

        SECTION("Construction using predicate and period")
        {
            auto                             flag = false;
            const sf::Sftp::TimeoutPredicate timeoutPredicate([&flag] { return flag; }, sf::milliseconds(4321));
            CHECK(timeoutPredicate.getPeriod() == sf::milliseconds(4321));
            REQUIRE(timeoutPredicate.getPredicate());
            CHECK(timeoutPredicate.getPredicate()() == false);
            flag = true;
            CHECK(timeoutPredicate.getPredicate()() == true);
        }

        SECTION("Construction using empty predicate and period")
        {
            const sf::Sftp::TimeoutPredicate timeoutPredicate({}, sf::milliseconds(5678));
            CHECK(timeoutPredicate.getPeriod() == sf::milliseconds(5678));
            REQUIRE(timeoutPredicate.getPredicate());
            CHECK(timeoutPredicate.getPredicate()() == false);
        }
    }

    SECTION("Construction")
    {
        const sf::Sftp sftp;
        CHECK_FALSE(sftp.getSessionInfo().has_value());
    }

    SECTION("Connect to non-existant server and timeout")
    {
        sf::Sftp sftp;
        // 213.0.113.0/24 is reserved for TEST-NET-3 and shouldn't contain hosts under normal circumstances
        const auto result = sftp.connect(sf::IpAddress(213, 0, 113, 1), 22, sf::milliseconds(1));
        CHECK(result.getValue() == sf::Sftp::Result::Value::Timeout);
        CHECK(result.getMessage().empty());
        CHECK_FALSE(sftp.getSessionInfo().has_value());
    }
}
