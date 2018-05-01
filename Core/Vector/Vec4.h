//
// Created by KingSun on 2018/4/22.
//

#ifndef KENGINE_VEC4_H
#define KENGINE_VEC4_H

#include <cassert>
#include <iosfwd>
#include "../function.h"

namespace KEngine{
    namespace KVector{
        template <typename T>
        struct Vec4{
            union {
                T values[4];
                struct { T x, y, z, w; };
                struct { T r, g, b, a; };
                struct { T s, t, p, q; };
            };

            Vec4():Vec4(0){}
            Vec4(const Vec4<T> &v):x(v.x), y(v.y), z(v.z), w(v.w){}
            explicit Vec4(const T &c):x(c), y(c), z(c), w(c){}
            Vec4(const T &x, const T &y, const T &z, const T &w):x(x), y(y), z(z), w(w){}
            Vec4(const Vec2<T> &v1, const Vec2<T> &v2):x(v1.x), y(v1.y), z(v2.x), w(v2.y){}
            Vec4(const Vec3<T> &v, const T &w):x(v.x), y(v.y), z(v.z), w(w){}

            inline int dimension()const {
                return 4;
            }
            const T* data()const {
                return this->values;
            }

            void set(const T &x, const T &y, const T &z, const T &w){
                this->x = x;
                this->y = y;
                this->z = z;
                this->w = w;
            }
            void set(const T &c){
                this->x = c;
                this->y = c;
                this->z = c;
                this->w = c;
            }
            T& operator[](unsigned int n){
                assert(n >= 0 && n < 4);
                return this->values[n];

            }
            const T& operator[](unsigned int n)const {
                assert(n >= 0 && n < 4);
                return this->values[n];
            }

            Vec4<T>& operator=(const Vec4<T> &v){
                set(v.x, v.y, v.z, v.w);
                return *this;
            }
            inline Vec4<T>& operator+=(const Vec4<T> &v){
                this->x += v.x;
                this->y += v.y;
                this->z += v.z;
                this->w += v.w;
                return *this;
            }
            inline Vec4<T>& operator-=(const Vec4<T> &v){
                this->x -= v.x;
                this->y -= v.y;
                this->z -= v.z;
                this->w -= v.w;
                return *this;
            }
            inline Vec4<T>& operator*=(const Vec4<T> &v){
                this->x *= v.x;
                this->y *= v.y;
                this->z *= v.z;
                this->w *= v.w;
                return *this;
            }

            template <typename C>
            inline Vec4<T>& operator*=(const C &c){
                this->x *= c;
                this->y *= c;
                this->z *= c;
                this->w *= c;
                return *this;
            }
            template <typename C>
            inline Vec4<T>& operator/=(const C &c){
                this->x /= c;
                this->y /= c;
                this->z /= c;
                this->w /= c;
                return *this;
            }

            template <typename F = T>
            F length()const {
                return KFunction::length<T, Vec4, F>(*this);
            }
            template <typename F = T>
            F dot(const Vec4<T> &v)const {
                return KFunction::dot<T, Vec4, F>(*this, v);
            }
            Vec4<T>& normalize(){
                const T len = length();
                if(!KCore::isZero(len)) this->operator/=(len);
                else set(static_cast<T>(KNAN));
                return *this;
            }
        };

        template <typename T>
        Vec4<T> operator-(const Vec4<T> &v){
            return Vec4<T>() -= v;
        }
        template <typename T>
        Vec4<T> operator+(const Vec4<T> &v1, const Vec4<T> &v2){
            return Vec4<T>(v1) += v2;
        }
        template <typename T>
        Vec4<T> operator-(const Vec4<T> &v1, const Vec4<T> &v2){
            return Vec4<T>(v1) -= v2;
        }
        template <typename T>
        Vec4<T> operator*(const Vec4<T> &v1, const Vec4<T> &v2){
            return Vec4<T>(v1) *= v2;
        }
        template <typename T, typename C>
        Vec4<T> operator*(const Vec4<T> &v, const C &c){
            return Vec4<T>(v) *= c;
        }
        template <typename T> //防止与Mat4 * Vec4发生冲突，不将常数c设置为其他类型
        Vec4<T> operator*(const T &c, const Vec4<T> &v){
            return Vec4<T>(v) *= c;
        }
        template <typename T, typename C>
        Vec4<T> operator/(const Vec4<T> &v, const C &c){
            return Vec4<T>(v) /= c;
        }
        template <typename T>
        std::istream& operator>>(std::istream &is, Vec4<T> &v){
            is>>v.x>>v.y>>v.z>>v.w;
            return is;
        }
        template <typename T>
        std::ostream& operator<<(std::ostream &os, const Vec4<T> &v){
            os<<v.x<<" "<<v.y<<" "<<v.z<<" "<<v.w;
            return os;
        }
    }
}
#endif //KENGINE_VEC4_H
