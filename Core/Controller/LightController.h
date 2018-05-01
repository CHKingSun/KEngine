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

        template <typename T = float>
        struct LightController{ //All the variable and functions are static.
            typedef KVector::Vec3<T> tvec3;
            typedef KColor::Color<T> tcolor;
            typedef Light<float> tlight;

        private:
            static std::vector<tlight*> lights;
            static const int MAX_LIGHTS = 12;

        public:
            static const Light<T>* getLight(int index){
                if(index >= lights.size()) return nullptr;
                return lights[index];
            }

            static bool addAmbientLight(const tcolor &ambient, const T &factor = 1.0){
                if(lights.size() == MAX_LIGHTS) return false;
                lights.push_back(new Light<T>(lights.size(), ambient, factor));
                return true;
            }
        };
    }
}

#endif //KENGINE_LIGHTCONTROLLER_H
