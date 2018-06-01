//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_SPOTLIGHT_H
#define KENGINE_SPOTLIGHT_H

#include "../../KHeader.h"
#include "../Matrix/Mat4.h"
#include "../Matrix/Quaternion.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        class SpotLight:public Light{
            using tvec3 = KVector::Vec3;
            using tcolor = KMaterial::Color;

        public:
            tvec3 position;
            tvec3 direction;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;
            Kfloat kc, kl, kq; //attenuation factor
            Kfloat innerCutOff, outerCutOff; //cosine of angle

            SpotLight():Light(SPOT), position(tvec3()), direction(tvec3(0, -1, 0)),
                     diffuse(KMaterial::GREY), specular(KMaterial::GREY),
                     innerCutOff(0.9978), outerCutOff(0.9), shadowFactor(0.0),
                     kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(const tvec3 &pos, const tvec3 &dir):
                    Light(SPOT), position(pos), direction(dir),
                    diffuse(KMaterial::GREY), specular(KMaterial::GREY),
                    innerCutOff(0.9978), outerCutOff(0.9), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(const tvec3 &pos, const tvec3 &dir, const tcolor &ambient,
                      const tcolor &diffuse, const tcolor &specular):
                    Light(ambient, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(0.9978), outerCutOff(0.9), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const Kfloat &inner, const Kfloat &outer):
                    Light(ambient, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const Kfloat &inner, const Kfloat &outer, const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(ambient, factor, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(shadowFactor),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const Kfloat &inner, const Kfloat &outer, const Kfloat &factor, const Kfloat &shadowFactor,
                      const Kfloat &kc,const Kfloat &kl, const Kfloat &kq):
                    Light(ambient, factor, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(shadowFactor),
                    kc(kc), kl(kl), kq(kq){}

			void bind(Kuint id = 0XFFFFFFFF)const override {
				if (id < MAX_LIGHTS_NUM) activeId = id;

				if (block == nullptr) return;
				Kboolean enable = true;
				const std::string index = std::to_string(activeId);
				if (preparedIndex->find(SLIGHT + index) == preparedIndex->end()) {
					preparedIndex->emplace(SLIGHT + index);
					block->prepare(std::vector<KBuffer::BlockData>{
						{ (SLIGHT + index + U_ENABLE).c_str(), &enable },
						{ (SLIGHT + index + U_FACTOR).c_str(), &factor },
						{ (SLIGHT + index + U_SHADOWFACTOR).c_str(), &shadowFactor },
						{ (SLIGHT + index + U_DIRECTION).c_str(), direction.data() },
						{ (SLIGHT + index + U_POSITION).c_str(), position.data() },
						{ (SLIGHT + index + U_AMBIENT).c_str(), ambient.data() },
						{ (SLIGHT + index + U_DIFFUSE).c_str(), diffuse.data() },
						{ (SLIGHT + index + U_SPECULAR).c_str(), specular.data() },
						{ (SLIGHT + index + U_KC).c_str(), &kc },
						{ (SLIGHT + index + U_KL).c_str(), &kl },
						{ (SLIGHT + index + U_KQ).c_str(), &kq },
						{ (SLIGHT + index + U_INNERCUTOFF).c_str(), &innerCutOff },
						{ (SLIGHT + index + U_OUTERCUTOFF).c_str(), &outerCutOff }
					}, true);
				}
				else {
					block->allocate(std::vector<KBuffer::BlockData>{
						{ (SLIGHT + index + U_ENABLE).c_str(), &enable },
						{ (SLIGHT + index + U_FACTOR).c_str(), &factor },
						{ (SLIGHT + index + U_SHADOWFACTOR).c_str(), &shadowFactor },
						{ (SLIGHT + index + U_DIRECTION).c_str(), direction.data() },
						{ (SLIGHT + index + U_POSITION).c_str(), position.data() },
						{ (SLIGHT + index + U_AMBIENT).c_str(), ambient.data() },
						{ (SLIGHT + index + U_DIFFUSE).c_str(), diffuse.data() },
						{ (SLIGHT + index + U_SPECULAR).c_str(), specular.data() },
						{ (SLIGHT + index + U_KC).c_str(), &kc },
						{ (SLIGHT + index + U_KL).c_str(), &kl },
						{ (SLIGHT + index + U_KQ).c_str(), &kq },
						{ (SLIGHT + index + U_INNERCUTOFF).c_str(), &innerCutOff },
						{ (SLIGHT + index + U_OUTERCUTOFF).c_str(), &outerCutOff }
					});
				}
			}

			void bindPosition()const override {
				if (block != nullptr) block->allocate({ (SLIGHT + std::to_string(activeId) + U_POSITION).c_str(), position.data() });
			}

			void bindDirection()const override {
				if (block != nullptr) block->allocate({ (SLIGHT + std::to_string(activeId) + U_DIRECTION).c_str(), direction.data() });
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

            void translate(const tvec3 &v){
                position += v;
            }
			void setPosition(const tvec3& v) {
				position = v;
			}
        };
    }
}

#endif //KENGINE_SPOTLIGHT_H
