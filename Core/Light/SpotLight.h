//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_SPOTLIGHT_H
#define KENGINE_SPOTLIGHT_H

#include "../KHeader.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        class SpotLight:public Light{
            typedef KVector::Vec3 tvec3;
            typedef KColor::Color tcolor;

        public:
            tvec3 position;
            tvec3 direction;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;
            Kfloat kc, kl, kq; //attenuation factor
            Kfloat innerCutOff, outerCutOff; //cosine of angle

            SpotLight() = delete;
            SpotLight(Kuint id):Light(id, SPOT), position(tvec3()), direction(tvec3()),
                     diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                     innerCutOff(0.9978), outerCutOff(0.953), shadowFactor(0.0),
                     kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(Kuint id, const tvec3 &pos, const tvec3 &dir):
                    Light(id, SPOT), position(pos), direction(dir),
                    diffuse(tcolor(0.8, 0.8, 0.8, 1.0)), specular(tcolor(0.8, 0.8, 0.8, 1.0)),
                    innerCutOff(0.9978), outerCutOff(0.953), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(Kuint id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular):
                    Light(id, ambient, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(0.9978), outerCutOff(0.953), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(Kuint id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const Kfloat &inner, const Kfloat &outer):
                    Light(id, ambient, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(0.0),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(Kuint id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const Kfloat &inner, const Kfloat &outer, const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(id, ambient, factor, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(shadowFactor),
                    kc(1.0), kl(0.09), kq(0.032){}

            SpotLight(Kuint id, const tvec3 &pos, const tvec3 &dir,
                      const tcolor &ambient, const tcolor &diffuse, const tcolor &specular,
                      const Kfloat &inner, const Kfloat &outer, const Kfloat &factor, const Kfloat &shadowFactor,
                      const Kfloat &kc,const Kfloat &kl, const Kfloat &kq):
                    Light(id, ambient, factor, SPOT), position(pos), direction(dir),
                    diffuse(diffuse), specular(specular),
                    innerCutOff(inner), outerCutOff(outer), shadowFactor(shadowFactor),
                    kc(kc), kl(kl), kq(kq){}

            void rotate(const Kfloat &angle, const tvec3 &v){
                direction = KMatrix::Quaternion(angle, v) * direction;
            }
            void rotate(const KMatrix::Quaternion &q){
                direction = q * direction;
            }
            void rotate(const KMatrix::Mat4 &m){
                direction = KMatrix::Quaternion().fromMatrix(m) * direction;
            }
            void rotate(const KMatrix::Mat3 &m){
                direction = KMatrix::Quaternion().fromMatrix(m) * direction;
            }

            void translate(const tvec3 &v){
                position += v;
            }
        };
    }
}

#endif //KENGINE_SPOTLIGHT_H
