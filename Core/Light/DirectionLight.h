//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_DIRECTIONLIGHT_H
#define KENGINE_DIRECTIONLIGHT_H

#include "../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        struct DirectionLight:public Light{
            typedef KVector::Vec3 tvec3;
            typedef KColor::Color tcolor;

            tvec3 direction;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;

            DirectionLight() = delete;
            DirectionLight(Kuint id):Light(id, DIRECTION), direction(tvec3()), shadowFactor(0.0),
                      diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)){}

            DirectionLight(Kuint id, const tvec3 &dir):Light(id, DIRECTION), direction(dir),
                      diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                      shadowFactor(0.0){}

            DirectionLight(Kuint id, const tvec3 &dir,
                           const tcolor &ambient, const tcolor &diffuse, const tcolor &specular):
                    Light(id, ambient, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(0.0){}

            DirectionLight(Kuint id, const tvec3 &dir,
                           const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                           const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(id, ambient, factor, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(shadowFactor){}
        };
    }
}

#endif //KENGINE_DIRECTIONLIGHT_H
