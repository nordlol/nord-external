#pragma once

#include <iostream>
#include <numbers>

#include "../../../imgui/imgui.h"

const auto pi = std::numbers::pi_v< float >;

namespace nord::rbx::engine
{
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

        inline const float& operator[]( int i ) const
        {
            return ( ( float* )this )[ i ];
        }

        inline float& operator[]( int i )
        {
            return ( ( float* )this )[ i ];
        }

        inline vector3_t __fastcall operator/( float s ) const
        {
            return *this * ( 1.0f / s );
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

        inline float squaredMagnitude() const
        {
            return x * x + y * y + z * z;
        }

        inline vector3_t direction() const
        {
            const float lenSquared = squaredMagnitude();
            const float invSqrt = 1.0f / sqrtf( lenSquared );
            return vector3_t( x * invSqrt, y * invSqrt, z * invSqrt );
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

    struct vector4_t
    {
        float x, y, z, w;

        vector4_t() = default;
        vector4_t( float x, float y, float z, float w ) : x{ x }, y{ y }, z{ z }, w{ w }
        {
        }

        inline vector4_t( const vector3_t& rkVector, float fW )
        {
            x = rkVector.x;
            y = rkVector.y;
            z = rkVector.z;
            w = fW;
        }

        inline float dot( const vector4_t& rkVector ) const
        {
            return x * rkVector.x + y * rkVector.y + z * rkVector.z + w * rkVector.w;
        }

        inline const float& operator[]( int i ) const
        {
            return ( ( float* )this )[ i ];
        }

        inline float& operator[]( int i )
        {
            return ( ( float* )this )[ i ];
        }

        vector3_t xyz() const
        {
            return vector3_t( x, y, z );
        }
    };

    struct matrix3_t
    {
        float elt[ 3 ][ 3 ];

        vector3_t column( std::uint32_t c )
        {
            return { elt[ 0 ][ c ], elt[ 1 ][ c ], elt[ 2 ][ c ] };
        }

        inline float* operator[]( int i )
        {
            return ( float* )&elt[ i ][ 0 ];
        }

        inline const float* operator[]( int i ) const
        {
            return ( const float* )&elt[ i ][ 0 ];
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
        float elt[ 4 ][ 4 ];

        constexpr auto operator[]( int i ) const noexcept
        {
            return elt[ i ];
        }
        auto operator*( vector3_t mul ) const noexcept
        {
            vector4_t ret;

            ret.x = elt[ 0 ][ 0 ] * mul.x + elt[ 0 ][ 1 ] * mul.y + elt[ 0 ][ 2 ] * mul.z + elt[ 0 ][ 3 ];
            ret.y = elt[ 1 ][ 0 ] * mul.x + elt[ 1 ][ 1 ] * mul.y + elt[ 1 ][ 2 ] * mul.z + elt[ 1 ][ 3 ];
            ret.z = elt[ 2 ][ 0 ] * mul.x + elt[ 2 ][ 1 ] * mul.y + elt[ 2 ][ 2 ] * mul.z + elt[ 2 ][ 3 ];
            ret.w = elt[ 3 ][ 0 ] * mul.x + elt[ 3 ][ 1 ] * mul.y + elt[ 3 ][ 2 ] * mul.z + elt[ 3 ][ 3 ];

            return ret;
        }

        float subDeterminant( int excludeRow, int excludeCol ) const
        {
            // Compute non-excluded row and column indices
            int row[ 3 ];
            int col[ 3 ];

            for ( int i = 0; i < 3; ++i )
            {
                row[ i ] = i;
                col[ i ] = i;

                if ( i >= excludeRow )
                {
                    ++row[ i ];
                }
                if ( i >= excludeCol )
                {
                    ++col[ i ];
                }
            }

            // Compute the first row of cofactors
            float cofactor00 = elt[ row[ 1 ] ][ col[ 1 ] ] * elt[ row[ 2 ] ][ col[ 2 ] ] -
                               elt[ row[ 1 ] ][ col[ 2 ] ] * elt[ row[ 2 ] ][ col[ 1 ] ];

            float cofactor10 = elt[ row[ 1 ] ][ col[ 2 ] ] * elt[ row[ 2 ] ][ col[ 0 ] ] -
                               elt[ row[ 1 ] ][ col[ 0 ] ] * elt[ row[ 2 ] ][ col[ 2 ] ];

            float cofactor20 = elt[ row[ 1 ] ][ col[ 0 ] ] * elt[ row[ 2 ] ][ col[ 1 ] ] -
                               elt[ row[ 1 ] ][ col[ 1 ] ] * elt[ row[ 2 ] ][ col[ 0 ] ];

            // Product of the first row and the cofactors along the first row
            return elt[ row[ 0 ] ][ col[ 0 ] ] * cofactor00 + elt[ row[ 0 ] ][ col[ 1 ] ] * cofactor10 +
                   elt[ row[ 0 ] ][ col[ 2 ] ] * cofactor20;
        }

        matrix4_t cofactor() const
        {
            matrix4_t out;

            // We'll use i to incrementally compute -1 ^ (r+c)
            int i = 1;

            for ( int r = 0; r < 4; ++r )
            {
                for ( int c = 0; c < 4; ++c )
                {
                    // Compute the determinant of the 3x3 submatrix
                    float det = subDeterminant( r, c );
                    out.elt[ r ][ c ] = i * det;
                    i = -i;
                }
                i = -i;
            }

            return out;
        }

        const vector4_t& row( int r ) const
        {
            return reinterpret_cast< const vector4_t* >( elt[ r ] )[ 0 ];
        }

        vector4_t column( int c ) const
        {
            vector4_t v;
            for ( int r = 0; r < 4; ++r )
            {
                v[ r ] = elt[ r ][ c ];
            }
            return v;
        }

        float determinant() const
        {
            // Determinant is the dot product of the first row and the first row
            // of cofactors (i.e. the first col of the adjoint matrix)
            return cofactor().row( 0 ).dot( row( 0 ) );
        }

        matrix4_t transpose() const
        {
            matrix4_t result;
            for ( int r = 0; r < 4; ++r )
            {
                for ( int c = 0; c < 4; ++c )
                {
                    result.elt[ c ][ r ] = elt[ r ][ c ];
                }
            }

            return result;
        }

        matrix4_t adjoint() const
        {
            return cofactor().transpose();
        }

        matrix4_t operator*( const float s ) const
        {
            matrix4_t result;
            for ( int r = 0; r < 4; ++r )
            {
                for ( int c = 0; c < 4; ++c )
                {
                    result.elt[ r ][ c ] = elt[ r ][ c ] * s;
                }
            }

            return result;
        }

        matrix4_t inverse() const
        {
            // Inverse = adjoint / determinant

            matrix4_t A = adjoint();

            // Determinant is the dot product of the first row and the first row
            // of cofactors (i.e. the first col of the adjoint matrix)
            float det = A.column( 0 ).dot( row( 0 ) );

            return A * ( 1.0f / det );
        }

        vector4_t operator*( const vector4_t& vector ) const
        {
            vector4_t result( 0, 0, 0, 0 );
            for ( int r = 0; r < 4; ++r )
            {
                for ( int c = 0; c < 4; ++c )
                {
                    result[ r ] += elt[ r ][ c ] * vector[ c ];
                }
            }

            return result;
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

        inline vector3_t point_to_object_space( const vector3_t& v ) const
        {
            float p[ 3 ];
            p[ 0 ] = v[ 0 ] - translation[ 0 ];
            p[ 1 ] = v[ 1 ] - translation[ 1 ];
            p[ 2 ] = v[ 2 ] - translation[ 2 ];

            return { rotation[ 0 ][ 0 ] * p[ 0 ] + rotation[ 1 ][ 0 ] * p[ 1 ] + rotation[ 2 ][ 0 ] * p[ 2 ],
                     rotation[ 0 ][ 1 ] * p[ 0 ] + rotation[ 1 ][ 1 ] * p[ 1 ] + rotation[ 2 ][ 1 ] * p[ 2 ],
                     rotation[ 0 ][ 2 ] * p[ 0 ] + rotation[ 1 ][ 2 ] * p[ 1 ] + rotation[ 2 ][ 2 ] * p[ 2 ] };
        }
    };
}  // namespace nord::rbx::engine