//
//  core.h
//  PhysicsEngine
//
//  Created by Vishal Patel on 10/06/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//
#include "precision.h"

namespace  Cyclone {
    
    
    class Vector3 {
      
        public:
            real x ;
        
            real y ;
        
            real z ;
        
        private:
        
            real pad;
        
        public:
        
        Vector3(): x(0),y(0),z(0) {
            
        }
        
        
        Vector3 ( const real x,const  real y, const real z ): x(x), y(y), z(z) {
            
        }
        
        void Invert() {
            x = -x;
            y = -y;
            z = -z;
        }
        
        
        real magnitude() const {
            
            return  real_sqrt ( x*x + y*y + z*z );
        }
        
        real SqureMegnitude() const {
            
            return  x*x + y*y + z*z;
        
        }
        
        void operator *=(const real value ) {
            
            x *= value;
            y *= value;
            z *= value;
            
        }
        
        void Normalise() {
            
           real l = magnitude();
            
            (*this)*= ((real) 1)/ l ; 
            
        }
        
        Vector3 operator * ( const real value ) const {
            
            return Vector3( x* value , y *value , z*value); 
        }
        
        
        void operator += ( const Vector3& vect ) {
            
            x += vect.x ;
            y += vect.y ;
            z += vect.z ;
            
        }
        
        Vector3 operator + ( const Vector3& vect ) {
            
            return  Vector3( x + vect.x , y+ vect.y , z+vect.z);
        }

        void operator -= ( const Vector3& vect ) {
            
            x -= vect.x ;
            y -= vect.y ;
            z -= vect.z ;
            
        }
        
        Vector3 operator - ( const Vector3& vect ) {
            
            return  Vector3( x - vect.x , y - vect.y , z - vect.z);
        }
        
        void addScaledVector ( const Vector3& vect, const real scale ) {
            
            x += vect.x * scale ;
            y += vect.y * scale ;
            z += vect.z * scale ;
        }
        
        /**
         * Calculates and returns a component-wise product of this
         * vector with the given vector.
         */
        Vector3 componentProduct(const Vector3 &vector) const
        {
            return Vector3(x * vector.x, y * vector.y, z * vector.z);
        }
        
        /**
         * Performs a component-wise product with the given vector and
         * sets this vector to its result.
         */
        void componentProductUpdate(const Vector3 &vector)
        {
            x *= vector.x;
            y *= vector.y;
            z *= vector.z;
        }
        
        /**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 vectorProduct(const Vector3 &vector) const
        {
            return Vector3(y*vector.z-z*vector.y,
                           z*vector.x-x*vector.z,
                           x*vector.y-y*vector.x);
        }
        
        /**
         * Updates this vector to be the vector product of its current
         * value and the given vector.
         */
        void operator %=(const Vector3 &vector)
        {
            *this = vectorProduct(vector);
        }
        
        /**
         * Calculates and returns the vector product of this vector
         * with the given vector.
         */
        Vector3 operator%(const Vector3 &vector) const
        {
            return Vector3(y*vector.z-z*vector.y,
                           z*vector.x-x*vector.z,
                           x*vector.y-y*vector.x);
        }
        
        /**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        real scalarProduct(const Vector3 &vector) const
        {
            return x*vector.x + y*vector.y + z*vector.z;
        }
        
        /**
         * Calculates and returns the scalar product of this vector
         * with the given vector.
         */
        real operator *(const Vector3 &vector) const
        {
            return x*vector.x + y*vector.y + z*vector.z;
        }
        
        
        /** Gets the squared magnitude of this vector. */
        real squareMagnitude() const
        {
            return x*x+y*y+z*z;
        }
        
        /** Limits the size of the vector to the given maximum. */
        void trim(real size)
        {
            if (squareMagnitude() > size*size)
            {
                Normalise();
                x *= size;
                y *= size;
                z *= size;
            }
        }
        
        
        /** Checks if the two vectors have identical components. */
        bool operator==(const Vector3& other) const
        {
            return x == other.x &&
            y == other.y &&
            z == other.z;
        }
        
        /** Checks if the two vectors have non-identical components. */
        bool operator!=(const Vector3& other) const
        {
            return !(*this == other);
        }
        
        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator<(const Vector3& other) const
        {
            return x < other.x && y < other.y && z < other.z;
        }
        
        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a < b) does not imply (b >= a).
         */
        bool operator>(const Vector3& other) const
        {
            return x > other.x && y > other.y && z > other.z;
        }
        
        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator<=(const Vector3& other) const
        {
            return x <= other.x && y <= other.y && z <= other.z;
        }
        
        /**
         * Checks if this vector is component-by-component less than
         * the other.
         *
         * @note This does not behave like a single-value comparison:
         * !(a <= b) does not imply (b > a).
         */
        bool operator>=(const Vector3& other) const
        {
            return x >= other.x && y >= other.y && z >= other.z;
        }
        
        /** Zero all the components of the vector. */
        void clear()
        {
            x = y = z = 0;
        }
        
    };
}