//
// Created by KingSun on 2018/5/1.
//

#ifndef KENGINE_CAMERA_H
#define KENGINE_CAMERA_H

#include <string>
#include <memory>
#include "../../KHeader.h"
#include "../Vector/Vec3.h"
#include "../Matrix/Mat4.h"
#include "../transform.h"
#include "../Matrix/Quaternion.h"
#include "../../Render/Buffer/UniformBlock.h"

//一切对镜头的操作都是逆操作
//会频繁的进行移动
//需要把位置传入(光照计算)
//需要频繁的进行整体旋转

namespace KEngine{
    namespace KCamera{
		enum DirectionType {
			RIGHT = GLFW_KEY_RIGHT, LEFT = GLFW_KEY_LEFT, BACK = GLFW_KEY_DOWN, FORWARD = GLFW_KEY_UP
		};

		class Camera {
			using tvec3 = KVector::Vec3;
			using tmat3 = KMatrix::Mat3;
			using tquaternion = KMatrix::Quaternion;
			using tmat4 = KMatrix::Mat4;

		protected:
            tvec3 position;
            tquaternion view; //view rotation
            tmat4 projection;
            tquaternion rotate; //camera rotation

			static std::shared_ptr<KBuffer::UnifromBlock> block;

			const static std::string PROJECTION; //porjection block name
			const static std::string EYE; //p_eye
			const static std::string VIEW; //u_view
			const static std::string PROJ; //u_proj

			virtual tmat4 toViewMatrix()const {
				tmat3 tmp(view.toMat3());
				return tmat4(tmp, tmp * -position) *= (-rotate).toMat4();
			}

        public:
            explicit Camera(const tvec3 &pos = tvec3()): position(pos),
                     view(tquaternion()), rotate(tquaternion()){
				setOrtho(-1, 1, -1, 1, -1, 1);
			} //ortho
            Camera(const tvec3 &eye, const tvec3 &center, const tvec3 &up):
				rotate(tquaternion()) {
				setOrtho(-1, 1, -1, 1, -1, 1);
                setView(eye, center, up);
            }
            Camera(const Kfloat &fovy, const Kfloat &aspect,
                   const Kfloat &zNear, const Kfloat &zFar,
                   const tvec3 &pos = tvec3()):position(pos),
				view(tquaternion()), rotate(tquaternion()) {
                setPerspective(fovy, aspect, zNear, zFar);
            }
			virtual ~Camera() = default;

			static void bindUniform(const KRenderer::Shader *shader) {
				if (block == nullptr) {
					block = std::make_shared<KBuffer::UnifromBlock>(shader, PROJECTION.c_str());
					block->prepare(std::vector<const char*>{
						EYE.c_str(), VIEW.c_str(), PROJ.c_str()
					});
				}
				else block->bindShader(shader);
			}

			void bind()const {
				if (block != nullptr) {
					block->allocate(std::vector<KBuffer::BlockData>{
						{ EYE.c_str(), getPosition().data() },
						{ VIEW.c_str(), toViewMatrix().data() },
						{ PROJ.c_str(), projection.data() }
					});
				}
			}

            void setPosition(const tvec3 &v){
                position = v;
            }
			void setRotation(const Kfloat& angle, const tvec3& axis) {
				rotate = tquaternion(angle, axis);
			}
            virtual void setView(const tvec3 &eye, const tvec3 &center, const tvec3 &up){
                //u-v-n is left-hand coordinate
                const tvec3 n((center - eye).normalize());
                const tvec3 u(tvec3::cross(n, up).normalize());
                const tvec3 v(tvec3::cross(u, n).normalize());

                position = eye;
                view = tquaternion().fromMatrix(tmat3(u, v, -n));
            }
            void setPerspective(const Kfloat &fovy, const Kfloat &aspect,
				                const Kfloat &zNear, const Kfloat &zFar){
                projection = KFunction::perspective(fovy, aspect, zNear, zFar);

            }
            void setOrtho(const Kfloat &left, const Kfloat &right, const Kfloat &bottom,
				          const Kfloat &top, const Kfloat &near, const Kfloat &far){
                projection = KFunction::ortho(left, right, bottom, top, near, far);
            }
            void setFrustum(const Kfloat &left, const Kfloat &right, const Kfloat &bottom,
                               const Kfloat &top, const Kfloat &near, const Kfloat &far){
                projection = KFunction::frustum(left, right, bottom, top, near, far);
            }

            void rotateCamera(const Kfloat &angle, const tvec3 &v) {
                rotate = tquaternion(angle, v) * rotate;
            }
            void rotateView(const Kfloat &angle, const tvec3 &v){
                //note: view is a inverse rotate matrix(also transpose matrix),
                //that means once you want to add rotate to it,
                //you should right multiply a transpose rotate matrix
                //(rot * originView).inverse = originView.inverse * rot.inverse = view * rot.transpose
                view *= tquaternion(-angle, v); //view *= tquaternion(angle, v).getConjugate();
            }

            void translate(const tvec3 &v){
                position += v;
            }
			virtual tvec3 getPosition()const {
				return rotate * position;
			}
        };

		const std::string Camera::PROJECTION("projection");
		const std::string Camera::EYE("p_eye");
		const std::string Camera::VIEW("u_view");
		const std::string Camera::PROJ("u_proj");

		std::shared_ptr<KBuffer::UnifromBlock> Camera::block(nullptr);
    }
}

#endif //KENGINE_CAMERA_H
