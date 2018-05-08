//
// Created by KingSun on 2018/4/27.
//

#ifndef KENGINE_DIRECTIONLIGHT_H
#define KENGINE_DIRECTIONLIGHT_H

#include "../../KHeader.h"
#include "../Vector/Color.h"
#include "../Vector/Vec3.h"
#include "../Matrix/Quaternion.h"
#include "Light.h"

namespace KEngine{
    namespace KLight{
        class DirectionLight:public Light{
            typedef KVector::Vec3 tvec3;
            typedef KColor::Color tcolor;

        public:
            tvec3 direction;

            tcolor diffuse;
            tcolor specular;

            Kfloat shadowFactor;

            DirectionLight():Light(DIRECTION), direction(tvec3()), shadowFactor(0.0),
                      diffuse(KColor::GREY), specular(KColor::GREY){}

            DirectionLight(const tvec3 &dir):Light(DIRECTION), direction(dir),
                      diffuse(KColor::GREY), specular(KColor::GREY),
                      shadowFactor(0.0){}

            DirectionLight(const tvec3 &dir, const tcolor &ambient,
                           const tcolor &diffuse, const tcolor &specular):
                    Light(ambient, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(0.0){}

            DirectionLight(const tvec3 &dir, const tcolor &ambient, const tcolor &diffuse,
                           const tcolor &specular, const Kfloat &factor, const Kfloat &shadowFactor):
                    Light(ambient, factor, DIRECTION), direction(dir),
                    diffuse(diffuse), specular(specular), shadowFactor(shadowFactor){}

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
        };
    }
}

#endif //KENGINE_DIRECTIONLIGHT_H
