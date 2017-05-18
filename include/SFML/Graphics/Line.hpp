#ifndef LINE_FIDO_H
#define LINE_FIDO_H

#include <iostream>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

class Line : public sf::Drawable
{
public:
    Line(const sf::Vector2f& point1_, const sf::Vector2f& point2_):
        color(sf::Color::Yellow), thickness(2.f)
    {
        point1 = point1_;
        point2 = point2_;

        sf::Vector2f direction = point2 - point1;
        sf::Vector2f unitDirection = direction/std::sqrt(direction.x*direction.x+direction.y*direction.y);
        sf::Vector2f unitPerpendicular(-unitDirection.y,unitDirection.x);

        sf::Vector2f offset = (thickness/2.f)*unitPerpendicular;

        vertices[0].position = point1 + offset;
        vertices[1].position = point2 + offset;
        vertices[2].position = point2 - offset;
        vertices[3].position = point1 - offset;

        for (int i=0; i<4; ++i)
            vertices[i].color = color;
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const
    {
        target.draw(vertices,4,sf::PrimitiveType::Quads);
    }

    bool isLeft(sf::Vector2f center) {
        return ((point2.x - point1.x)*(center.y - point1.y) - (point2.y - point1.y)*(center.x - point1.x)) > 0;
    }

    double distance(sf::Vector2f center) {
        return fabs((point2.y - point1.y)*center.x - (point2.x-point1.x)*center.y + point2.x*point1.y - point2.y*point1.x) / sqrt(pow(point2.y-point1.y, 2) + pow(point2.x-point1.x, 2));
    }

    double getThickness() {
        return thickness;
    }
private:
    sf::Vertex vertices[4];
    sf::Vector2f point1, point2;
    float thickness;
    sf::Color color;
};

#endif
