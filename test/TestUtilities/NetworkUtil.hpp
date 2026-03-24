#pragma once

#include <SFML/Network/IpAddress.hpp>

#include <string>
#include <string_view>
#include <vector>

[[nodiscard]] std::string runIpV4LoopbackTests();

[[nodiscard]] std::string runIpV4LinkTests();

[[nodiscard]] std::string runIpV4InternetTests();

[[nodiscard]] std::string runIpV6LoopbackTests();

[[nodiscard]] std::string runIpV6LinkTests();

[[nodiscard]] std::string runIpV6InternetTests();

[[nodiscard]] std::string runDnsTestsAdvanced();

[[nodiscard]] std::vector<sf::IpAddress> resolveV4(std::string_view hostname);

[[nodiscard]] std::vector<sf::IpAddress> resolveV6(std::string_view hostname);
