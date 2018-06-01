//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_DIRECTIONLIGHT_H
#define KENGINE_DIRECTIONLIGHT_H

#include "../../KHeader.h"
#include "Light.h"
#include "../Matrix/Quaternion.h"

namespace KEngine{
    namespace KLight{
        class DirectionLight : public Light{
            using tvec3 = KVector::Vec3;
            using tcolor = KMaterial::Color;

        public:
            tvec3 direction;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;

            DirectionLight():Light(DIRECTION), direction(tvec3(0, -1, 0)), shadowFactor(0.0),
                      diffuse(KMaterial::GREY), specular(KMaterial::GREY){}

            DirectionLight(const tvec3 &dir):Light(DIRECTION), direction(dir),
                      diffuse(KMaterial::GREY), specular(KMaterial::GREY),
                      shadowFactor(0.0){}

            DirectionLight(const tvec3 &dir, const tcolor &ambient,
                           const tcolor &diffuse, const tcolor &specular):
                    Light(ambient, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(0.0){}

            DirectionLight(const tvec3 &dir, const tcolor &ambient, const tcolor &diffuse,
                           const tcolor &specular, const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(ambient, factor, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(shadowFactor){}

			void bind(Kuint id = 0XFFFFFFFF)const override {
				if (id < MAX_LIGHTS_NUM) activeId = id;

				if (block == nullptr) return;
				Kboolean enable = true;
				const std::string index = std::to_string(activeId);
				if (preparedIndex->find(DLIGHT + index) == preparedIndex->end()) {
					preparedIndex->emplace(DLIGHT + index);
					block->prepare(std::vector<KBuffer::BlockData>{
						{ (DLIGHT + index + U_ENABLE).c_str(), &enable },
						{ (DLIGHT + index + U_FACTOR).c_str(), &factor },
						{ (DLIGHT + index + U_SHADOWFACTOR).c_str(), &shadowFactor },
						{ (DLIGHT + index + U_DIRECTION).c_str(), direction.data() },
						{ (DLIGHT + index + U_AMBIENT).c_str(), ambient.data() },
						{ (DLIGHT + index + U_DIFFUSE).c_str(), diffuse.data() },
						{ (DLIGHT + index + U_SPECULAR).c_str(), specular.data() }
					}, true);
				}
				else {
					block->allocate(std::vector<KBuffer::BlockData>{
						{ (DLIGHT + index + U_ENABLE).c_str(), &enable },
						{ (DLIGHT + index + U_FACTOR).c_str(), &factor },
						{ (DLIGHT + index + U_SHADOWFACTOR).c_str(), &shadowFactor },
						{ (DLIGHT + index + U_DIRECTION).c_str(), direction.data() },
						{ (DLIGHT + index + U_AMBIENT).c_str(), ambient.data() },
						{ (DLIGHT + index + U_DIFFUSE).c_str(), diffuse.data() },
						{ (DLIGHT + index + U_SPECULAR).c_str(), specular.data() }
					});
				}
			}

			void bindDirection()const override {
				if(block != nullptr) block->allocate({ (DLIGHT + std::to_string(activeId) + U_DIRECTION).c_str(), direction.data() });
			}

            void rotate(const Kfloat &angle, const tvec3 &v){
                direction = KMatrix::Quaternion(angle, v) * direction;
            }
            void rotate(const KMatrix::Quaternion &q){
                direction = q * direction;
            }
            void rotate(const KMatrix::Mat4 &m){
                direction = KMatrix::Quaternion().fromMatrix(m) * direction;
            }
            void rotate(const KMatrix::Mat3 &m){
                direction = KMatrix::Quaternion().fromMatrix(m) * direction;
            }
        };
    }
}

#endif //KENGINE_DIRECTIONLIGHT_H
