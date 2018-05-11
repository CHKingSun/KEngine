//
// Created by KingSun on 2018/4/24.
//

#ifndef KENGINE_KHEADER_H
#define KENGINE_KHEADER_H

#include <cmath>

#define EPSILON_E6 1E-6
#define PI 3.1415926535
#ifdef KNAN
#undef KNAN
#endif
#define KNAN nan("Nan")

namespace KEngine{
    using Kfloat = float;
    using Kdouble = double;
    using Kint = int;
    using Kuint = unsigned int;
    using Ksize = unsigned int;
    using Kchar = char;
    using Kuchar = unsigned char;
    using Kboolean = bool;

    namespace KFunction{
        template <typename T>
        T abs(const T &c){
            return c >= static_cast<T>(0) ? c : -c;
        }
        template <typename T>
        Kboolean isZero(const T &c){
            return abs(c) <= static_cast<T>(EPSILON_E6);
        }
        template <typename T>
        const T& min(const T &a, const T &b){
            return a < b ? a : b;
        }
        template <typename T>
        const T& max(const T &a, const T &b){
            return a > b ? a : b;
        }
        template <typename T>
        T clamp(const T &val, const T &min, const T &max){
            if(max < min) return static_cast<T>(KNAN);
            if(val > max) return max;
            if(val < min) return min;
            return val;
        }
        template <typename T, typename F = Kdouble>
        F toDegree(const T &radian){
            return F(radian) * static_cast<F>(180) / static_cast<F>(PI);
        }
        template <typename T, typename F = Kdouble>
        F toRadian(const T &degree){
            return F(degree) * static_cast<F>(PI) / static_cast<F>(180);
        }
    }

    namespace KVector{
        class Vec2;
        class Vec3;
        class Vec4;
    }

    namespace KMatrix{
        class Mat3;
        class Mat4;
        class Quaternion;
    }

    namespace KColor{
        class Color;
    }

    namespace KLight{
        class Light;
        class PointLight;
        class DirectionLight;
        class SpotLight;
    }

    namespace KCamera{
        class Camera;
    }

    namespace KController{
        class LightController;
        class CameraController;
    }

    namespace KRenderer{
        class Renderer;
        class Shader;
    }

	namespace KBuffer {
		class VertexBuffer;
		class IndexBuffer;
		class VertexArray;
	}

    namespace KMaterial{
        class Material;
        class Texture;
    }

	namespace KObject {
		class Object3D;
	}

    namespace KWindow{
        class Window;
    }
}

#endif //KENGINE_KHEADER_H
