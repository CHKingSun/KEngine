//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_POINTLIGHT_H
#define KENGINE_POINTLIGHT_H

#include "../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        class PointLight: public Light{
            typedef KVector::Vec3 tvec3;
            typedef KColor::Color tcolor;

        public:
            tvec3 position;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;
            Kfloat kc, kl, kq; //attenuation factor

            PointLight() = delete;
            PointLight(Kuint id):Light(id, POINT), position(tvec3()),
                    diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(Kuint id, const tvec3 &pos):Light(id, POINT), position(pos),
                    diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(Kuint id, const tvec3 &pos,
                       const tcolor &ambient, const tcolor &diffuse, const tcolor &specular):
                    Light(id, ambient, POINT), position(pos), diffuse(diffuse), specular(specular),
                    shadowFactor(0.0), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(Kuint id, const tvec3 &pos,
                       const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                       const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(id, ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(1.0), kl(0.09), kq(0.032){}

            PointLight(Kuint id, const tvec3 &pos,
                       const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                       const Kfloat &factor, const Kfloat &shadowFactor, const Kfloat &kc,const Kfloat &kl, const Kfloat &kq):
                    Light(id, ambient, factor, POINT), position(pos),
                    diffuse(diffuse), specular(specular),
                    shadowFactor(shadowFactor), kc(kc), kl(kl), kq(kq){}

            void translate(const tvec3 &v){
                position += v;
            }
        };
    }
}

#endif //KENGINE_POINTLIGHT_H
