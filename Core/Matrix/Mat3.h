//
// Created by KingSun on 2018/4/23.
//

#ifndef KENGINE_MAT3_H
#define KENGINE_MAT3_H

#include <cassert>
#include <iosfwd>
#include <cmath>
#include "../KHeader.h"

namespace KEngine{
    namespace KMatrix{
        template <typename T>
        struct Mat3{
            typedef KVector::Vec3<T> Row;

        private:
            Row values[3]; //行优先

        public:
            Mat3():Mat3(static_cast<T>(1)){}
            Mat3(const Mat3<T> &m){
                this->values[0] = m[0];
                this->values[1] = m[1];
                this->values[2] = m[2];
            }
            explicit Mat3(const T &c){
                this->values[0] = Row(c, 0, 0);
                this->values[1] = Row(0, c, 0);
                this->values[2] = Row(0, 0, c);
            }
            Mat3(const Row &v0, const Row &v1, const Row &v2){
                this->values[0] = v0;
                this->values[1] = v1;
                this->values[2] = v2;
            }
            Mat3(const T &x0, const T y0, const T &z0,
                 const T &x1, const T &y1, const T &z1,
                 const T &x2, const T &y2, const T &z2){
                values[0] = Row(x0, y0, z0);
                values[1] = Row(x1, y1, z1);
                values[2] = Row(x2, y2, z2);
            }

            const T* data()const {
                return &this->values[0][0];
            }

            Row& operator[](unsigned int n){
                assert(n >= 0 && n < 3);
                return this->values[n];
            }
            const Row& operator[](unsigned int n)const {
                assert(n >= 0 && n < 3);
                return this->values[n];
            }

            Mat3<T>& operator=(const Mat3<T> &m){
                this->values[0] = m[0];
                this->values[1] = m[1];
                this->values[2] = m[2];
                return *this;
            }
            Mat3<T>& operator+=(const Mat3<T> &m){
                this->values[0] += m[0];
                this->values[1] += m[1];
                this->values[2] += m[2];
                return *this;
            }
            Mat3<T>& operator-=(const Mat3<T> &m){
                this->values[0] -= m[0];
                this->values[1] -= m[1];
                this->values[2] -= m[2];
                return *this;
            }
            Mat3<T>& operator*=(const Mat3<T> &m){
                //matrix multiply. m * n != n * m;
                /**
                 * Mat4<T> row(*this);
                 * Mat4<T> col(m).transpose();
                 * for(int i = 0; i < 3; ++i){
                 *      for(int j = 0; j < 3; ++j){
                 *          this->values[i][j] = row[i] * col[j];
                 *      }
                 * }
                 */

                Row row0(this->values[0]);
                Row row1(this->values[1]);
                Row row2(this->values[2]);

                Row col0(m[0][0], m[1][0], m[2][0]);
                Row col1(m[0][1], m[1][1], m[2][1]);
                Row col2(m[0][2], m[1][2], m[2][1]);

                this->values[0][0] = KFunction::dot(row0, col0);
                this->values[0][1] = KFunction::dot(row0, col1);
                this->values[0][2] = KFunction::dot(row0, col2);

                this->values[1][0] = KFunction::dot(row1, col0);
                this->values[1][1] = KFunction::dot(row1, col1);
                this->values[1][2] = KFunction::dot(row1, col2);

                this->values[2][0] = KFunction::dot(row2, col0);
                this->values[2][1] = KFunction::dot(row2, col1);
                this->values[2][2] = KFunction::dot(row2, col2);

                return *this;
            }

            template <typename C>
            Mat3<T>& operator*=(const C &c){
                this->values[0] *= c;
                this->values[1] *= c;
                this->values[2] *= c;
                return *this;
            }
            template <typename C>
            Mat3<T>& operator/=(const C &c){
                this->values[0] /= c;
                this->values[1] /= c;
                this->values[2] /= c;
                return *this;
            }

            Mat3<T>& transpose(){
                for(int i = 0; i < 3; ++i){
                    for(int j = i + 1; j < 3; ++j){
                        T t = this->values[i][j];
                        this->values[i][j] = this->values[j][i];
                        this->values[j][i] = t;
                    }
                }
                return *this;
            }
            Mat3<T>& inverse(){
                const T cof00 = values[1][1] * values[2][2] - values[2][1] * values[1][2];
                const T cof01 = -(values[1][0] * values[2][2] - values[2][0] * values[1][2]);
                const T cof02 = values[1][0] * values[2][1] - values[2][0] * values[1][1];

                const T detM = values[0][0] * cof00 + values[0][1] * cof01 + values[0][2] * cof02;
                if(fabs(detM) <= 1E-6){
                    for(int i = 0; i < 3; ++i)
                        for(int j = 0; j < 3; ++j)
                            values[i][j] = static_cast<T>(KNAN);
                    return *this;
                }

                const T cof10 = -(values[0][1] * values[2][2] - values[2][1] * values[0][2]);
                const T cof11 = values[0][0] * values[2][2] - values[2][0] * values[0][2];
                const T cof12 = -(values[0][0] * values[2][1] - values[2][0] * values[0][1]);

                const T cof20 = values[0][1] * values[1][2] - values[1][1] * values[0][2];
                const T cof21 = -(values[0][0] * values[1][2] - values[1][0] * values[0][2]);
                const T cof22 = values[0][0] * values[1][1] - values[1][0] * values[0][1];

                values[0][0] = cof00 / detM;
                values[0][1] = cof01 / detM;
                values[0][2] = cof02 / detM;

                values[1][0] = cof10 / detM;
                values[1][1] = cof11 / detM;
                values[1][2] = cof12 / detM;

                values[2][0] = cof20 / detM;
                values[2][1] = cof21 / detM;
                values[2][2] = cof22 / detM;

                return *this;
            }
            template <typename F = T>
            F determinant()const {
                const F cof00 = values[1][1] * values[2][2] - values[2][1] * values[1][2];
                const F cof01 = -(values[1][0] * values[2][2] - values[2][0] * values[1][2]);
                const F cof02 = values[1][0] * values[2][1] - values[2][0] * values[1][1];

                return values[0][0] * cof00 + values[0][1] * cof01 + values[0][2] * cof02;
            }
        };

        template <typename T>
        Mat3<T> operator-(const Mat3<T> &m){
            return Mat3<T>(0) -= m;
        }
        template <typename T>
        Mat3<T> operator+(const Mat3<T> &m1, const Mat3<T> &m2){
            return Mat3<T>(m1) += m2;
        }
        template <typename T>
        Mat3<T> operator-(const Mat3<T> &m1, const Mat3<T> &m2){
            return Mat3<T>(m1) -= m2;
        }
        template <typename T>
        Mat3<T> operator*(const Mat3<T> &m1, const Mat3<T> &m2){
            return Mat3<T>(m1) *= m2;
        }
        template <typename T>
        KVector::Vec3<T> operator*(const Mat3<T> &m, const KVector::Vec3<T> &v){
            return KVector::Vec3<T>(
                    KFunction::dot(v, m[0]),
                    KFunction::dot(v, m[1]),
                    KFunction::dot(v, m[1])
            );
        }
        template <typename T, typename C>
        Mat3<T> operator*(const Mat3<T> &m1, const C &c){
            return Mat3<T>(m1) *= c;
        }
        template <typename T, typename C>
        Mat3<T> operator*(const C &c, const Mat3<T> &m1){
            return Mat3<T>(m1) *= c;
        }
        template <typename T, typename C>
        Mat3<T> operator/(const Mat3<T> &m1, const C &c){
            return Mat3<T>(m1) /= c;
        }
        template <typename T>
        std::istream& operator>>(std::istream &is, Mat3<T> &m){
            is >> m[0] >> m[1] >> m[2];
            return is;
        }
        template <typename T>
        std::ostream& operator<<(std::ostream &os, const Mat3<T> &m){
            os << m[0] << '\n';
            os << m[1] << '\n';
            os << m[2] << '\n';
            return os;
        }
    }
}

#endif //KENGINE_MAT3_H
