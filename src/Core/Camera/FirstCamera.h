//
// Created by KingSun on 2018/05/20
//

#include "./Camera.h"

namespace KEngine {
	namespace KCamera {
		//note: FirstCamera's rotate is always be default
		class FirstCamera : public Camera {
			using tvec3 = KVector::Vec3;
			using tmat3 = KMatrix::Mat3;
			using tquaternion = KMatrix::Quaternion;
			using tmat4 = KMatrix::Mat4;

		private:
			Kfloat restrictAngle;
			const static tvec3 restrictAxis;

			tmat4 toViewMatrix()const override {
				tmat3 tmp(view.toMat3());
				return tmat4(tmp, tmp * -position);
			}

		public:
			explicit FirstCamera(const tvec3 &pos = tvec3(), Kfloat angle = 30.0f): Camera(pos), restrictAngle(angle) {} //ortho
			FirstCamera(const tvec3 &eye, const tvec3 &center, const tvec3 &up, Kfloat angle = 30.0f):
				Camera(eye, center, up), restrictAngle(angle) {}
			FirstCamera(const Kfloat &fovy, const Kfloat &aspect,
				const Kfloat &zNear, const Kfloat &zFar,
				const tvec3 &pos = tvec3(), Kfloat angle = 30.0f):
				Camera(fovy, aspect, zNear, zFar, pos), restrictAngle(angle) {}
			~FirstCamera() override = default;

			void setRestrictAngle(Kfloat angle) {
				restrictAngle = angle;
			}

			void rotateView(const Kfloat &angle, const tvec3 &v) {
				//note: view is a inverse rotate matrix(also transpose matrix),
				//that means once you want to add rotate to it,
				//you should right multiply a transpose rotate matrix
				//(rot * originView).inverse = originView.inverse * rot.inverse = view * rot.transpose
				view *= tquaternion(-angle, v); //view *= tquaternion(angle, v).getConjugate();
			}

			void rotateView(const tvec3& v) {
				const static tvec3 center(0, 0, 1);

				if (!KFunction::isZero(v.y) &&
					!(restrictAxis.getAngle(getDirection(FORWARD)) <= restrictAngle && v.y < 0 ||
					restrictAxis.getAngle(getDirection(BACK)) <= restrictAngle && v.y > 0)) {
					if (v.y < 0) view *= tquaternion(center.getAngle(tvec3(0, v.y, v.z)), (-view * tvec3(-1, 0, 0)) * tvec3(1, 0, 1));
					else view *= tquaternion(center.getAngle(tvec3(0, v.y, v.z)), (-view * tvec3(1, 0, 0)) * tvec3(1, 0, 1));
					//the following is the same as before. multipling (1, 0, 1) is to correct the dir(the y axis is always be 0).
					//if (v.y < 0) view *= tquaternion(center.getAngle(tvec3(0, v.y, v.z)), getDirection(LEFT));
					//else view *= tquaternion(center.getAngle(tvec3(0, v.y, v.z)), getDirection(RIGHT));
					//or
					//tvec3 up(0, v.y, v.z);
					//view *= tquaternion(up.getAngle(center), -view * up.cross(center));
				}
				if (!KFunction::isZero(v.x)) {
					view *= tquaternion(center.getAngle(tvec3(v.x, 0, v.z)), tvec3(0, -v.x, 0));
					//if (v.x < 0) view *= tquaternion(center.getAngle(tvec3(v.x, 0, v.z)), tvec3(0, 1, 0));
					//else view *= tquaternion(center.getAngle(tvec3(v.x, 0, v.z)), tvec3(0, -1, 0));
					//the following is the same as before.
					//tvec3 right(v.x, 0, v.z);
					//view *= tquaternion(right.getAngle(center), right.cross(center));
				}

				//When you rotate to left or right, you do not need to mutiply view with axis. So the following is wrong.
				//view *= tquaternion(-(v1.getAngle(v2)), -view * v1.cross(v2));
				//note: you should change your center form (0, 0, 1) to your view direction.
				//still has some fault like right direction(u) is not prependicular to y axis.
				//view = tquaternion(-(v1.getAngle(v2)), v1.cross(v2)) * view;
				//It's wrong with direction, but you can get the rgiht display.
				//rotateView(v1.getAngle(v2), v1.cross(v2));
			}

			tvec3 getDirection(DirectionType type = FORWARD)const {
				switch (type)
				{
				case KEngine::KCamera::FORWARD:
					return -view * tvec3(0, 0, -1);
					//note: we at first use (0, 0, -1) as center(or (0, 0, 0) when eye position(0, 0, 1));
					//view is a inverse rotate matrix,
				case KEngine::KCamera::BACK:
					return -view * tvec3(0, 0, 1);
				case KEngine::KCamera::LEFT:
					return -view * tvec3(-1, 0, 0);
				case KEngine::KCamera::RIGHT:
					return -view * tvec3(1, 0, 0);
				default:
					break;
				}
				return -view * tvec3(0, 0, -1);
			}

			tvec3 getPosition()const override {
				return position;
			}

			void setRotation(const Kfloat& angle, const tvec3& axis) = delete;
			void rotateCamera(const Kfloat &angle, const tvec3 &v) = delete;
		};

		const KVector::Vec3 FirstCamera::restrictAxis(0, 1, 0);
	}
}
