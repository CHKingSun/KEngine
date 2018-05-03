//
// Created by KingSun on 2018/4/26.
//

#ifndef KENGINE_COLOR_H
#define KENGINE_COLOR_H

#include "../KHeader.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"

//typeid
//toFloat toHex toInt_2_10_10_10 toChar
//setHex setInt_2_10_10_10 setChar setFloat setRGB setAlpha
//setHSL toHSL
//BLACK WHITE RED BLUE GREEN YELLOW ORANGE PINK

namespace KEngine{
    namespace KColor{
        using namespace KVector;

        class Color:public Vec4{
        public:
            Color():Color(0){}
            Color(const Vec4 &v):Vec4(v){}
            explicit Color(const Kfloat &c):Vec4(c){}
            Color(const Kfloat &r, const Kfloat &g, const Kfloat &b, const Kfloat &a):Vec4(r, g, b, a){}
            Color(const Vec2 &v1, const Vec2 &v2):Vec4(v1, v2){}
            Color(const Vec3 &v, const Kfloat &a):Vec4(v, a){}

            //0Xaarrggbb
            Color& setFromHex(Kuint hex){
                const Kuint mask = (1<<8) - 1;
                const Kuchar b = hex & mask;
                hex >>= 8;
                const Kuchar g = hex & mask;
                hex >>= 8;
                const Kuchar r = hex & mask;
                hex >>= 8;
                const Kuchar a = hex & mask;

                return setFromChar(r, g, b, a);
            }
            Color& setFromChar(const Kuchar &r, const Kuchar &g,
                            const Kuchar &b, const Kuchar &a){
                const Kfloat tr = static_cast<Kfloat>(r) / static_cast<Kfloat>(255);
                const Kfloat tg = static_cast<Kfloat>(g) / static_cast<Kfloat>(255);
                const Kfloat tb = static_cast<Kfloat>(b) / static_cast<Kfloat>(255);
                const Kfloat ta = static_cast<Kfloat>(a) / static_cast<Kfloat>(255);
                this->set(tr, tg, tb, ta);
                return *this;
            }
            Color& setFromDouble(const Kdouble &r, const Kdouble &g, const Kdouble &b, const Kdouble &a){
                this->set(r, g, b, a);
                return  *this;
            }
            Color& setFromInt_2_10_10_10(Kuint c){
                const Kuint mask = (1 << 10) - 1;
                const Kdouble r = Kdouble(c & mask) / Kdouble(mask);
                c >>= 10;
                const Kdouble g = Kdouble(c & mask) / Kdouble(mask);
                c >>= 10;
                const Kdouble b = Kdouble(c & mask) / Kdouble(mask);
                c >>= 10;
                const Kdouble a = Kdouble(c & mask) / Kdouble(3);
                return setFromDouble(r, g, b, a);
            }

            Kuint toInt_2_10_10_10(){
                Kuint res = 0;
                const Kdouble *rgba = toDouble();
                const Kuint mask = (1 << 10) - 1;
                res += rgba[3] * 3;
                res = static_cast<Kuint>((res << 10) + rgba[0] * mask);
                res = static_cast<Kuint>((res << 10) + rgba[1] * mask);
                res = static_cast<Kuint>((res << 10) + rgba[2] * mask);
                return res;
            }
            const Kdouble* toDouble(){
                auto *res = new Kdouble[4];
                res[0] = static_cast<Kdouble>(this->r);
                res[1] = static_cast<Kdouble>(this->g);
                res[2] = static_cast<Kdouble>(this->b);
                res[3] = static_cast<Kdouble>(this->a);
                return res;
            }
            const Kuchar* toChar(){
                auto *res = new Kuchar[4];
                res[0] = static_cast<Kuchar>(this->r * 255);
                res[1] = static_cast<Kuchar>(this->g * 255);
                res[2] = static_cast<Kuchar>(this->b * 255);
                res[3] = static_cast<Kuchar>(this->a * 255);
                return res;
            }
            Kuint toHex(){
                Kuint res = 0;
                const Kdouble *rgba = toDouble();
                const Kuint mask = (1 << 8) - 1;
                res += rgba[3] * mask;
                res = static_cast<Kuint>((res << 8) + rgba[0] * mask);
                res = static_cast<Kuint>((res << 8) + rgba[1] * mask);
                res = static_cast<Kuint>((res << 8) + rgba[2] * mask);
                return res;
            }

            void setRgb(const Kfloat &r, const Kfloat &g, const Kfloat &b){
                this->set(r, g, b, this->a);
            }
            void setAlpha(const Kfloat &a){
                this->set(this->r, this->g, this->b, a);
            }
        };

        const Color BLACK(0, 0, 0, 1.0f);
        const Color RED(1.0f, 0, 0, 1.0f);
        const Color BLUE(0, 1.0f, 0, 1.0f);
        const Color GREEN(0, 0, 1.0f, 1.0f);
        const Color ORANGE(1.0f, 1.0f, 0, 1.0f);
        const Color YELLOW(0, 1.0f, 1.0f, 1.0f);
        const Color PINK(1.0f, 0, 1.0f, 1.0f);
    }
}

#endif //KENGINE_COLOR_H
