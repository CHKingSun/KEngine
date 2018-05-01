//
// Created by KingSun on 2018/4/26.
//

#ifndef KENGINE_COLOR_H
#define KENGINE_COLOR_H

#include <typeinfo>
#include "../KHeader.h"
#include "Vec4.h"

//typeid
//toFloat toHex toInt_2_10_10_10 toChar
//setHex setInt_2_10_10_10 setChar setFloat setRGB setAlpha
//setHSL toHSL
//BLACK WHITE RED BLUE GREEN YELLOW ORANGE PINK

namespace KEngine{
    namespace KColor{
        using namespace KVector;

        template <typename T = float>
        struct Color:public Vec4<T>{
            Color():Color(0){}
            Color(const Vec4<T> &v):Vec4<T>(v){}
            explicit Color(const T &c):Vec4<T>(c){}
            Color(const T &r, const T &g, const T &b, const T &a):Vec4<T>(r, g, b, a){}
            Color(const Vec2<T> &v1, const Vec2<T> &v2):Vec4<T>(v1, v2){}
            Color(const Vec3<T> &v, const T &a):Vec4<T>(v, a){}

            //0Xaarrggbb
            Color<T>& setHex(unsigned int hex){
                const unsigned int mask = (1<<8) - 1;
                const unsigned char b = hex & mask;
                hex >>= 8;
                const unsigned char g = hex & mask;
                hex >>= 8;
                const unsigned char r = hex & mask;
                hex >>= 8;
                const unsigned char a = hex & mask;

                return setChar(r, g, b, a);
            }
            Color<T>& setChar(const unsigned char &r, const unsigned char &g,
                            const unsigned char &b, const unsigned char &a){
                if(typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char) ||
                        typeid(T) == typeid(int) || typeid(T) == typeid(unsigned int)){
                    this->set(r, g, b, a);
                } else {
                    const T tr = static_cast<T>(r) / static_cast<T>(255);
                    const T tg = static_cast<T>(g) / static_cast<T>(255);
                    const T tb = static_cast<T>(b) / static_cast<T>(255);
                    const T ta = static_cast<T>(a) / static_cast<T>(255);
                    this->set(tr, tg, tb, ta);
                }
                return *this;
            }
            Color<T>& setDouble(const double &r, const double &g, const double &b, const double &a){
                if(typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char) ||
                   typeid(T) == typeid(int) || typeid(T) == typeid(unsigned int) ||
                   typeid(T) == typeid(short) || typeid(T) == typeid(unsigned short)){
                    const T tr = static_cast<T>(r) * static_cast<T>(255);
                    const T tg = static_cast<T>(g) * static_cast<T>(255);
                    const T tb = static_cast<T>(b) * static_cast<T>(255);
                    const T ta = static_cast<T>(a) * static_cast<T>(255);
                    this->set(tr, tg, tb, ta);
                } else {
                    this->set(r, g, b, a);
                }
            }
            Color<T>& setInt_2_10_10_10(unsigned int c){
                const unsigned int mask = (1 << 10) - 1;
                const double r = double(c & mask) / double(mask);
                c >>= 10;
                const double g = double(c & mask) / double(mask);
                c >>= 10;
                const double b = double(c & mask) / double(mask);
                c >>= 10;
                const double a = double(c & mask) / double(mask);
                setDouble(r, g, b, a);
            }

            const unsigned int& toInt_2_10_10_10(){
                unsigned int res = 0;
                const double *rgba = toDouble();
                const unsigned int mask = (1 << 10) - 1;
                res += rgba[3] * mask;
                res = (res << 10) + rgba[0] * mask;
                res = (res << 10) + rgba[1] * mask;
                res = (res << 10) + rgba[2] * mask;
                return res;
            }
            const double* toDouble(){
                double res[4];
                if(typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char) ||
                   typeid(T) == typeid(int) || typeid(T) == typeid(unsigned int) ||
                   typeid(T) == typeid(short) || typeid(T) == typeid(unsigned short)){
                    res[0] = static_cast<double>(this->r) / static_cast<double>(255);
                    res[1] = static_cast<double>(this->g) / static_cast<double>(255);
                    res[2] = static_cast<double>(this->b) / static_cast<double>(255);
                    res[3] = static_cast<double>(this->a) / static_cast<double>(255);
                } else {
                    res[0] = static_cast<double>(this->r);
                    res[1] = static_cast<double>(this->g);
                    res[2] = static_cast<double>(this->b);
                    res[3] = static_cast<double>(this->a);
                }
                return res;
            }
            const unsigned char* toChar(){
                unsigned char res[4];
                if(typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char) ||
                   typeid(T) == typeid(int) || typeid(T) == typeid(unsigned int) ||
                   typeid(T) == typeid(short) || typeid(T) == typeid(unsigned short)){
                    res[0] = static_cast<unsigned char>(this->r);
                    res[1] = static_cast<unsigned char>(this->g);
                    res[2] = static_cast<unsigned char>(this->b);
                    res[3] = static_cast<unsigned char>(this->a);
                } else {
                    res[0] = this->r * 255;
                    res[1] = this->g * 255;
                    res[2] = this->b * 255;
                    res[3] = this->a * 255;
                }
                return res;
            }
            const unsigned int& toHex(){
                unsigned int res = 0;
                const double *rgba = toDouble();
                const unsigned int mask = (1 << 8) - 1;
                res += rgba[3] * mask;
                res = (res << 8) + rgba[0] * mask;
                res = (res << 8) + rgba[1] * mask;
                res = (res << 8) + rgba[2] * mask;
                return res;
            }

            void setRgb(const T &r, const T &g, const T &b){
                this->set(r, g, b, this->a);
            }
            void setAlpha(const T &a){
                this->set(this->r, this->g, this->b, a);
            }
        };

        const Color<> BLACK(0, 0, 0, 1.0f);
        const Color<> RED(1.0f, 0, 0, 1.0f);
        const Color<> BLUE(0, 1.0f, 0, 1.0f);
        const Color<> GREEN(0, 0, 1.0f, 1.0f);
        const Color<> ORANGE(1.0f, 1.0f, 0, 1.0f);
        const Color<> YELLOW(0, 1.0f, 1.0f, 1.0f);
        const Color<> PINK(1.0f, 0, 1.0f, 1.0f);
    }
}

#endif //KENGINE_COLOR_H
