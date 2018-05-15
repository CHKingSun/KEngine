//
// Created by KingSun on 2018/4/26.
//

#ifndef KENGINE_LIGHT_H
#define KENGINE_LIGHT_H

#include "../../KHeader.h"
#include "../Vector/Vec3.h"
#include "../Material/Color.h"

namespace KEngine{
    namespace KLight{

        enum LightType{
            AMBIENT = 1, POINT, DIRECTION, SPOT
        };

        class Light{
            friend class KController::LightController;

        private:
            Kuint id;
            LightType type;

        public:
            using tcolor = KMaterial::Color;

            bool enable;

            Kfloat factor; //Light intensity

            tcolor ambient;

            Light(LightType type = AMBIENT): enable(true), type(type),
                 factor(1.0), ambient(KMaterial::WHITE){};
            Light(const tcolor &ambient, LightType type = AMBIENT):
                    enable(true), type(type), factor(1.0), ambient(ambient){};
            Light(const tcolor &ambient, const Kfloat &factor, LightType type = AMBIENT):
                    enable(true), type(type), factor(factor), ambient(ambient){};

			void bind(const KRenderer::Shader *shader) {

			}

            Light& operator=(const Light &light){
                this->type = light.type;
                this->id = light.id;
                this->enable = light.enable;
                this->factor = light.factor;
                this->ambient = light.ambient;
            }
            friend std::ostream& operator<<(std::ostream &os, const Light &light);
        };

        std::ostream& operator<<(std::ostream &os, const Light &light){
            os << "id: " << light.id << " type: ";
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
    }
}

#endif //KENGINE_LIGHT_H
