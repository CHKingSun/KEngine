//
// Created by KingSun on 2018/4/30.
//

#ifndef KENGINE_LIGHTCONTROLLER_H
#define KENGINE_LIGHTCONTROLLER_H

#include <vector>
#include "../Light/Light.h"
#include "../Light/PointLight.h"
#include "../Light/DirectionLight.h"
#include "../Light/SpotLight.h"
#include "../KHeader.h"

namespace KEngine{
    namespace KController{
        using namespace KLight;

        class LightController{ //All the variable and functions are static.
            typedef KVector::Vec3 tvec3;
            typedef KColor::Color tcolor;

        private:
            static std::vector<KEngine::KLight::Light*> lights;
            static const Ksize MAX_LIGHTS = 12;

        public:
            static const Light* getLight(Kuint index){
                if(index >= lights.size()) return nullptr;
                return lights.at(index);
            }

            static Kboolean addAmbientLight(const tcolor &ambient, const Kfloat &factor = 1.0){
                if(lights.size() == MAX_LIGHTS) return false;
                lights.push_back(new Light(lights.size(), ambient, factor));
                return true;
            }
        };
    }
}

#endif //KENGINE_LIGHTCONTROLLER_H
