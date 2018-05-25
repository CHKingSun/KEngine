//
// Created by KingSun on 2018/4/26.
//

#ifndef KENGINE_LIGHT_H
#define KENGINE_LIGHT_H

#include <ostream>
#include <string>
#include "../../KHeader.h"
#include "../../Render/Shader.h"
#include "../Vector/Vec3.h"
#include "../Material/Color.h"

namespace KEngine{
    namespace KLight{

        enum LightType{
            AMBIENT = 1, POINT, DIRECTION, SPOT
        };

        class Light{
		protected:
            mutable Kuint activeId;
            LightType type;

			const static std::string ALIGHT; //ambient light u_aLights
			const static std::string PLIGHT; //point light u_pLights
			const static std::string DLIGHT; //diffuse light u_dLights
			const static std::string SLIGHT; //spot light u_sLights

			const static std::string U_ENABLE; //enable
			const static std::string U_POSITION; //position
			const static std::string U_DIRECTION; //direction
			const static std::string U_FACTOR; //factor
			const static std::string U_SHADOWFACTOR; //shadowFactor
			const static std::string U_AMBIENT; //ambient
			const static std::string U_DIFFUSE; //diffuse
			const static std::string U_SPECULAR; //specular
			const static std::string U_KC; //kc
			const static std::string U_KL; //kl
			const static std::string U_KQ; //kq
			const static std::string U_INNERCUTOFF; //innerCutOff
			const static std::string U_OUTERCUTOFF; //outerCutoff

			const static Kuint MAX_LIGHTS_NUM;

        public:
            using tcolor = KMaterial::Color;

            bool enable;

            Kfloat factor; //Light intensity

            tcolor ambient;

            Light(LightType type = AMBIENT): enable(true), type(type), activeId(0),
                 factor(1.0), ambient(KMaterial::GREY){};
            Light(const tcolor &ambient, LightType type = AMBIENT): activeId(0),
                    enable(true), type(type), factor(1.0), ambient(ambient){};
            Light(const tcolor &ambient, const Kfloat &factor, LightType type = AMBIENT):
                    enable(true), type(type), activeId(0), factor(factor), ambient(ambient){};

			virtual void bind(const KRenderer::Shader* shader, Kuint id = 0XFFFFFFFF)const {
				if (id < MAX_LIGHTS_NUM) activeId = id;

				const std::string index = std::to_string(activeId);
				shader->bindUniform1i(ALIGHT + index + U_ENABLE, enable);
				shader->bindUniform1f(ALIGHT + index + U_FACTOR, factor);
				shader->bindUniform4f(ALIGHT + index + U_AMBIENT, ambient);
			}

			virtual void bindPosition(const const KRenderer::Shader* shader)const {
				//Do nothing in Light and Direction class.
			}

			virtual void bindDirection(const const KRenderer::Shader* shader)const {
				//Do nothing in Light and Position class.
			}

			void active(const KRenderer::Shader* shader)const {
				//Maybe it's better to be a virtual function
				std::string name;
				switch (type) {
				case KEngine::KLight::AMBIENT:
					name = ALIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				case KEngine::KLight::POINT:
					name = PLIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				case KEngine::KLight::DIRECTION:
					name = DLIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				case KEngine::KLight::SPOT:
					name = SLIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				default:
					break;
				}
				shader->bindUniform1i(name, 1);
			}

			void unActive(const KRenderer::Shader* shader)const {
				std::string name;
				switch (type) {
				case KEngine::KLight::AMBIENT:
					name = ALIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				case KEngine::KLight::POINT:
					name = PLIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				case KEngine::KLight::DIRECTION:
					name = DLIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				case KEngine::KLight::SPOT:
					name = SLIGHT + std::to_string(activeId) + U_ENABLE;
					break;
				default:
					break;
				}
				shader->bindUniform1i(name, 0);
			}

            friend std::ostream& operator<<(std::ostream &os, const Light &light);
        };

        std::ostream& operator<<(std::ostream &os, const Light &light){
            os << "id: " << light.activeId << " type: ";
            switch (light.type){
                case AMBIENT:
                    os << "AMBIENT";
                    break;
                case POINT:
                    os << "POINT";
                    break;
                case DIRECTION:
                    os << "DIRECTION";
                    break;
                case SPOT:
                    os << "SPOT";
                    break;
            }
            return os;
        }

		const Kuint Light::MAX_LIGHTS_NUM = 4;

		const std::string Light::ALIGHT = "u_aLights[";
		const std::string Light::PLIGHT = "u_pLights[";
		const std::string Light::DLIGHT = "u_dLights[";
		const std::string Light::SLIGHT = "u_sLights[";

		const std::string Light::U_ENABLE = "].enable";
		const std::string Light::U_POSITION = "].position";
		const std::string Light::U_DIRECTION = "].direction";
		const std::string Light::U_FACTOR = "].factor";
		const std::string Light::U_SHADOWFACTOR = "].shadowFactor";
		const std::string Light::U_AMBIENT = "].ambient";
		const std::string Light::U_DIFFUSE = "].diffuse";
		const std::string Light::U_SPECULAR = "].specular";
		const std::string Light::U_KC = "].kc";
		const std::string Light::U_KL = "].kl";
		const std::string Light::U_KQ = "].kq";
		const std::string Light::U_INNERCUTOFF = "].innerCutOff";
		const std::string Light::U_OUTERCUTOFF = "].outerCutOff";
    }
}

#endif //KENGINE_LIGHT_H
