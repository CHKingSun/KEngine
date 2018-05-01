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

        template <typename T = float>
        struct Light{
        private:
            unsigned int id;
            LightType type;

        public:
            typedef KColor::Color<T> tcolor;

            bool enable;

            T factor; //Light intensity

            tcolor ambient;

            Light();
            Light(unsigned int id, LightType type = AMBIENT): id(id), enable(true), type(type),
                 factor(1.0), ambient(tcolor(0.8, 0.8, 0.8, 1.0)){};
            Light(unsigned int id, const tcolor &ambient, LightType type = AMBIENT):
                    enable(true), type(type), factor(1.0), ambient(ambient){};
            Light(unsigned int id, const tcolor &ambient, const T &factor, LightType type = AMBIENT):
                    enable(true), type(type), factor(factor), ambient(ambient){};

            Light<T>& operator=(const Light<T> &light){
                this->type = light.type;
                this->id = light.id;
                this->enable = light.enable;
                this->factor = light.factor;
                this->ambient = light.ambient;
            }
        };

        template <typename T>
        std::ostream& operator<<(std::ostream &os, const Light<T> &light){
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
