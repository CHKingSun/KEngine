//
// Created by KingSun on 2018/4/23.
//

#ifndef KENGINE_MAT4_H
#define KENGINE_MAT4_H

#include <cassert>
#include <iosfwd>
#include <cmath>
#include "../KHeader.h"
#include "Mat3.h"
#include "../Vector/Vec4.h"

namespace KEngine{
    namespace KMatrix{
        template <typename T>
        struct Mat4{
            typedef KVector::Vec4<T> Row;

        private:
            Row values[4]; //行优先

        public:
            Mat4():Mat4(static_cast<T>(1)){}
            Mat4(const Mat4<T> &m){
                this->values[0] = m[0];
                this->values[1] = m[1];
                this->values[2] = m[2];
                this->values[3] = m[3];
            }
            explicit Mat4(const T &c){
                this->values[0] = Row(c, 0, 0, 0);
                this->values[1] = Row(0, c, 0, 0);
                this->values[2] = Row(0, 0, c, 0);
                this->values[3] = Row(0, 0, 0, c);
            }
            Mat4(const Row &v0, const Row &v1, const Row &v2, const Row &v3){
                this->values[0] = v0;
                this->values[1] = v1;
                this->values[2] = v3;
                this->values[3] = v3;
            }
            Mat4(const T &x0, const T &y0, const T &z0, const T &w0,
                 const T &x1, const T &y1, const T &z1, const T &w1,
                 const T &x2, const T &y2, const T &z2, const T &w2,
                 const T &x3, const T &y3, const T &z3, const T &w3){
                this->values[0] = Row(x0, y0, z0, w0);
                this->values[1] = Row(x1, y1, z1, w1);
                this->values[2] = Row(x2, y2, z2, w2);
                this->values[3] = Row(x3, y3, z3, w3);
            }

            const T* data()const {
                return &this->values[0][0];
            }

            Row& operator[](unsigned int n){
                assert(n >= 0 && n < 4);
                return this->values[n];
            }
            const Row& operator[](unsigned int n)const {
                assert(n >= 0 && n < 4);
                return this->values[n];
            }

            Mat4<T>& operator=(const Mat4<T> &m){
                this->values[0] = m[0];
                this->values[1] = m[1];
                this->values[2] = m[2];
                this->values[3] = m[3];
            }
            Mat4<T>& operator+=(const Mat4<T> &m){
                this->values[0] += m[0];
                this->values[1] += m[1];
                this->values[2] += m[2];
                this->values[3] += m[3];
                return *this;
            }
            Mat4<T>& operator-=(const Mat4<T> &m){
                this->values[0] -= m[0];
                this->values[1] -= m[1];
                this->values[2] -= m[2];
                this->values[2] *= m[2];
                return *this;
            }
            Mat4<T>& operator*=(const Mat4<T> &m){
                //matrix multiply. m * n != n * m;
                /**
                 * Mat4<T> row(*this);
                 * Mat4<T> col(m).transpose();
                 * for(int i = 0; i < 4; ++i){
                 *      for(int j = 0; j < 4; ++j){
                 *          this->values[i][j] = row[i] * col[j];
                 *      }
                 * }
                 */

                Row row0(this->values[0]);
                Row row1(this->values[1]);
                Row row2(this->values[2]);
                Row row3(this->values[3]);

                Row col0(m[0][0], m[1][0], m[2][0], m[3][0]);
                Row col1(m[0][1], m[1][1], m[2][1], m[3][1]);
                Row col2(m[0][2], m[1][2], m[2][1], m[3][2]);
                Row col3(m[0][3], m[1][3], m[2][3], m[3][3]);

                this->values[0][0] = KFunction::dot(row0, col0);
                this->values[0][1] = KFunction::dot(row0, col1);
                this->values[0][2] = KFunction::dot(row0, col2);
                this->values[0][3] = KFunction::dot(row0, col3);

                this->values[1][0] = KFunction::dot(row1, col0);
                this->values[1][1] = KFunction::dot(row1, col1);
                this->values[1][2] = KFunction::dot(row1, col2);
                this->values[1][3] = KFunction::dot(row1, col3);

                this->values[2][0] = KFunction::dot(row2, col0);
                this->values[2][1] = KFunction::dot(row2, col1);
                this->values[2][2] = KFunction::dot(row2, col2);
                this->values[2][3] = KFunction::dot(row2, col3);

                this->values[3][0] = KFunction::dot(row3, col0);
                this->values[3][1] = KFunction::dot(row3, col1);
                this->values[3][2] = KFunction::dot(row3, col2);
                this->values[3][3] = KFunction::dot(row3, col3);

                return *this;
            }

            template <typename C>
            Mat4<T>& operator*=(const C &c){
                this->values[0] *= c;
                this->values[1] *= c;
                this->values[2] *= c;
                this->values[3] *= c;
                return *this;
            }
            template <typename C>
            Mat4<T>& operator/=(const C &c){
                this->values[0] /= c;
                this->values[1] /= c;
                this->values[2] /= c;
                this->values[3] /= c;
                return *this;
            }

            Mat4<T>& transpose(){
                for(int i = 0; i < 4; ++i){
                    for(int j = i + 1; j < 4; ++j){
                        T t = this->values[i][j];
                        this->values[i][j] = this->values[j][i];
                        this->values[j][i] = t;
                    }
                }
                return *this;
            }
            Mat4<T>& inverse(){
                // | 00 01 02 03 |
                // | 10 11 12 13 |
                // | 20 21 22 23 |
                // | 30 31 32 33 |
                const T det0 = values[2][0] * values[3][1] - values[3][0] * values[2][1];
                const T det1 = values[2][0] * values[3][2] - values[3][0] * values[2][2];
                const T det2 = values[2][0] * values[3][3] - values[3][0] * values[2][3];
                const T det3 = values[2][1] * values[3][2] - values[3][1] * values[2][2];
                const T det4 = values[2][1] * values[3][3] - values[3][1] * values[2][3];
                const T det5 = values[2][2] * values[3][3] - values[3][2] * values[2][3];

                const T cof00 = values[1][1] * det5 - values[1][2] * det4 + values[1][3] * det3;
                const T cof01 = values[1][0] * det5 - values[1][2] * det2 + values[1][3] * det1;
                const T cof02 = values[1][0] * det4 - values[1][1] * det2 + values[1][3] * det0;
                const T cof03 = values[1][0] * det3 - values[1][1] * det1 + values[1][2] * det0;

                const T detM = values[0][0] * cof00 - values[0][1] * cof01 +
                               values[0][2] * cof02 - values[0][3] * cof03;
                if(fabs(detM) <= 1E-6){
                    for(int i = 0; i < 4; ++i)
                        for(int j = 0; j < 4; ++j)
                            values[i][j] = static_cast<T>(KNAN);
                    return *this;
                }

                const T cof10 = values[0][1] * det5 - values[0][2] * det4 + values[0][3] * det3;
                const T cof11 = values[0][0] * det5 - values[0][2] * det2 + values[0][3] * det1;
                const T cof12 = values[0][0] * det4 - values[0][1] * det2 + values[0][3] * det0;
                const T cof13 = values[0][0] * det3 - values[0][1] * det1 + values[0][2] * det0;

                const T det6 = values[0][0] * values[1][1] - values[1][0] * values[0][1];
                const T det7 = values[0][0] * values[1][2] - values[1][0] * values[0][2];
                const T det8 = values[0][0] * values[1][3] - values[1][0] * values[0][3];
                const T det9 = values[0][1] * values[1][2] - values[1][1] * values[0][2];
                const T det10 = values[0][1] * values[1][3] - values[1][1] * values[0][3];
                const T det11 = values[0][2] * values[1][3] - values[1][2] * values[0][3];

                const T cof20 = values[3][1] * det11 - values[3][2] * det10 + values[3][3] * det9;
                const T cof21 = values[3][0] * det11 - values[3][2] * det8 + values[3][3] * det7;
                const T cof22 = values[3][0] * det10 - values[3][1] * det8 + values[3][3] * det6;
                const T cof23 = values[3][0] * det9 - values[3][1] * det7 + values[3][2] * det6;
                const T cof30 = values[2][1] * det11 - values[2][2] * det10 + values[2][3] * det9;
                const T cof31 = values[2][0] * det11 - values[2][2] * det8 + values[2][3] * det7;
                const T cof32 = values[2][0] * det10 - values[2][1] * det8 + values[2][3] * det6;
                const T cof33 = values[2][0] * det9 - values[2][1] * det7 + values[2][2] * det6;

                values[0][0] = cof00 / detM;
                values[0][1] = -cof01 / detM;
                values[0][2] = cof02 / detM;
                values[0][3] = -cof03 / detM;

                values[1][0] = -cof10 / detM;
                values[1][1] = cof11 / detM;
                values[1][2] = -cof12 / detM;
                values[1][3] = cof13 / detM;

                values[2][0] = cof20 / detM;
                values[2][1] = -cof21 / detM;
                values[2][2] = cof22 / detM;
                values[2][3] = -cof23 / detM;

                values[3][0] = -cof30 / detM;
                values[3][1] = cof31 / detM;
                values[3][2] = -cof32 / detM;
                values[3][3] = cof33 / detM;

                return *this;
            }
            template <typename F = T>
            F determinant()const {
                const F det0 = values[2][0] * values[3][1] - values[3][0] * values[2][1];
                const F det1 = values[2][0] * values[3][2] - values[3][0] * values[2][2];
                const F det2 = values[2][0] * values[3][3] - values[3][0] * values[2][3];
                const F det3 = values[2][1] * values[3][2] - values[3][1] * values[2][2];
                const F det4 = values[2][1] * values[3][3] - values[3][1] * values[2][3];
                const F det5 = values[2][2] * values[3][3] - values[3][2] * values[2][3];

                const F cof00 = values[1][1] * det5 - values[1][2] * det4 + values[1][3] * det3;
                const F cof01 = values[1][0] * det5 - values[1][2] * det2 + values[1][3] * det1;
                const F cof02 = values[1][0] * det4 - values[1][1] * det2 + values[1][3] * det0;
                const F cof03 = values[1][0] * det3 - values[1][1] * det1 + values[1][2] * det0;

                return values[0][0] * cof00 - values[0][1] * cof01 +
                       values[0][2] * cof02 - values[0][3] * cof03;
            }
            Mat3<T> toMat3(){
                return Mat3<T>(
                        values[0][0], values[0][1], values[0][2],
                        values[0][0], values[0][1], values[0][2],
                        values[0][0], values[0][1], values[0][2]
                );
            }
        };

        template <typename T>
        Mat4<T> operator-(const Mat4<T> &m){
            return Mat4<T>(0) -= m;
        }
        template <typename T>
        Mat4<T> operator+(const Mat4<T> &m1, const Mat4<T> &m2){
            return Mat4<T>(m1) += m2;
        }
        template <typename T>
        Mat4<T> operator-(const Mat4<T> &m1, const Mat4<T> &m2){
            return Mat4<T>(m1) -= m2;
        }
        template <typename T>
        Mat4<T> operator*(const Mat4<T> &m1, const Mat4<T> &m2){
            return Mat4<T>(m1) *= m2;
        }
        template <typename T>
        KVector::Vec4<T> operator*(const Mat4<T> &m, KVector::Vec4<T> &v){
            return KVector::Vec4<T>(
                    KFunction::dot(v, m[0]),
                    KFunction::dot(v, m[1]),
                    KFunction::dot(v, m[2]),
                    KFunction::dot(v, m[3])
            );
        }
        template <typename T, typename C>
        Mat4<T> operator*(const Mat4<T> &m1, const C &c){
            return Mat4<T>(m1) *= c;
        }
        template <typename T, typename C>
        Mat4<T> operator*(const C &c, const Mat4<T> &m1){
            return Mat4<T>(m1) *= c;
        }
        template <typename T, typename C>
        Mat4<T> operator/(const Mat4<T> &m1, const C &c){
            return Mat4<T>(m1) /= c;
        }
        template <typename T>
        std::istream& operator>>(std::istream &is, Mat4<T> &m){
            is >> m[0] >> m[1] >> m[2] >> m[3];
            return is;
        }
        template <typename T>
        std::ostream& operator<<(std::ostream &os, const Mat4<T> &m){
            os << m[0] << '\n';
            os << m[1] << '\n';
            os << m[2] << '\n';
            os << m[3] << '\n';
            return os;
        }
    }
}

#endif //KENGINE_MAT4_H
