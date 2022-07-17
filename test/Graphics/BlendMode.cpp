#include <SFML/Graphics/BlendMode.hpp>

#include <doctest/doctest.h>

#include <GraphicsUtil.hpp>

TEST_CASE("sf::BlendMode class - [graphics]")
{
    SUBCASE("Construction")
    {
        SUBCASE("Default constructor")
        {
            const sf::BlendMode blendMode;
            CHECK(blendMode.colorSrcFactor == sf::BlendMode::SrcAlpha);
            CHECK(blendMode.colorDstFactor == sf::BlendMode::OneMinusSrcAlpha);
            CHECK(blendMode.colorEquation == sf::BlendMode::Add);
            CHECK(blendMode.alphaSrcFactor == sf::BlendMode::One);
            CHECK(blendMode.alphaDstFactor == sf::BlendMode::OneMinusSrcAlpha);
            CHECK(blendMode.alphaEquation == sf::BlendMode::Add);
        }

        SUBCASE("Combined color and alpha constructor using default parameter")
        {
            const sf::BlendMode blendMode(sf::BlendMode::Zero, sf::BlendMode::SrcColor);
            CHECK(blendMode.colorSrcFactor == sf::BlendMode::Zero);
            CHECK(blendMode.colorDstFactor == sf::BlendMode::SrcColor);
            CHECK(blendMode.colorEquation == sf::BlendMode::Add);
            CHECK(blendMode.alphaSrcFactor == sf::BlendMode::Zero);
            CHECK(blendMode.alphaDstFactor == sf::BlendMode::SrcColor);
            CHECK(blendMode.alphaEquation == sf::BlendMode::Add);
        }

        SUBCASE("Combined color and alpha constructor")
        {
            const sf::BlendMode blendMode(sf::BlendMode::Zero, sf::BlendMode::SrcColor, sf::BlendMode::ReverseSubtract);
            CHECK(blendMode.colorSrcFactor == sf::BlendMode::Zero);
            CHECK(blendMode.colorDstFactor == sf::BlendMode::SrcColor);
            CHECK(blendMode.colorEquation == sf::BlendMode::ReverseSubtract);
            CHECK(blendMode.alphaSrcFactor == sf::BlendMode::Zero);
            CHECK(blendMode.alphaDstFactor == sf::BlendMode::SrcColor);
            CHECK(blendMode.alphaEquation == sf::BlendMode::ReverseSubtract);
        }

        SUBCASE("Separate color and alpha constructor")
        {
            const sf::BlendMode blendMode(sf::BlendMode::Zero,
                                          sf::BlendMode::SrcColor,
                                          sf::BlendMode::ReverseSubtract,
                                          sf::BlendMode::OneMinusDstAlpha,
                                          sf::BlendMode::DstAlpha,
                                          sf::BlendMode::Max);
            CHECK(blendMode.colorSrcFactor == sf::BlendMode::Zero);
            CHECK(blendMode.colorDstFactor == sf::BlendMode::SrcColor);
            CHECK(blendMode.colorEquation == sf::BlendMode::ReverseSubtract);
            CHECK(blendMode.alphaSrcFactor == sf::BlendMode::OneMinusDstAlpha);
            CHECK(blendMode.alphaDstFactor == sf::BlendMode::DstAlpha);
            CHECK(blendMode.alphaEquation == sf::BlendMode::Max);
        }
    }

    SUBCASE("Operators")
    {
        SUBCASE("operator==")
        {
            CHECK(sf::BlendMode() == sf::BlendMode());
            CHECK(sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One) ==
                  sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One));
            CHECK(sf::BlendMode(sf::BlendMode::Zero,
                                sf::BlendMode::SrcColor,
                                sf::BlendMode::ReverseSubtract,
                                sf::BlendMode::OneMinusDstAlpha,
                                sf::BlendMode::DstAlpha,
                                sf::BlendMode::Max) ==
                  sf::BlendMode(sf::BlendMode::Zero,
                                sf::BlendMode::SrcColor,
                                sf::BlendMode::ReverseSubtract,
                                sf::BlendMode::OneMinusDstAlpha,
                                sf::BlendMode::DstAlpha,
                                sf::BlendMode::Max));

            CHECK_FALSE(sf::BlendMode() == sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One));
            CHECK_FALSE(sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One) ==
                        sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
            CHECK_FALSE(
                sf::BlendMode(sf::BlendMode::Zero,
                              sf::BlendMode::SrcColor,
                              sf::BlendMode::ReverseSubtract,
                              sf::BlendMode::OneMinusDstAlpha,
                              sf::BlendMode::DstAlpha,
                              sf::BlendMode::Max) ==
                sf::BlendMode(sf::BlendMode::One,
                              sf::BlendMode::SrcColor,
                              sf::BlendMode::ReverseSubtract,
                              sf::BlendMode::OneMinusDstAlpha,
                              sf::BlendMode::DstAlpha,
                              sf::BlendMode::Max));
        }

        SUBCASE("operator!=")
        {
            CHECK_FALSE(sf::BlendMode() != sf::BlendMode());
            CHECK_FALSE(sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One) !=
                        sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One));
            CHECK_FALSE(
                sf::BlendMode(sf::BlendMode::Zero,
                              sf::BlendMode::SrcColor,
                              sf::BlendMode::ReverseSubtract,
                              sf::BlendMode::OneMinusDstAlpha,
                              sf::BlendMode::DstAlpha,
                              sf::BlendMode::Max) !=
                sf::BlendMode(sf::BlendMode::Zero,
                              sf::BlendMode::SrcColor,
                              sf::BlendMode::ReverseSubtract,
                              sf::BlendMode::OneMinusDstAlpha,
                              sf::BlendMode::DstAlpha,
                              sf::BlendMode::Max));

            CHECK(sf::BlendMode() != sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One));
            CHECK(sf::BlendMode(sf::BlendMode::Zero, sf::BlendMode::One) !=
                  sf::BlendMode(sf::BlendMode::One, sf::BlendMode::Zero));
            CHECK(sf::BlendMode(sf::BlendMode::Zero,
                                sf::BlendMode::SrcColor,
                                sf::BlendMode::ReverseSubtract,
                                sf::BlendMode::OneMinusDstAlpha,
                                sf::BlendMode::DstAlpha,
                                sf::BlendMode::Max) !=
                  sf::BlendMode(sf::BlendMode::One,
                                sf::BlendMode::SrcColor,
                                sf::BlendMode::ReverseSubtract,
                                sf::BlendMode::OneMinusDstAlpha,
                                sf::BlendMode::DstAlpha,
                                sf::BlendMode::Max));
        }
    }

    SUBCASE("Static constants")
    {
        CHECK(sf::BlendAlpha.colorSrcFactor == sf::BlendMode::SrcAlpha);
        CHECK(sf::BlendAlpha.colorDstFactor == sf::BlendMode::OneMinusSrcAlpha);
        CHECK(sf::BlendAlpha.colorEquation == sf::BlendMode::Add);
        CHECK(sf::BlendAlpha.alphaSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendAlpha.alphaDstFactor == sf::BlendMode::OneMinusSrcAlpha);
        CHECK(sf::BlendAlpha.alphaEquation == sf::BlendMode::Add);

        CHECK(sf::BlendAdd.colorSrcFactor == sf::BlendMode::SrcAlpha);
        CHECK(sf::BlendAdd.colorDstFactor == sf::BlendMode::One);
        CHECK(sf::BlendAdd.colorEquation == sf::BlendMode::Add);
        CHECK(sf::BlendAdd.alphaSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendAdd.alphaDstFactor == sf::BlendMode::One);
        CHECK(sf::BlendAdd.alphaEquation == sf::BlendMode::Add);

        CHECK(sf::BlendMultiply.colorSrcFactor == sf::BlendMode::DstColor);
        CHECK(sf::BlendMultiply.colorDstFactor == sf::BlendMode::Zero);
        CHECK(sf::BlendMultiply.colorEquation == sf::BlendMode::Add);
        CHECK(sf::BlendMultiply.alphaSrcFactor == sf::BlendMode::DstColor);
        CHECK(sf::BlendMultiply.alphaDstFactor == sf::BlendMode::Zero);
        CHECK(sf::BlendMultiply.alphaEquation == sf::BlendMode::Add);

        CHECK(sf::BlendMin.colorSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendMin.colorDstFactor == sf::BlendMode::One);
        CHECK(sf::BlendMin.colorEquation == sf::BlendMode::Min);
        CHECK(sf::BlendMin.alphaSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendMin.alphaDstFactor == sf::BlendMode::One);
        CHECK(sf::BlendMin.alphaEquation == sf::BlendMode::Min);

        CHECK(sf::BlendMax.colorSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendMax.colorDstFactor == sf::BlendMode::One);
        CHECK(sf::BlendMax.colorEquation == sf::BlendMode::Max);
        CHECK(sf::BlendMax.alphaSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendMax.alphaDstFactor == sf::BlendMode::One);
        CHECK(sf::BlendMax.alphaEquation == sf::BlendMode::Max);

        CHECK(sf::BlendNone.colorSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendNone.colorDstFactor == sf::BlendMode::Zero);
        CHECK(sf::BlendNone.colorEquation == sf::BlendMode::Add);
        CHECK(sf::BlendNone.alphaSrcFactor == sf::BlendMode::One);
        CHECK(sf::BlendNone.alphaDstFactor == sf::BlendMode::Zero);
        CHECK(sf::BlendNone.alphaEquation == sf::BlendMode::Add);
    }
}
