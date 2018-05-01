//
// Created by KingSun on 2018/4/22.
//

#ifndef KENGINE_VEC3_H
#define KENGINE_VEC3_H

#include <cassert>
#include <iosfwd>
#include "Vec2.h"
#include "../KHeader.h"
#include "../function.h"

namespace KEngine{
    namespace KVector{
        template <typename T>
        struct Vec3{
            union {
                T values[3];
                struct { T x, y, z; };
                struct { T r, g ,b; };
            };

            Vec3():Vec3(0){}
            Vec3(const Vec3<T> &v):x(v.x), y(v.y), z(v.z){}
            explicit Vec3(const T &c):x(c), y(c), z(c){}
            Vec3(const T &x, const T &y, const T &z):x(x), y(y), z(z){}
            Vec3(const Vec2<T> &v, const T &z):x(v.x), y(v.y), z(z){}
            Vec3(const T &x, const Vec2<T> &v):x(x), y(v.x), z(v.y){}

            inline int dimension()const {
                return 3;
            }
            const T* data()const {
                return this->values;
            }
//            T* data(){
//                return this->values;
//            }

            void set(const T &x, const T &y, const T &z){
                this->x = x;
                this->y = y;
                this->z = z;
            }
            void set(const T &c){
                this->x = c;
                this->y = c;
                this->z = c;
            }
            T& operator[](unsigned int n){
                assert(n >= 0 && n < 3);
                return this->values[n];
            }
            const T& operator[](unsigned int n)const {
                assert(n >= 0 && n < 3);
                return this->values[n];
            }

            Vec3<T>& operator=(const Vec3<T> &v){
                set(v.x, v.y, v.z);
                return *this;
            }
            inline Vec3<T>& operator+=(const Vec3<T> &v){
                this->x += v.x;
                this->y += v.y;
                this->z += v.z;
            }
            inline Vec3<T>& operator-=(const Vec3<T> &v){
                this->x -= v.x;
                this->y -= v.y;
                this->z -= v.z;
            }
            inline Vec3<T>& operator*=(const Vec3<T> &v){
                this->x *= v.x;
                this->y *= v.y;
                this->z *= v.z;
            }

            template <typename C>
            inline Vec3<T>& operator*=(const C &c){
                this->x *= c;
                this->y *= c;
                this->z *= c;
                return *this;
            }
            template <typename C>
            inline Vec3<T>& operator/=(const C &c){
                this->x /= c;
                this->y /= c;
                this->z /= c;
                return *this;
            }

            template <typename F = T>
            F length()const {
                return KFunction::length<T, Vec3, F>(*this);
            }
            bool isZero()const {
                return KCore::isZero(length());
            }
            template <typename F = T>
            F dot(const Vec3<T> &v)const {
                return KFunction::dot<T, Vec3, F>(*this, v);
            }
            Vec3<T>& normalize(){
                //maybe need judging length != 0
                const T len = length();
                if(!KCore::isZero(len)) this->operator/=(len);
                else set(static_cast<T>(KNAN));
                return *this;
            }
            Vec3<T>& cross(const Vec3<T> &v){
                T tx = this->x, ty = this->y, tz = this->z;
                this->x = ty * v.z - v.y * tz;
                this->y = - (tx * v.z - v.x * tz);
                this->z = tx * v.y - v.x * ty;
                return *this;
            }
            template <typename F = T>
            F getAngle(const Vec3<T> &v)const {
                const F len1 = length<F>();
                const F len2 = v.length<F>();
                if(KCore::isZero(len1) || KCore::isZero(len2)) return static_cast<F>(KNAN);

                return F(acos(dot<F>(v) / (len1 * len2)));
            }

            static Vec3<T> cross(const Vec3<T> &v1, const Vec3<T> &v2){
                T tx = v1.x, ty = v1.y, tz = v1.z;
                T vx = v2.x, vy = v2.y, vz = v2.z;
                T rx = ty * vz - vy * tz;
                T ry = - (tx * vz - vx * tz);
                T rz = tx * vy - vx * ty;
                return Vec3<T>(rx, ty, rz);
            }
            template <typename F = T>
            static F getAngle(const Vec3<T> &v1, const Vec3 &v2){
                return v1.getAngle<F>(v2);
            };
        };

        template <typename T>
        Vec3<T> operator-(const Vec3<T> &v){
            return Vec3<T>() -= v;
        }
        template <typename T>
        Vec3<T> operator+(const Vec3<T> &v1, const Vec3<T> &v2){
            return Vec3<T>(v1) += v2;
        }
        template <typename T>
        Vec3<T> operator-(const Vec3<T> &v1, const Vec3<T> &v2){
            return Vec3<T>(v1) -= v2;
        }
        template <typename T>
        Vec3<T> operator*(const Vec3<T> &v1, const Vec3<T> &v2){
            return Vec3<T>(v1) *= v2;
        }
        template <typename T, typename C>
        Vec3<T> operator*(const Vec3<T> &v, const C &c){
            return Vec3<T>(v) *= c;
        };
        template <typename T> //防止与Mat3 * Vec3 发生冲突，不将常数c设置为其他类型
        Vec3<T> operator*(const T &c, const Vec3<T> &v){
            return Vec3<T>(v) *= c;
        };
        template <typename T, typename C>
        Vec3<T> operator/(const Vec3<T> &v, const C &c){
            return Vec3<T>(v) /= c;
        };
        template <typename T>
        std::istream& operator>>(std::istream &is, Vec3<T> &v){
            is>>v.x>>v.y>>v.z;
            return is;
        }
        template <typename T>
        std::ostream& operator<<(std::ostream &os, const Vec3<T> &v){
            os<<v.x<<" "<<v.y<<" "<<v.z;
            return os;
        }
    }
}

#endif //KENGINE_VEC3_H
