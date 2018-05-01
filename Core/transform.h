//
// Created by KingSun on 2018/4/23.
//

#ifndef KENGINE_TRANSFORM_H
#define KENGINE_TRANSFORM_H

//translate rotate scale
//lookAt perspective ortho frustum
//project unproject(vec3 obj, mat4 model, mat4 proj, vec4 viewport)

#include <cassert>
#include <cmath>
#include "KHeader.h"
#include "./Vector/Vec3.h"
#include "./Matrix/Mat4.h"

namespace KEngine{
    namespace KFunction{
        using namespace KVector;
        using namespace KMatrix;

        template <typename T>
        Mat4<T> translate(const Vec3<T> &v){
            return Mat4<T>(
                    1, 0, 0, v.x,
                    0, 1, 0, v.y,
                    0, 0, 1, v.z,
                    0, 0, 0, 1
            );
        }

        template <typename T>
        Mat4<T> rotate(const T &angle, const Vec3<T> &v){
            const T cos = static_cast<T>(cos(angle));
            const T sin = static_cast<T>(sin(angle));

            Vec3<T> axis(normalize(v));
            Vec3<T> tmp((static_cast<T>(1) - cos) * v);
            Mat4<T> m;

            m[0][0] = cos + axis.x * tmp.x;
            m[0][1] = axis.x * tmp.y - axis.z * sin;
            m[0][2] = axis.x * tmp.z + axis.y * sin;

            m[1][0] = axis.y * tmp.x + axis.z * sin;
            m[1][1] = cos + axis.y * tmp.y;
            m[1][2] = axis.y * tmp.z - axis.x * sin;

            m[2][0] = axis.z * tmp.x - axis.y * sin;
            m[2][1] = axis.z * tmp.y + axis.x * sin;
            m[2][2] = cos + axis.z * tmp.z;

            return m;
        }

        template <typename T>
        Mat4<T> scale(const Vec3<T> &v){
            return Mat4<T>(
                    v.x, 0, 0, 0,
                    0, v.y, 0, 0,
                    0, 0, v.z, 0,
                    0, 0, 0, 1
            );
        }

        template <typename T>
        Mat4<T> lookAt(const Vec3<T> &eye, const Vec3<T> &center, const Vec3<T> &up){
            const Vec3<T> z((center - eye).normalize());
            const Vec3<T> x(Vec3<T>::cross(z, up).normalize());
            const Vec3<T> y(Vec3<T>::cross(x, z).normalize());

            return Mat4<T>(
                    x.x, x.y, x.z, -dot(x, z),
                    y.x, y.y, y.z, -dot(y, z),
                    -z.x, -z.y, -z.z, dot(z, z),
                    0, 0, 0, 1
            );
        }

        template <typename T>
        Mat4<T> ortho(const T &left, const T &right, const T &bottom,
                      const T &top, const T &near, const T &far){
            Mat4<T> m;

            m[0][0] = static_cast<T>(2) / (right - left);
            m[1][1] = static_cast<T>(2) / (top - bottom);
            m[2][2] = static_cast<T>(2) / (far - near);

            m[0][3] = -(right + left) / (right - left);
            m[1][3] = -(top - bottom) / (top - bottom);
            m[2][3] = -(far + near) / (far - near);

            return m;
        }

        template <typename T>
        Mat4<T> frustum(const T &left, const T &right, const T &bottom,
                      const T &top, const T &near, const T &far){
            Mat4<T> m(0);

            m[0][0] = static_cast<T>(2) * near / (right - left);
            m[1][1] = static_cast<T>(2) * near / (top - bottom);
            m[0][2] = -(right + left) / (right - left);
            m[1][2] = -(top - bottom) / (top - bottom);

            m[2][2] = -(far + near) / (far - near);
            m[2][3] = static_cast<T>(-2) * far * near / (far - near);
            m[3][2] = static_cast<T>(-1);

            return m;
        }

        template <typename T>
        Mat4<T> perspective(const T &fovy, const T &aspect, const T &zNear, const T &zFar){
            assert((aspect > static_cast<T>(1E-6)) && ((zFar - zNear) > static_cast<T>(1E-6)));

            const T cot = static_cast<T>(1) / tan(fovy / 2.0);
            Mat4<T> m(0);

            m[0][0] = cot / aspect;
            m[1][1] = cot;
            m[2][2] = -(zFar + zNear) / (zFar - zNear);
            m[2][3] = static_cast<T>(-2) * zFar * zNear / (zFar - zNear);
            m[3][2] = static_cast<T>(-1);

            return m;
        }

        template <typename T, typename I = T>
        Vec3<T> project(const Vec3<T> &v, const Mat4<T> &model, const Mat4<T> &proj, const Vec4<I> &viewport){
            Vec4<T> pos = proj * model * Vec4<T>(v, static_cast<T>(1)); //裁剪坐标系
            pos /= pos.w; //转换到[-1, 1]
            pos /= static_cast<T>(2); //转换到[-0.5, 0.5]
            pos += static_cast<T>(0.5); //转换到[0, 1]

            pos.x = pos.x * T(viewport[2]) + T(viewport[0]); //转换到[v[0], v[2]]
            pos.y = pos.y * T(viewport[3]) + T(viewport[1]); //转换到[v[1], v[3]]

            return Vec3<T>(pos.x, pos.y, pos.z);
        };

        template <typename T, typename I = T>
        Vec3<T> unProject(const Vec3<T> &v, const Mat4<T> &model, const Mat4<T> &proj, const Vec4<I> &viewport){
            Vec4<T> pos = Vec4<T>(v, static_cast<T>(1));
            pos.x = (pos.x - T(viewport[0])) / T(viewport[2]); //转换到[0, 1]
            pos.y = (pos.y - T(viewport[1])) / T(viewport[3]); //转换到[0, 1]
            pos *= static_cast<T>(2); //[0, 2]
            pos -= static_cast<T>(1); //[-1, 1]

            pos = (proj * model).inverse() * pos;
            pos /= pos.w;

            return Vec3<T>(pos.x, pos.y, pos.z);
        };
    }
}

#endif //KENGINE_TRANSFORM_H
