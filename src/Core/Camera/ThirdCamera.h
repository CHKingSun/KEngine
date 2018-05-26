//
// Created by KingSun on 2018/05/26
//

#ifndef KENGINE_THIRD_CAMERA_H
#define KENGINE_THIRD_CAMERA_H

#include "../../KHeader.h"
#include "./Camera.h"
#include "../../Object/Object3D.h"

namespace KEngine {
	namespace KCamera{
		class ThirdCamera : public Camera {
			using tvec3 = KVector::Vec3;
			using tmat3 = KMatrix::Mat3;
			using tquaternion = KMatrix::Quaternion;
			using tmat4 = KMatrix::Mat4;

		private:
			Kfloat restrictAngle;
			Kfloat space;
			KObject::Object3D* target;
			const static tvec3 restrictAxis;

			tmat4 toViewMatrix()const override {
				tmat3 vw(view.toMat3());
				tmat3 rot((-rotate).toMat3());
				tvec3 pos(0, 0, 0);
				if (target != nullptr) pos = target->getPosition();
				return tmat4(vw, vw * -position) *= tmat4(rot, rot * -pos);
			}

		public:
			explicit ThirdCamera(Kfloat angle = 45.0f,
				Kfloat space = 1.0, KObject::Object3D* object = nullptr):
				Camera(), space(space), target(object), restrictAngle(angle) {
				position = (getDirection(BACK) * space);
				} //ortho
			ThirdCamera(const tvec3 &eye, const tvec3 &center, const tvec3 &up,
				Kfloat angle = 45.0f, Kfloat space = 1.0, KObject::Object3D* object = nullptr):
				Camera(eye, center, up), space(space), target(object), restrictAngle(angle) {
				position = (getDirection(BACK) * space);
			}
			ThirdCamera(const Kfloat &fovy, const Kfloat &aspect,
				const Kfloat &zNear, const Kfloat &zFar,
				Kfloat angle = 45.0f, Kfloat space = 1.0, KObject::Object3D* object = nullptr):
				Camera(fovy, aspect, zNear, zFar), space(space), target(object), restrictAngle(angle) {
				position = (getDirection(BACK) * space);
			}
			~ThirdCamera() override = default;

			void setRestrictAngle(Kfloat angle) {
				restrictAngle = angle;
			}

			void setSpace(Kfloat space) {
				this->space = space;
				position = (getDirection(BACK) * space);
			}

			void setTarget(KObject::Object3D* object) {
				target = object;
			}

			void setView(const tvec3 &eye, const tvec3 &center, const tvec3 &up) override {
				//u-v-n is left-hand coordinate
				const tvec3 n((center - eye).normalize());
				const tvec3 u(tvec3::cross(n, up).normalize());
				const tvec3 v(tvec3::cross(u, n).normalize());

				view = tquaternion().fromMatrix(tmat3(u, v, -n));
				position = (getDirection(BACK) * space);
			}

			void rotateView(const tvec3& v) {
				const static tvec3 center(0, 0, 1);

				if (!KFunction::isZero(v.y) &&
					!(restrictAxis.getAngle(getDirection(FORWARD)) <= restrictAngle && v.y < 0 ||
						restrictAxis.getAngle(getDirection(BACK)) <= restrictAngle && v.y > 0)) {
					if (v.y < 0) view *= tquaternion(center.getAngle(tvec3(0, v.y, v.z)), (-view * tvec3(-1, 0, 0)) * tvec3(1, 0, 1));
					else view *= tquaternion(center.getAngle(tvec3(0, v.y, v.z)), (-view * tvec3(1, 0, 0)) * tvec3(1, 0, 1));
				}
				if (!KFunction::isZero(v.x)) {
					rotate = tquaternion(center.getAngle(tvec3(v.x, 0, v.z)), tvec3(0, v.x, 0)) *= rotate;
				}
			}

			const tvec3& getPosition()const override {
				if (target != nullptr) return target->getPosition() - (rotate * -position);
				return rotate * position;
			}

			tvec3 getDirection(DirectionType type = FORWARD)const {
				switch (type)
				{
				case KEngine::KCamera::FORWARD:
					return rotate * -view * tvec3(0, 0, -1);\
				case KEngine::KCamera::BACK:
					return rotate * -view * tvec3(0, 0, 1);
				case KEngine::KCamera::LEFT:
					return rotate * -view * tvec3(-1, 0, 0);
				case KEngine::KCamera::RIGHT:
					return rotate * -view * tvec3(1, 0, 0);
				default:
					break;
				}
				return -view * tvec3(0, 0, -1);
			}

			void translate(const tvec3 &v) = delete;
			void setPosition(const tvec3 &v) = delete;
		};

		const KVector::Vec3 ThirdCamera::restrictAxis(0, 1, 0);
	}
}

#endif // !KENGINE_THIRD_CAMERA_H
