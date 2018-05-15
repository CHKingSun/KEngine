//
// Created by KingSun on 2018/05/11
//

#ifndef KENGINE_FACE_H
#define KENGINE_FACE_H

#include <cassert>
#include "../KHeader.h"

namespace KEngine {
	namespace KObject {
		template <typename T = Kushort>
		class Face {
		private:
			T values[3];

		public:
			Face<T>() = default;
			Face<T>(T p0, T p1, T p2) {
				set(p0, p1, p2);
			}
			~Face<T>() = default;

			inline const T* data()const {
				return &values[0];
			}
			inline const T& operator[](Kuint n)const {
				assert(n >= 0 && n < 3);
				return values[n];
			}
			inline T& operator[](Kuint n) {
				assert(n >= 0 && n < 3);
				return values[n];
			}

			Face<T>& set(T p0, T p1, T p2) {
				values[0] = p0;
				values[1] = p1;
				values[2] = p2;
				return *this;
			}

			//Vec3 getNormal(const Kfloat* data)
			//judge if a point is in face. (const Kfloat* data, Ve3 point)
			//judge if a ray is intersect with face. (const Kfloat* data, Ray ray)
		};
	}
}

#endif //KENGINE_FACE_H
