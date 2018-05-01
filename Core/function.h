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
        template <typename T, template <typename > typename vecType, typename F = T>
        F dot(const vecType<T> &vec1, const vecType<T> &vec2){
            F res = 0;
            for(int i = 0; i < vec1.dimension(); ++i) res += vec1[i] * vec2[i];
            return res;
        };

        template <typename T, template <typename > typename vecType, typename F = T>
        F length(const vecType<T> &vec){
            return sqrt(dot<T, vecType, F>(vec, vec));
        };

        template <typename T, template <typename > typename vecType, typename F = T>
        F distance(const vecType<T> &vec1, const vecType<T> &vec2){
            return length<T, vecType, F>(vec2 - vec1);
        };

        template <typename T, template <typename > typename vecType, typename F = T>
        vecType<T> normalize(const vecType<T> &vec){
            const F len = length<T, vecType, F>(vec);
            if(KCore::isZero(len)) return vecType<T>(KNAN);
            return vec / len;
        };

        template <typename T, template <typename > typename vecType, typename F = T>
        vecType<T> faceforward(const vecType<T> &N, const vecType<T> &I, const vecType<T> Nref){
            if(dot<T, vecType, F>(Nref, I) < static_cast<F>(0)){
                return N;
            } else {
                return -N;
            }
        };

        template <typename T, template <typename > typename vecType, typename F = T>
        vecType<T> reflect(const vecType<T> &I, const vecType<T> &N){
            const vecType<T> n = normalize(N);
            return I - static_cast<T>(2) * dot<T, vecType, F>(n, I) * n;
        };

        template <typename T, template <typename > typename vecType, typename F = T>
        vecType<T> refract(const vecType<T> &I, const vecType<T> &N, const F &eta){
            const vecType<T> i = normalize(I);
            const vecType<T> n = normalize(N);
            const F cosni = dot<T, vecType, F>(n, i);

            const F k = static_cast<F>(1) - eta * eta * (static_cast<F>(1) - cosni * cosni);
            if(k < static_cast<F>(0)){
                return vecType<T>(static_cast<T>(0));
            } else {
                return eta * i - (eta * cosni + sqrt(k)) * n;
            }
        };

        //matrix functions
        template <typename T, template <typename > typename matType>
        matType<T> inverse(const matType<T> &m){
            return matType<T>(m).inverse();
        };

        template <typename T, template <typename > typename matType>
        matType<T> transpose(const matType<T> &m){
            return matType<T>(m).inverse();
        };

        template <typename T, template <typename > typename matType, typename F = T>
        F determinant(const matType<T> &m){
            return m.determinant();
        };
    }
}

#endif //KENGINE_FUNCTION_H
