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

			void bind(const KRenderer::Shader* shader, Kuint id = 0XFFFFFFFF)const override {
				if (id < MAX_LIGHTS_NUM) activeId = id;

				const std::string index = std::to_string(activeId);
				shader->bindUniform1i(DLIGHT + index + U_ENABLE, enable);
				shader->bindUniform1f(DLIGHT + index + U_FACTOR, factor);
				shader->bindUniform1f(DLIGHT + index + U_SHADOWFACTOR, shadowFactor);
				shader->bindUniform3f(DLIGHT + index + U_DIRECTION, direction);
				shader->bindUniform4f(DLIGHT + index + U_AMBIENT, ambient);
				shader->bindUniform4f(DLIGHT + index + U_DIFFUSE, diffuse);
				shader->bindUniform4f(DLIGHT + index + U_SPECULAR, specular);
			}

			void bindDirection(const KRenderer::Shader* shader)const override {
				shader->bindUniform3f(DLIGHT + std::to_string(activeId) + U_DIRECTION, direction);
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
