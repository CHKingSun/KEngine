//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_DIRECTIONLIGHT_H
#define KENGINE_DIRECTIONLIGHT_H

#include "../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        template <typename T>
        struct DirectionLight:public Light<T>{
            typedef KVector::Vec3<T> tvec3;
            typedef KColor::Color<T> tcolor;

            tvec3 direction;

            tcolor diffuse;
            tcolor specular;

            T shadowFactor;

            DirectionLight() = delete;
            DirectionLight(unsigned int id):Light<T>(id, DIRECTION), direction(tvec3()), shadowFactor(0.0),
                      diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)){}

            DirectionLight(unsigned int id, const tvec3 &dir):Light<T>(id, DIRECTION), direction(dir),
                      diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                      shadowFactor(0.0){}

            DirectionLight(unsigned int id, const tvec3 &dir,
                           const tcolor &ambient, const tcolor &diffuse, const tcolor &specular):
                    Light<T>(id, ambient, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(0.0){}

            DirectionLight(unsigned int id, const tvec3 &dir,
                           const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                           const T &factor, const T &shadowFactor):
                    Light<T>(id, ambient, factor, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(shadowFactor){}
        };
    }
}

#endif //KENGINE_DIRECTIONLIGHT_H
