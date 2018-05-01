//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_SPOTLIGHT_H
#define KENGINE_SPOTLIGHT_H

#include "../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        template <typename T>
        struct SpotLight:public Light<T>{
            typedef KVector::Vec3<T> tvec3;
            typedef KColor::Color<T> tcolor;

            tvec3 position;
            tvec3 direction;

            tcolor diffuse;
            tcolor specular;

            T shadowFactor;
            T kc, kl, kq; //attenuation factor
            T innerCutOff, outerCutOff; //cosine of angle

            SpotLight() = delete;
            SpotLight(unsigned int id):Light<T>(id, SPOT), position(tvec3()), direction(tvec3()),
                     diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                     innerCutOff(0.9978), outerCutOff(0.953), shadowFactor(0.0),
                     kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(unsigned int id, const tvec3 &pos, const tvec3 &dir):
                    Light<T>(id, SPOT), position(pos), direction(dir),
                    diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                    innerCutOff(0.9978), outerCutOff(0.953), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(unsigned int id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular):
                    Light<T>(id, ambient, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(0.9978), outerCutOff(0.953), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(unsigned int id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const T &inner, const T &outer):
                    Light<T>(id, ambient, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(unsigned int id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const T &inner, const T &outer, const T &factor, const T &shadowFactor):
                    Light<T>(id, ambient, factor, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(shadowFactor),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(unsigned int id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const T &inner, const T &outer, const T &factor, const T &shadowFactor,
                      const T &kc,const T &kl, const T &kq):
                    Light<T>(id, ambient, factor, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(shadowFactor),
                    kc(kc), kl(kl), kq(kq){}
        };
    }
}

#endif //KENGINE_SPOTLIGHT_H
