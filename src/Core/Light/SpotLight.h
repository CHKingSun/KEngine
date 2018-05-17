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

			void bind(const KRenderer::Shader* shader, Kuint id = 0XFFFFFFFF)const override {
				if (id < MAX_LIGHTS_NUM) activeId = id;

				const std::string index = std::to_string(activeId);
				shader->bindUniform1i(SLIGHT + index + U_ENABLE, enable);
				shader->bindUniform1f(SLIGHT + index + U_FACTOR, factor);
				shader->bindUniform1f(SLIGHT + index + U_SHADOWFACTOR, shadowFactor);
				shader->bindUniform3f(SLIGHT + index + U_POSITION, position);
				shader->bindUniform3f(SLIGHT + index + U_DIRECTION, direction);
				shader->bindUniform4f(SLIGHT + index + U_AMBIENT, ambient);
				shader->bindUniform4f(SLIGHT + index + U_DIFFUSE, diffuse);
				shader->bindUniform4f(SLIGHT + index + U_SPECULAR, specular);
				shader->bindUniform1f(SLIGHT + index + U_KC, kc);
				shader->bindUniform1f(SLIGHT + index + U_KL, kl);
				shader->bindUniform1f(SLIGHT + index + U_KQ, kq);
				shader->bindUniform1f(SLIGHT + index + U_INNERCUTOFF, innerCutOff);
				shader->bindUniform1f(SLIGHT + index + U_OUTERCUTOFF, outerCutOff);
			}

			void bindPosition(const KRenderer::Shader* shader)const override {
				shader->bindUniform3f(SLIGHT + std::to_string(activeId) + U_POSITION, position);
			}

			void bindDirection(const KRenderer::Shader* shader)const override {
				shader->bindUniform3f(SLIGHT + std::to_string(activeId) + U_DIRECTION, direction);
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
        };
    }
}

#endif //KENGINE_SPOTLIGHT_H
