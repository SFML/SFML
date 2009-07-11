////////////////////////////////////////////////////////////
//
// SFGE - Simple and Fast Game Engine
// Copyright (C) 2007-2009 Laurent Gomila (laurent.gom@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
/// Default constructor (builds an identity matrix)
////////////////////////////////////////////////////////////
inline Matrix3::Matrix3()
{
    myData[0] = 1.f; myData[4] = 0.f; myData[8]  = 0.f; myData[12] = 0.f;
    myData[1] = 0.f; myData[5] = 1.f; myData[9]  = 0.f; myData[13] = 0.f;
    myData[2] = 0.f; myData[6] = 0.f; myData[10] = 1.f; myData[14] = 0.f;
    myData[3] = 0.f; myData[7] = 0.f; myData[11] = 0.f; myData[15] = 1.f;
}


////////////////////////////////////////////////////////////
/// Construct a matrix from its 9 elements
////////////////////////////////////////////////////////////
inline Matrix3::Matrix3(float a00, float a01, float a02,
                        float a10, float a11, float a12,
                        float a20, float a21, float a22)
{
    myData[0] = a00; myData[4] = a01; myData[8]  = 0.f; myData[12] = a02;
    myData[1] = a10; myData[5] = a11; myData[9]  = 0.f; myData[13] = a12;
    myData[2] = 0.f; myData[6] = 0.f; myData[10] = 1.f; myData[14] = 0.f;
    myData[3] = a20; myData[7] = a21; myData[11] = 0.f; myData[15] = a22;
}


////////////////////////////////////////////////////////////
/// Build a matrix from a set of transformations
////////////////////////////////////////////////////////////
inline void Matrix3::SetFromTransformations(const Vector2f& origin, const Vector2f& translation, float rotation, const Vector2f& scale)
{
    // Combine the transformations
    float angle  = rotation * 3.141592654f / 180.f;
    float cosine = static_cast<float>(cos(angle));
    float sine   = static_cast<float>(sin(angle));
    float sxCos  = scale.x * cosine;
    float syCos  = scale.y * cosine;
    float sxSin  = scale.x * sine;
    float sySin  = scale.y * sine;
    float tx     = -origin.x * sxCos - origin.y * sySin + translation.x;
    float ty     =  origin.x * sxSin - origin.y * syCos + translation.y;

    // Rebuild the matrix
    myData[0] =  sxCos; myData[4] = sySin; myData[8]  = 0.f; myData[12] = tx;
    myData[1] = -sxSin; myData[5] = syCos; myData[9]  = 0.f; myData[13] = ty;
    myData[2] =  0.f;   myData[6] = 0.f;   myData[10] = 1.f; myData[14] = 0.f;
    myData[3] =  0.f;   myData[7] = 0.f;   myData[11] = 0.f; myData[15] = 1.f;
}


////////////////////////////////////////////////////////////
/// Build a matrix from a projection
////////////////////////////////////////////////////////////
inline void Matrix3::SetFromProjection(const Vector2f& center, const Vector2f& size, float rotation)
{
    // Rotation components
    float angle  = rotation * 3.141592654f / 180.f;
    float cosine = static_cast<float>(cos(angle));
    float sine   = static_cast<float>(sin(angle));
    float tx     = -center.x * cosine - center.y * sine + center.x;
    float ty     =  center.x * sine - center.y * cosine + center.y;

    // Projection components
    float a =  2.f / size.x;
    float b = -2.f / size.y;
    float c = -a * center.x;
    float d = -b * center.y;

    // Rebuild the projection matrix
    myData[0] =  a * cosine; myData[4] = a * sine;   myData[8]  = 0.f; myData[12] = a * tx + c;
    myData[1] = -b * sine;   myData[5] = b * cosine; myData[9]  = 0.f; myData[13] = b * ty + d;
    myData[2] = 0.f;         myData[6] = 0.f;        myData[10] = 1.f; myData[14] = 0.f;
    myData[3] = 0.f;         myData[7] = 0.f;        myData[11] = 0.f; myData[15] = 1.f;
}


////////////////////////////////////////////////////////////
/// Transform a point by the matrix
////////////////////////////////////////////////////////////
inline Vector2f Matrix3::Transform(const Vector2f& point) const
{
    return Vector2f(myData[0] * point.x + myData[4] * point.y + myData[12],
                    myData[1] * point.x + myData[5] * point.y + myData[13]);
}


////////////////////////////////////////////////////////////
/// Return the inverse of the matrix
////////////////////////////////////////////////////////////
inline Matrix3 Matrix3::GetInverse() const
{
    // Compute the determinant
    float det = myData[0] * (myData[15] * myData[5] - myData[7] * myData[13]) -
                myData[1] * (myData[15] * myData[4] - myData[7] * myData[12]) +
                myData[3] * (myData[13] * myData[4] - myData[5] * myData[12]);

    // Compute the inverse if determinant is not zero
    if ((det < -1E-7f) || (det > 1E-7f))
    {
        return Matrix3( (myData[15] * myData[5] - myData[7] * myData[13]) / det,
                       -(myData[15] * myData[4] - myData[7] * myData[12]) / det,
                        (myData[13] * myData[4] - myData[5] * myData[12]) / det,
                       -(myData[15] * myData[1] - myData[3] * myData[13]) / det,
                        (myData[15] * myData[0] - myData[3] * myData[12]) / det,
                       -(myData[13] * myData[0] - myData[1] * myData[12]) / det,
                        (myData[7]  * myData[1] - myData[3] * myData[5])  / det,
                       -(myData[7]  * myData[0] - myData[3] * myData[4])  / det,
                        (myData[5]  * myData[0] - myData[1] * myData[4])  / det);
    }
    else
    {
        return Identity;
    }
}


////////////////////////////////////////////////////////////
/// Return the elements of the matrix as a 4x4,
/// in an array of 16 floats
////////////////////////////////////////////////////////////
inline const float* Matrix3::Get4x4Elements() const
{
    return myData;
}


////////////////////////////////////////////////////////////
/// Operator () overloads to access the matrix elements
////////////////////////////////////////////////////////////
inline float Matrix3::operator ()(unsigned int row, unsigned int column) const
{
    switch (row + column * 3)
    {
        case 0 : return myData[0];
        case 1 : return myData[1];
        case 2 : return myData[3];
        case 3 : return myData[4];
        case 4 : return myData[5];
        case 5 : return myData[7];
        case 6 : return myData[12];
        case 7 : return myData[13];
        case 8 : return myData[15];

        default : return myData[0];
    }
}
inline float& Matrix3::operator ()(unsigned int row, unsigned int column)
{
    switch (row + column * 3)
    {
        case 0 : return myData[0];
        case 1 : return myData[1];
        case 2 : return myData[3];
        case 3 : return myData[4];
        case 4 : return myData[5];
        case 5 : return myData[7];
        case 6 : return myData[12];
        case 7 : return myData[13];
        case 8 : return myData[15];

        default : return myData[0];
    }
}


////////////////////////////////////////////////////////////
/// Operator * overload to multiply two matrices
////////////////////////////////////////////////////////////
inline Matrix3 Matrix3::operator *(const Matrix3& right) const
{
    return Matrix3(myData[0] * right.myData[0]  + myData[4] * right.myData[1]  + myData[12] * right.myData[3],
                   myData[0] * right.myData[4]  + myData[4] * right.myData[5]  + myData[12] * right.myData[7],
                   myData[0] * right.myData[12] + myData[4] * right.myData[13] + myData[12] * right.myData[15],
                   myData[1] * right.myData[0]  + myData[5] * right.myData[1]  + myData[13] * right.myData[3],
                   myData[1] * right.myData[4]  + myData[5] * right.myData[5]  + myData[13] * right.myData[7],
                   myData[1] * right.myData[12] + myData[5] * right.myData[13] + myData[13] * right.myData[15],
                   myData[3] * right.myData[0]  + myData[7] * right.myData[1]  + myData[15] * right.myData[3],
                   myData[3] * right.myData[4]  + myData[7] * right.myData[5]  + myData[15] * right.myData[7],
                   myData[3] * right.myData[12] + myData[7] * right.myData[13] + myData[15] * right.myData[15]);
}


////////////////////////////////////////////////////////////
/// Operator *= overload to multiply-assign two matrices
////////////////////////////////////////////////////////////
inline Matrix3& Matrix3::operator *=(const Matrix3& right)
{
    return *this = *this * right;
}
