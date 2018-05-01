//
// Created by KingSun on 2018/4/24.
//

#ifndef KENGINE_KHEADER_H
#define KENGINE_KHEADER_H

#define EPSILON_E6 1E-6
#define PI 3.1415926535
#ifdef KNAN
#undef KNAN
#endif
#define KNAN nan("Nan")

namespace KEngine{
    typedef float Kfloat;
    typedef double Kdouble;
    typedef int Kint;
    typedef unsigned int Kuint;
    typedef unsigned int Ksize;
    typedef char Kchar;
    typedef unsigned char Kuchar;
    typedef bool Kboolean;

    namespace KCore{
        template <typename T>
        T kabs(const T &c){
            return c >= static_cast<T>(0) ? c : -c;
        }
        template <typename T>
        Kboolean isZero(const T &c){
            return kabs(c) <= static_cast<T>(EPSILON_E6);
        }
        template <typename T, typename F = Kfloat>
        F toDegree(const T &radian){
            return F(radian) * static_cast<F>(180) / static_cast<F>(PI);
        }
        template <typename T, typename F = Kfloat>
        T toRadian(const T &degree){
            return F(degree) * static_cast<F>(PI) / static_cast<F>(180);
        }
    }

    namespace KVector{
        struct Vec2;
        struct Vec3;
        struct Vec4;
    }

    namespace KMatrix{
        struct Mat3;
        struct Mat4;
    }

    namespace KColor{
        struct Color;
    }

    namespace KLight{
        struct Light;
        struct PointLight;
        struct DirectionLight;
        struct SpotLight;
    }

    namespace KController{
        struct LightController;
    }
}

#endif //KENGINE_KHEADER_H
