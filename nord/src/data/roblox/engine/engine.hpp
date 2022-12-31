#pragma once

#include <iostream>
#include <numbers>

#include "../../../imgui/imgui.h"

const auto pi = std::numbers::pi_v< float >;

namespace nord::rbx::engine
{
    struct vector4_t
    {
        float x, y, z, w;

        vector4_t() = default;
        vector4_t( float x, float y, float z, float w ) : x{ x }, y{ y }, z{ z }, w{ w }
        {
        }
    };

    struct vector2_t
    {
        float x, y;

        bool empty()
        {
            return x == 0.f && y == 0.f;
        }

        const float distance( const vector2_t& vector ) const
        {
            return std::sqrtf( std::powf( x - vector.x, 2.0 ) + std::powf( y - vector.y, 2.0 ) );
        }

        vector2_t operator-( const vector2_t& vec ) const
        {
            return { x - vec.x, y - vec.y };
        }

        vector2_t operator+( const vector2_t& vec ) const
        {
            return { x + vec.x, y + vec.y };
        }

        vector2_t operator*( float val ) const
        {
            return { x * val, y * val };
        }

        vector2_t operator/( float val ) const
        {
            return { x / val, y / val };
        }

        float magnitude()
        {
            return std::sqrtf( ( x * x ) + ( y * y ) );
        }

        float get_angle_dist( const vector2_t& angle ) const
        {
            vector2_t diff = *this - angle;
            if ( diff.x < -180.f )
                diff.x += 360.f;
            if ( diff.x > 180.f )
                diff.x -= 360.f;
            return fabsf( diff.magnitude() );
        }

        operator ImVec2()
        {
            return ImVec2{ x, y };
        }
    };

    struct vector3_t
    {
        float x, y, z;

        vector3_t( float x, float y, float z ) : x( x ), y( y ), z( z )
        {
        }

        vector3_t() : x( 0 ), y( 0 ), z( 0 )
        {
        }

        const float dot( const vector3_t& vec ) const
        {
            return x * vec.x + y * vec.y + z * vec.z;
        }

        const float distance( vector3_t vector ) const
        {
            return sqrtf( powf( vector.x - x, 2.0 ) + powf( vector.y - y, 2.0 ) + powf( vector.z - z, 2.0 ) );
        }

        const vector3_t operator*( float value ) const
        {
            return { x * value, y * value, z * value };
        }

        const float squared() const
        {
            return x * x + y * y + z * z;
        }

        vector3_t operator-( const vector3_t& vec ) const
        {
            return { x - vec.x, y - vec.y, z - vec.z };
        }

        vector3_t operator+( const vector3_t& vec ) const
        {
            return { x + vec.x, y + vec.y, z + vec.z };
        }

        vector3_t operator*( const vector3_t& vec ) const
        {
            return { x * vec.x, y * vec.y, z * vec.z };
        }

        vector3_t operator/( const vector3_t& vec ) const
        {
            return { x / vec.x, y / vec.y, z / vec.z };
        }

        float magnitude() const
        {
            return std::sqrtf( ( x * x ) + ( y * y ) + ( z * z ) );
        }

        vector3_t normalized() const
        {
            float len = magnitude();
            return vector3_t( x / len, y / len, z / len );
        }

        bool empty() const
        {
            return x == 0.f && y == 0.f && z == 0.f;
        }

        vector2_t get_angle_to( const vector3_t& dest ) const
        {
            vector3_t diff = *this - dest;
            vector2_t ret;

            float length = diff.magnitude();
            ret.y = asinf( diff.y / length );
            ret.x = -atan2f( diff.x, -diff.z );

            return ret * ( 180.f / pi );
        }

        vector3_t operator-() const
        {
            return { -x, -y, -z };
        }
    };

    struct matrix3_t
    {
        float elt[ 3 ][ 3 ];

        vector3_t column( std::uint32_t c )
        {
            return { elt[ 0 ][ c ], elt[ 1 ][ c ], elt[ 2 ][ c ] };
        }

        bool toEulerAnglesXYZ( float& rfXAngle, float& rfYAngle, float& rfZAngle ) const
        {
            // rot =  cy*cz          -cy*sz           sy
            //        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
            //       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

            if ( elt[ 0 ][ 2 ] < 1.0f )
            {
                if ( elt[ 0 ][ 2 ] > -1.0f )
                {
                    rfXAngle = std::atan2f( -elt[ 1 ][ 2 ], elt[ 2 ][ 2 ] );
                    rfYAngle = std::sinf( elt[ 0 ][ 2 ] );
                    rfZAngle = std::atan2f( -elt[ 0 ][ 1 ], elt[ 0 ][ 0 ] );
                    return true;
                }
                else
                {
                    // WARNING.  Not unique.  XA - ZA = -atan2(r10,r11)
                    rfXAngle = -std::atan2f( elt[ 1 ][ 0 ], elt[ 1 ][ 1 ] );
                    rfYAngle = -( pi / 2 );
                    rfZAngle = 0.0f;
                    return false;
                }
            }
            else
            {
                // WARNING.  Not unique.  XAngle + ZAngle = atan2(r10,r11)
                rfXAngle = std::atan2f( elt[ 1 ][ 0 ], elt[ 1 ][ 1 ] );
                rfYAngle = pi / 2;
                rfZAngle = 0.0f;
                return false;
            }
        }

        bool toEulerAnglesYXZ( float& rfYAngle, float& rfXAngle, float& rfZAngle ) const
        {
            // rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
            //        cx*sz           cx*cz          -sx
            //       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

            if ( elt[ 1 ][ 2 ] < 1.0 )
            {
                if ( elt[ 1 ][ 2 ] > -1.0 )
                {
                    rfYAngle = std::atan2f( elt[ 0 ][ 2 ], elt[ 2 ][ 2 ] );
                    rfXAngle = ( float )asin( -elt[ 1 ][ 2 ] );
                    rfZAngle = std::atan2f( elt[ 1 ][ 0 ], elt[ 1 ][ 1 ] );
                    return true;
                }
                else
                {
                    // WARNING.  Not unique.  YA - ZA = atan2(r01,r00)
                    rfYAngle = std::atan2f( elt[ 0 ][ 1 ], elt[ 0 ][ 0 ] );
                    rfXAngle = pi / 2;
                    rfZAngle = 0.0;
                    return false;
                }
            }
            else
            {
                // WARNING.  Not unique.  YA + ZA = atan2(-r01,r00)
                rfYAngle = std::atan2f( -elt[ 0 ][ 1 ], elt[ 0 ][ 0 ] );
                rfXAngle = -( pi / 2 );
                rfZAngle = 0.0f;
                return false;
            }
        }
    };

    struct matrix4_t
    {
        float mat[ 4 ][ 4 ];

        constexpr auto operator[]( int i ) const noexcept
        {
            return mat[ i ];
        }
        auto operator*( vector3_t mul ) const noexcept
        {
            vector4_t ret;

            ret.x = mat[ 0 ][ 0 ] * mul.x + mat[ 0 ][ 1 ] * mul.y + mat[ 0 ][ 2 ] * mul.z + mat[ 0 ][ 3 ];
            ret.y = mat[ 1 ][ 0 ] * mul.x + mat[ 1 ][ 1 ] * mul.y + mat[ 1 ][ 2 ] * mul.z + mat[ 1 ][ 3 ];
            ret.z = mat[ 2 ][ 0 ] * mul.x + mat[ 2 ][ 1 ] * mul.y + mat[ 2 ][ 2 ] * mul.z + mat[ 2 ][ 3 ];
            ret.w = mat[ 3 ][ 0 ] * mul.x + mat[ 3 ][ 1 ] * mul.y + mat[ 3 ][ 2 ] * mul.z + mat[ 3 ][ 3 ];

            return ret;
        }
    };

    struct cframe_t
    {
        matrix3_t rotation;
        vector3_t translation;

        vector3_t look_vector()
        {
            return -rotation.column( 2 );
        }
    };
}  // namespace nord::rbx