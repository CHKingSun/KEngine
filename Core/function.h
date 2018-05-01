//
// Created by KingSun on 2018/4/22.
//

#ifndef KENGINE_FUNCTION_H
#define KENGINE_FUNCTION_H

#include <cmath>
#include "KHeader.h"

namespace KEngine{
    namespace KFunction{
        //vector functions
        template <typename vecType, typename F = Kfloat>
        F dot(const vecType &vec1, const vecType &vec2){
            F res = 0;
            for(int i = 0; i < vec1.dimension(); ++i) res += vec1[i] * vec2[i];
            return res;
        };

        template <typename vecType, typename F = Kfloat>
        F length(const vecType &vec){
            return sqrt(dot<vecType, F>(vec, vec));
        };

        template <typename vecType, typename F = Kfloat>
        F distance(const vecType &vec1, const vecType &vec2){
            return length<vecType, F>(vec2 - vec1);
        };

        template <typename vecType, typename F = Kfloat>
        vecType normalize(const vecType &vec){
            const F len = length<vecType, F>(vec);
            if(KCore::isZero(len)) return vecType(KNAN);
            return vec / len;
        };

        template <typename vecType, typename F = Kfloat>
        vecType faceforward(const vecType &N, const vecType &I, const vecType Nref){
            if(dot<vecType, F>(Nref, I) < static_cast<F>(0)){
                return N;
            } else {
                return -N;
            }
        };

        template <typename vecType>
        vecType reflect(const vecType &I, const vecType &N){
            const vecType n = normalize(N);
            return I - static_cast<Kfloat>(2) * dot<vecType, Kfloat>(n, I) * n;
        };

        template <typename vecType, typename F = Kfloat>
        vecType refract(const vecType &I, const vecType &N, const F &eta){
            const vecType i = normalize(I);
            const vecType n = normalize(N);
            const F cosni = dot<vecType, F>(n, i);

            const F k = static_cast<F>(1) - eta * eta * (static_cast<F>(1) - cosni * cosni);
            if(k < static_cast<F>(0)){
                return vecType();
            } else {
                return eta * i - (eta * cosni + sqrt(k)) * n;
            }
        };

        //matrix functions
        template <typename matType>
        matType inverse(const matType &m){
            return matType(m).inverse();
        };

        template <typename matType>
        matType transpose(const matType &m){
            return matType(m).inverse();
        };

        template <typename matType, typename F = Kfloat>
        F determinant(const matType &m){
            return m.determinant();
        };
    }
}

#endif //KENGINE_FUNCTION_H
