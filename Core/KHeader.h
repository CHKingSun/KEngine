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
    namespace KCore{
        template <typename T>
        T kabs(const T &c){
            return c >= static_cast<T>(0) ? c : -c;
        }
        template <typename T>
        bool isZero(const T &c){
            return kabs(c) <= static_cast<T>(EPSILON_E6);
        }
        template <typename T, typename F = float>
        F toDegree(const T &radian){
            return F(radian) * static_cast<F>(180) / static_cast<F>(PI);
        }
        template <typename T, typename F = float>
        T toRadian(const T &degree){
            return F(degree) * static_cast<F>(PI) / static_cast<F>(180);
        }
    }

    namespace KVector{
        template <typename> struct Vec2;
        template <typename> struct Vec3;
        template <typename> struct Vec4;
    }

    namespace KMatrix{
        template <typename> struct Mat3;
        template <typename> struct Mat4;
    }

    namespace KColor{
        template <typename> struct Color; //声明时不要有T = float
    }

    namespace KLight{
        template <typename> struct Light;
        template <typename> struct PointLight;
        template <typename> struct DirectionLight;
        template <typename> struct SpotLight;
    }

    namespace KController{
        template <typename> struct LightController;
    }

    typedef KVector::Vec2<float> vec2;
    typedef KVector::Vec3<float> vec3;
    typedef KVector::Vec4<float> vec4;
    typedef KMatrix::Mat3<float> mat3;
    typedef KMatrix::Mat4<float> mat4;

    typedef KColor::Color<float> color;

    typedef KVector::Vec2<double> dvec2;
    typedef KVector::Vec3<double> dvec3;
    typedef KVector::Vec4<double> dvec4;
    typedef KMatrix::Mat3<double> dmat3;
    typedef KMatrix::Mat4<double> dmat4;
}

#endif //KENGINE_KHEADER_H
