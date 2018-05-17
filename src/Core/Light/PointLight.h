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

			void bind(const KRenderer::Shader* shader, Kuint id = 0XFFFFFFFF)const override {
				if (id < MAX_LIGHTS_NUM) activeId = id;

				const std::string index = std::to_string(activeId);
				shader->bindUniform1i(PLIGHT + index + U_ENABLE, enable);
				shader->bindUniform1f(PLIGHT + index + U_FACTOR, factor);
				shader->bindUniform1f(PLIGHT + index + U_SHADOWFACTOR, shadowFactor);
				shader->bindUniform3f(PLIGHT + index + U_POSITION, position);
				shader->bindUniform4f(PLIGHT + index + U_AMBIENT, ambient);
				shader->bindUniform4f(PLIGHT + index + U_DIFFUSE, diffuse);
				shader->bindUniform4f(PLIGHT + index + U_SPECULAR, specular);
				shader->bindUniform1f(PLIGHT + index + U_KC, kc);
				shader->bindUniform1f(PLIGHT + index + U_KL, kl);
				shader->bindUniform1f(PLIGHT + index + U_KQ, kq);
			}

			void bindPosition(const KRenderer::Shader* shader)const override {
				shader->bindUniform3f(PLIGHT + std::to_string(activeId) + U_POSITION, position);
			}

            void translate(const tvec3 &v){
                position += v;
            }
        };
    }
}

#endif //KENGINE_POINTLIGHT_H
