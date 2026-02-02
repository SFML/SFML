#include <SFML/Network/Http.hpp>

#include <catch2/catch_test_macros.hpp>

#include <NetworkUtil.hpp>
#include <type_traits>

TEST_CASE("[Network] sf::Http")
{
    SECTION("Type traits")
    {
        STATIC_CHECK(!std::is_copy_constructible_v<sf::Http>);
        STATIC_CHECK(!std::is_copy_assignable_v<sf::Http>);
        STATIC_CHECK(!std::is_nothrow_move_constructible_v<sf::Http>);
        STATIC_CHECK(!std::is_nothrow_move_assignable_v<sf::Http>);
    }

    SECTION("setHost")
    {
        sf::Http http;

        SECTION("Valid host w/ prefix")
        {
            CHECK(http.setHost("http://google.com"));
        }

        SECTION("Valid host w/o prefix")
        {
            CHECK(http.setHost("google.com"));
        }

        SECTION("Invalid host w/ prefix")
        {
            CHECK(!http.setHost("http://dummy"));
        }

        SECTION("Invalid host w/o prefix")
        {
            CHECK(!http.setHost("dummy"));
        }
    }

    SECTION("Request")
    {
        SECTION("Type traits")
        {
            STATIC_CHECK(std::is_copy_constructible_v<sf::Http::Request>);
            STATIC_CHECK(std::is_copy_assignable_v<sf::Http::Request>);
            STATIC_CHECK(std::is_move_constructible_v<sf::Http::Request>);
            STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Http::Request>);
        }
    }

    SECTION("Response")
    {
        SECTION("Type traits")
        {
            STATIC_CHECK(std::is_copy_constructible_v<sf::Http::Response>);
            STATIC_CHECK(std::is_copy_assignable_v<sf::Http::Response>);
            STATIC_CHECK(std::is_move_constructible_v<sf::Http::Response>);
            STATIC_CHECK(std::is_nothrow_move_assignable_v<sf::Http::Response>);
        }

        SECTION("Construction")
        {
            const sf::Http::Response response;
            CHECK(response.getField("").empty());
            CHECK(response.getStatus() == sf::Http::Response::Status::ConnectionFailed);
            CHECK(response.getMajorHttpVersion() == 0);
            CHECK(response.getMinorHttpVersion() == 0);
            CHECK(response.getBody().empty());
        }
    }
}

TEST_CASE("[Network] sf::Http Connection", runIpV4InternetTests())
{
    SECTION("HTTP Connection")
    {
        const sf::Http http("http://github.com");

        SECTION("Request Index")
        {
            const sf::Http::Response         response = http.sendRequest(sf::Http::Request{}, sf::milliseconds(1000));
            const sf::Http::Response::Status status   = response.getStatus();

            CHECK_FALSE(response.getMajorHttpVersion() == 0);
            CHECK(status == sf::Http::Response::Status::MovedPermanently);
            CHECK(response.getField("Location") == "https://github.com/");
            CHECK(response.getField("location") == "https://github.com/");
        }

        SECTION("Request Resource")
        {
            const sf::Http::Response response = http.sendRequest(sf::Http::Request("sitemap"), sf::milliseconds(1000));
            const sf::Http::Response::Status status = response.getStatus();

            CHECK_FALSE(response.getMajorHttpVersion() == 0);
            CHECK(status == sf::Http::Response::Status::MovedPermanently);
            CHECK(response.getField("Location") == "https://github.com/sitemap");
            CHECK(response.getField("location") == "https://github.com/sitemap");
        }
    }

    SECTION("HTTPS Connection")
    {
        SECTION("Request Index")
        {
            const sf::Http                   http("https://github.com");
            const sf::Http::Response         response = http.sendRequest(sf::Http::Request{}, sf::milliseconds(1000));
            const sf::Http::Response::Status status   = response.getStatus();

            CHECK_FALSE(response.getMajorHttpVersion() == 0);
            CHECK(status == sf::Http::Response::Status::Ok);
            CHECK_FALSE(response.getField("Server").empty());
            CHECK_FALSE(response.getField("server").empty());
            CHECK_FALSE(response.getField("Content-Type").empty());
            CHECK_FALSE(response.getField("content-type").empty());
            CHECK_FALSE(response.getBody().empty());
        }

        SECTION("Request Resource")
        {
            const sf::Http           http("https://github.com");
            const sf::Http::Response response = http.sendRequest(sf::Http::Request("sitemap"), sf::milliseconds(1000));
            const sf::Http::Response::Status status = response.getStatus();

            CHECK_FALSE(response.getMajorHttpVersion() == 0);
            CHECK(status == sf::Http::Response::Status::Ok);
            CHECK_FALSE(response.getField("Server").empty());
            CHECK_FALSE(response.getField("server").empty());
            CHECK_FALSE(response.getField("Content-Type").find("text/html") == std::string::npos);
            CHECK_FALSE(response.getField("content-type").find("text/html") == std::string::npos);
            CHECK(response.getField("Accept-Ranges") == "bytes");
            CHECK(response.getField("accept-ranges") == "bytes");
            CHECK_FALSE(response.getBody().find("GitHub") == std::string::npos);
        }

        SECTION("Request Non-Existant Resource")
        {
            const sf::Http                   http("https://github.com");
            const sf::Http::Response         response = http.sendRequest(sf::Http::Request("RESOURCETHATDOESNOTEXIST"),
                                                                 sf::milliseconds(1000));
            const sf::Http::Response::Status status   = response.getStatus();

            CHECK_FALSE(response.getMajorHttpVersion() == 0);
            CHECK(status == sf::Http::Response::Status::NotFound);
            CHECK_FALSE(response.getField("Server").empty());
            CHECK_FALSE(response.getField("server").empty());
        }

        SECTION("HEAD Request")
        {
            const sf::Http    http("https://github.com");
            sf::Http::Request request("/", sf::Http::Request::Method::Head);
            request.setHttpVersion(1, 1);

            const sf::Http::Response         response = http.sendRequest(request, sf::milliseconds(1000));
            const sf::Http::Response::Status status   = response.getStatus();

            CHECK_FALSE(response.getMajorHttpVersion() == 0);
            CHECK(status == sf::Http::Response::Status::Ok);
            CHECK_FALSE(response.getField("Server").empty());
            CHECK_FALSE(response.getField("server").empty());
            CHECK_FALSE(response.getField("Content-Type").find("text/html") == std::string::npos);
            CHECK_FALSE(response.getField("content-type").find("text/html") == std::string::npos);
            CHECK(response.getField("Accept-Ranges") == "bytes");
            CHECK(response.getField("accept-ranges") == "bytes");
        }
    }
}
