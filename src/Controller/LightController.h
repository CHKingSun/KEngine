//
// Created by KingSun on 2018/4/30.
//

#ifndef KENGINE_LIGHTCONTROLLER_H
#define KENGINE_LIGHTCONTROLLER_H

#include <vector>
#include "../Core/Light/Light.h"
#include "../Core/Light/PointLight.h"
#include "../Core/Light/DirectionLight.h"
#include "../Core/Light/SpotLight.h"
#include "../KHeader.h"

namespace KEngine{
    namespace KController{
        using namespace KLight;

        class LightController{
        private:
            std::vector<Light*> *lights;
            Light *defaultLight;
            Kuint nextId;
            Kuint n_lights;
            const Ksize MAX_LIGHTS = 12;

        public:
            LightController(){
                nextId = 0;
                n_lights = 0;
                lights = nullptr;
                defaultLight = new Light();
                defaultLight->id = nextId++;
            }
            ~LightController(){
                deleteAllLight();
                delete lights;
                delete defaultLight;
            }

            Light* at(Kuint index)const {
                if(lights == nullptr || index >= n_lights) return nullptr;
                return lights->at(index);
            }

            Kboolean deleteLight(Kuint index){
                if(lights == nullptr || index >= n_lights) return false;
                auto it = lights->begin() + index;
                delete(*it);
                *it = nullptr;
                lights->erase(it);
                if(--n_lights == 0) defaultLight->enable = true;
                return true;
            }
            void deleteAllLight(){
                if(lights == nullptr) return;
                for (auto &light : *lights){
                    delete light;
                    light = nullptr;
                }
                lights->clear();
                n_lights = 0;
                nextId = 1;
            }

            Kuint size()const {
                return n_lights;
            }
            Kboolean setDefaultLight(Light *light){
                if(light == nullptr) return false;
                light->id = 0;
                light->enable = defaultLight->enable;
                delete(defaultLight);
                defaultLight = light;
                return true;
            }
            Kuint addLight(Light *light){
                if(n_lights == MAX_LIGHTS || light == nullptr) return 0; //return default id
                if(lights == nullptr) lights = new std::vector<Light*>();
                light->id = nextId;
                //Maybe later will use for vectors to save each type lights;
                //switch
                lights->push_back(light);
                ++n_lights;
                defaultLight->enable = false;
                return nextId++;
            }
        };
    }
}

#endif //KENGINE_LIGHTCONTROLLER_H
