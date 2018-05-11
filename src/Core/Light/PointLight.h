//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_POINTLIGHT_H
#define KENGINE_POINTLIGHT_H

#include "../../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        class PointLight: public Light{
            using tvec3 = KVector::Vec3;
            using tcolor = KColor::Color;

        public:
            tvec3 position;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;
            Kfloat kc, kl, kq; //attenuation factor

            PointLight():Light(POINT), position(tvec3()),
                    diffuse(KColor::GREY), specular(KColor::GREY),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos):Light(POINT), position(pos),
                    diffuse(KColor::GREY), specular(KColor::GREY),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos, const tcolor &ambient,
                       const tcolor &diffuse, const tcolor &specular):
                    Light(ambient, POINT), position(pos), diffuse(diffuse), specular(specular),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos, const tcolor &ambient, const tcolor &diffuse,
                       const tcolor &specular, const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(const tvec3 &pos, const tcolor &ambient, const tcolor &diffuse,
                       const tcolor &specular, const Kfloat &factor, const Kfloat &shadowFactor,
                       const Kfloat &kc,const Kfloat &kl, const Kfloat &kq):
                    Light(ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(kc), kl(kl), kq(kq){}

            void translate(const tvec3 &v){
                position += v;
            }
        };
    }
}

#endif //KENGINE_POINTLIGHT_H
