//
// Created by KingSun on 2018/5/1.
//

#ifndef KENGINE_CAMERA_H
#define KENGINE_CAMERA_H

#include "../../KHeader.h"
#include "../Vector/Vec3.h"
#include "../Matrix/Mat4.h"
#include "../transform.h"
#include "../Matrix/Quaternion.h"

//会频繁的进行移动
//需要把位置传入(光照计算)
//需要频繁的进行整体旋转

namespace KEngine{
    namespace KCamera{
        class Camera{
//            using tvec2 = KVector::Vec2;
//            using tvec4 = KVector::Vec4;
			using tvec3 = KVector::Vec3;
            using tmat3 = KMatrix::Mat3;
            using tquaternion = KMatrix::Quaternion;
            using tmat4 = KMatrix::Mat4;

        private:
            tvec3 position;
            tquaternion view; //view rotation
            tmat4 projection;
            tquaternion rotate; //camera rotation
            tmat4 viewMatrix;
//            tvec4 viewport; //reserved
//            tvec2 zdepth; //reserved

        public:
            explicit Camera(const tvec3 &pos = tvec3()): position(pos),
                     view(tquaternion()), projection(tmat4()), rotate(tquaternion()){
                updateViewMatrix();
            } //ortho
            Camera(const tvec3 &eye, const tvec3 &center, const tvec3 &up):
                    projection(tmat4()), rotate(tquaternion()){
                setView(eye, center, up);
            }
            Camera(const Kfloat &fovy, const Kfloat &aspect,
                   const Kfloat &zNear, const Kfloat &zFar,
                   const tvec3 &pos = tvec3()):
                    position(pos), view(tquaternion()), rotate(tquaternion()){
                setPerspective(fovy, aspect, zNear, zFar);
                updateViewMatrix();
            }

            const tvec3 &getPosition()const {
                return position;
            }
            const tmat4 &getView()const {
                return viewMatrix;
            }
            const tmat4 &getProjection()const {
                return projection;
            }

            void updateViewMatrix(){
                tmat3 tmp = view.toMat3();
                viewMatrix = rotate.toMat4() * tmat4(tmp, (tmp * -position));
            }

            Camera& setPosition(const tvec3 &v){
                position = v;
                updateViewMatrix();
                return *this;
            }
            Camera& setView(const tvec3 &eye, const tvec3 &center, const tvec3 &up){
                //u-v-n is left-hand coordinate
                const tvec3 n((center - eye).normalize());
                const tvec3 u(tvec3::cross(n, up).normalize());
                const tvec3 v(tvec3::cross(u, n).normalize());

                position = eye;
                view = tquaternion().fromMatrix(tmat3(u, v, -n));
                updateViewMatrix();
                return *this;
            }
            Camera& setPerspective(const Kfloat &fovy, const Kfloat &aspect,
                                   const Kfloat &zNear, const Kfloat &zFar){
                projection = KFunction::perspective(fovy, aspect, zNear, zFar);
//                zdepth = tvec2(zNear, zFar);
                return *this;

            }
            Camera& setOrtho(const Kfloat &left, const Kfloat &right, const Kfloat &bottom,
                             const Kfloat &top, const Kfloat &near, const Kfloat &far){
                projection = KFunction::ortho(left, right, bottom, top, near, far);
                return *this;
            }
            Camera& setFrustum(const Kfloat &left, const Kfloat &right, const Kfloat &bottom,
                               const Kfloat &top, const Kfloat &near, const Kfloat &far){
                projection = KFunction::frustum(left, right, bottom, top, near, far);
                return *this;
            }

            void rotateCamera(const Kfloat &angle, const tvec3 &v){
                rotate = tquaternion(angle, v) * rotate;
                updateViewMatrix();
            }
            void rotateView(const Kfloat &angle, const tvec3 &v){
                //note: view is a inverse rotate matrix(also transpose matrix),
                //that means once you want to add rotate to it,
                //you should right multiply a transpose rotate matrix
                //(rot * originView).inverse = originView.inverse * rot.inverse = view * rot.transpose
                view *= tquaternion(angle, -v); //view *= tquaternion(angle, v).getConjugate();
                updateViewMatrix();
            }

            void translate(const tvec3 &v){
                position += v;
                updateViewMatrix();
            }
        };
    }
}

#endif //KENGINE_CAMERA_H
