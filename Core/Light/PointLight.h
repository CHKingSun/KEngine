//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_POINTLIGHT_H
#define KENGINE_POINTLIGHT_H

#include "../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        template <typename T = float>
        struct PointLight: public Light<T>{
            typedef KVector::Vec3<T> tvec3;
            typedef KColor::Color<T> tcolor;

            tvec3 position;

            tcolor diffuse;
            tcolor specular;

            T shadowFactor;
            T kc, kl, kq; //attenuation factor

            PointLight() = delete;
            PointLight(unsigned int id):Light<T>(id, POINT), position(tvec3()),
                    diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(unsigned int id, const tvec3 &pos):Light<T>(id, POINT), position(pos),
                    diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(unsigned int id, const tvec3 &pos,
                       const tcolor &ambient, const tcolor &diffuse, const tcolor &specular):
                    Light<T>(id, ambient, POINT), position(pos), diffuse(diffuse), specular(specular),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(unsigned int id, const tvec3 &pos,
                       const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                       const T &factor, const T &shadowFactor):
                    Light<T>(id, ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(unsigned int id, const tvec3 &pos,
                       const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                       const T &factor, const T &shadowFactor, const T &kc,const T &kl, const T &kq):
                    Light<T>(id, ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(kc), kl(kl), kq(kq){}
        };
    }
}

#endif //KENGINE_POINTLIGHT_H
