//
// Create by KingSun in 2018/05/11
//

#ifndef KENGINE_OBJECT3D_H
#define KENGINE_OBJECT3D_H

#include <string>
#include "../KHeader.h"
#include "../Core/Vector/Vec3.h"
#include "../Core/Matrix/Quaternion.h"

namespace KEngine {
	namespace KObject {
		using tvec3 = KVector::Vec3;
		using tquaternion = KMatrix::Quaternion;

		class Object3D {
		private:
			tvec3 position;
			tquaternion rotation;
			tvec3 scale;

			std::string type;

		public:

		};
	}
}

#endif //KENGINE_OBJECT3D_H
