////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics/RoundedRectangleShape.h>
#include <cmath>

namespace sf
{
////////////////////////////////////////////////////////////
RoundedRectangleShape::RoundedRectangleShape(const Vector2f& size, float radius, unsigned int cornerPointCount)
{

    mySize = size;
    myRadius = radius;
    myCornerPointCount = cornerPointCount;
    Update();
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::SetSize(const Vector2f& size)
{
    mySize = size;
    Update();
}

////////////////////////////////////////////////////////////
const Vector2f& RoundedRectangleShape::GetSize() const
{
    return mySize;
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::SetRadius(float radius)
{
    myRadius = radius;
    Update();
}

////////////////////////////////////////////////////////////
const float RoundedRectangleShape::GetRadius() const
{
    return myRadius;
}

////////////////////////////////////////////////////////////
void RoundedRectangleShape::SetCornerPointCount(unsigned int count)
{
    myCornerPointCount = count;
    Update();
}

////////////////////////////////////////////////////////////
unsigned int RoundedRectangleShape::GetPointCount() const
{
    return myCornerPointCount*4;
}

////////////////////////////////////////////////////////////
sf::Vector2f RoundedRectangleShape::GetPoint(unsigned int index) const
{
    if(index >= myCornerPointCount*4 || index < 0)
        return sf::Vector2f(0,0);

    float deltaAngle = 90.0f/(myCornerPointCount-1);


    sf::Vector2f center;
    unsigned int centerIndex = index/myCornerPointCount;
    unsigned int offset = 0;
    static const float pi = 3.141592654f;

    switch(centerIndex)
    {
        case 0: center.x = mySize.x - myRadius;     center.y = myRadius;            break;
        case 1: center.x = myRadius;                center.y = myRadius;            break;
        case 2: center.x = myRadius;                center.y = mySize.y - myRadius; break;
        case 3: center.x = mySize.x - myRadius;     center.y = mySize.y - myRadius; break;
    }




    return sf::Vector2f(myRadius*cos(deltaAngle*(index-centerIndex)*pi/180)+center.x,
                        myRadius*sin(deltaAngle*(index-centerIndex)*pi/180)-center.y);
}
}//namespace sf
