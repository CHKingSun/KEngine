//
// Created by KingSun on 2018/4/21.
//

#ifndef KENGINE_VEC2_H
#define KENGINE_VEC2_H

#include <cassert>
#include <iosfwd>
#include "../function.h"

namespace KEngine{
    namespace KVector{
        template <typename T>
        struct Vec2{
            union {
                T values[2];
                struct { T x, y; };
                struct { T s, t; };
            };

            Vec2():Vec2(0){}
            Vec2(const Vec2<T> &v):x(v.x), y(v.y){}
            explicit Vec2(const T &c):x(c), y(c){}
            Vec2(const T &x, const T &y):x(x), y(y){}

            inline int dimension()const {
                return 2;
            }
            const T* data()const{
                return this->values;
            }
//            T* data(){
//                return this->values;
//            }

            void set(const T &x, const T &y){
                this->x = x;
                this->y = y;
            }
            void set(const T &c){
                this->x = c;
                this->y = c;
            }
            T& operator[](unsigned int n){
                assert(n >= 0 && n < 2);
                return this->values[n]; //return reference, for assign
            }
            const T& operator[](unsigned int n)const{
                assert(n >= 0 && n < 2);
                return this->values[n]; //for const object, just get value.
            }

            Vec2<T>& operator=(const Vec2<T> &v){
                set(v.x, v.y);
                return *this;
            }
            inline Vec2<T>& operator+=(const Vec2<T> &v){
                this->x += v.x;
                this->y += v.y;
                return *this;
            }
            inline Vec2<T>& operator-=(const Vec2<T> &v){
                this->x -= v.x;
                this->y -= v.y;
                return *this;
            }
            inline Vec2<T>& operator*=(const Vec2<T> &v){
                this->x *= v.x;
                this->y *= v.y;
                return *this;
            }

            template <typename C>
            inline Vec2<T>& operator/=(const C &c){
                this->x /= c;
                this->y /= c;
                return *this;
            }
            template <typename C>
            inline Vec2<T>& operator*=(const C &c){
                this->x *= c;
                this->y *= c;
                return *this;
            }

            template <typename F = T>
            F length()const {
                return KFunction::length<T, Vec2, F>(*this);
            }
            template <typename F = T>
            F dot(const Vec2<T> &v)const {
                return KFunction::dot<T, Vec2, F>(*this, v);
            }
            Vec2<T>& normalize(){
                const T len = length();
                if(!KCore::isZero(len)) this->operator/=(len);
                else this->set(static_cast<T>(KNAN));
                return *this;
            }
        };

        template <typename T>
        Vec2<T> operator-(const Vec2<T> &v){
            return Vec2<T>() -= v;
        }
        template <typename T>
        Vec2<T> operator+(const Vec2<T> &v1, const Vec2<T> &v2){
            return Vec2<T>(v1) += v2;
        }
        template <typename T>
        Vec2<T> operator-(const Vec2<T> &v1, const Vec2<T> &v2){
            return Vec2<T>(v1) -= v2;
        }
        template <typename T>
        Vec2<T> operator*(const Vec2<T> &v1, const Vec2<T> &v2){
            return Vec2<T>(v1) *= v2;
        }
        template <typename T, typename C>
        Vec2<T> operator*(const Vec2<T> &v, const C &c){
            return Vec2<T>(v) *= c;
        };
        template <typename T>
        Vec2<T> operator*(const T &c, const Vec2<T> &v){
            return Vec2<T>(v) *= c;
        };
        template <typename T, typename C>
        Vec2<T> operator/(const Vec2<T> &v, const C &c){
            return Vec2<T>(v) /= c;
        };
        template <typename T>
        std::istream& operator>>(std::istream &is, Vec2<T> &v){
            is>>v.x>>v.y;
            return is;
        }
        template <typename T>
        std::ostream& operator<<(std::ostream &os, const Vec2<T> &v){
            os<<v.x<<" "<<v.y;
            return os;
        }
    }
}


#endif //KENGINE_VEC2_H
