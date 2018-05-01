//
// Created by KingSun on 2018/4/26.
//

#ifndef KENGINE_LIGHT_H
#define KENGINE_LIGHT_H

#include "../KHeader.h"
#include "../Vector/Vec3.h"
#include "../Vector/Color.h"

namespace KEngine{
    namespace KLight{

        enum LightType{
            AMBIENT = 1, POINT, DIRECTION, SPOT
        };

        struct Light{
        private:
            Kuint id;
            LightType type;

        public:
            typedef KColor::Color tcolor;

            bool enable;

            Kfloat factor; //Light intensity

            tcolor ambient;

            Light() = delete;
            Light(Kuint id, LightType type = AMBIENT): id(id), enable(true), type(type),
                 factor(1.0), ambient(tcolor(0.8, 0.8, 0.8, 1.0)){};
            Light(Kuint id, const tcolor &ambient, LightType type = AMBIENT):
                    enable(true), type(type), factor(1.0), ambient(ambient){};
            Light(Kuint id, const tcolor &ambient, const Kfloat &factor, LightType type = AMBIENT):
                    enable(true), type(type), factor(factor), ambient(ambient){};

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
