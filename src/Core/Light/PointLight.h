//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_POINTLIGHT_H
#define KENGINE_POINTLIGHT_H

#include "../../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        class PointLight: public Light{
            using tvec3 = KVector::Vec3;
            using tcolor = KMaterial::Color;

        public:
            tvec3 position;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;
            Kfloat kc, kl, kq; //attenuation factor

            PointLight():Light(POINT), position(tvec3()),
                    diffuse(KMaterial::GREY), specular(KMaterial::GREY),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos):Light(POINT), position(pos),
                    diffuse(KMaterial::GREY), specular(KMaterial::GREY),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos, const tcolor &ambient,
                       const tcolor &diffuse, const tcolor &specular):
                    Light(ambient, POINT), position(pos), diffuse(diffuse), specular(specular),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos, const tcolor &ambient, const tcolor &diffuse,
                       const tcolor &specular, const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos, const tcolor &ambient, const tcolor &diffuse,
                       const tcolor &specular, const Kfloat &factor, const Kfloat &shadowFactor,
                       const Kfloat &kc,const Kfloat &kl, const Kfloat &kq):
                    Light(ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(kc), kl(kl), kq(kq){}

			void bind(Kuint id = 0XFFFFFFFF)const override {
				if (id < MAX_LIGHTS_NUM) activeId = id;

				if (block == nullptr) return;
				Kboolean enable = true;
				const std::string index = std::to_string(activeId);
				if (preparedIndex->find(PLIGHT + index) == preparedIndex->end()) {
					preparedIndex->emplace(PLIGHT + index);
					block->prepare(std::vector<KBuffer::BlockData>{
						{ (PLIGHT + index + U_ENABLE).c_str(), &enable },
						{ (PLIGHT + index + U_FACTOR).c_str(), &factor },
						{ (PLIGHT + index + U_SHADOWFACTOR).c_str(), &shadowFactor },
						{ (PLIGHT + index + U_POSITION).c_str(), position.data() },
						{ (PLIGHT + index + U_AMBIENT).c_str(), ambient.data() },
						{ (PLIGHT + index + U_DIFFUSE).c_str(), diffuse.data() },
						{ (PLIGHT + index + U_SPECULAR).c_str(), specular.data() },
						{ (PLIGHT + index + U_KC).c_str(), &kc },
						{ (PLIGHT + index + U_KL).c_str(), &kl },
						{ (PLIGHT + index + U_KQ).c_str(), &kq }
					}, true);
				}
				else {
					block->allocate(std::vector<KBuffer::BlockData>{
						{ (PLIGHT + index + U_ENABLE).c_str(), &enable },
						{ (PLIGHT + index + U_FACTOR).c_str(), &factor },
						{ (PLIGHT + index + U_SHADOWFACTOR).c_str(), &shadowFactor },
						{ (PLIGHT + index + U_POSITION).c_str(), position.data() },
						{ (PLIGHT + index + U_AMBIENT).c_str(), ambient.data() },
						{ (PLIGHT + index + U_DIFFUSE).c_str(), diffuse.data() },
						{ (PLIGHT + index + U_SPECULAR).c_str(), specular.data() }
					});
				}
			}

			void bindPosition()const override {
				if(block != nullptr) block->allocate({ (PLIGHT + std::to_string(activeId) + U_POSITION).c_str(), position.data() });
			}

            void translate(const tvec3 &v){
                position += v;
            }
        };
    }
}

#endif //KENGINE_POINTLIGHT_H
