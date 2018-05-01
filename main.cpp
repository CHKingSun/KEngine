/**
 * just for test
 * */

#include <iostream>

#include "./Core/KHeader.h"
#include "./Core/function.h"
#include "./Core/transform.h"
#include "./Core/Vector/Vec2.h"
#include "./Core/Vector/Vec3.h"
#include "./Core/Vector/Vec4.h"
#include "./Core/Matrix/Mat3.h"
#include "./Core/Matrix/Mat4.h"
#include "./Core/Vector/Color.h"
#include "./Core/Light/Light.h"
#include "./Core/Light/PointLight.h"
#include "./Core/Light/DirectionLight.h"
#include "./Core/Light/SpotLight.h"
//#include "./Core/Controller/LightController.h"

using namespace std;
using namespace KEngine;
using namespace KEngine::KColor;
using namespace KEngine::KFunction;
using namespace KEngine::KVector;
using namespace KEngine::KMatrix;
using namespace KEngine::KLight;
//using namespace KEngine::KController;

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << sizeof(KEngine::KVector::Vec2) << std::endl;

    Vec2 a = Vec2(), b = Vec2(2.0f);
    Color c = Color(255).setHex(0Xff00ff00);
    c *= 2;
    a + b;
    2.0f * a;
    3.f * Vec2(9.0);
    a *= b;
    Mat4 m = translate(Vec3(9));
    float const *d = Mat3().data();
    std::cout<<Mat4(static_cast<float>(KNAN))<<std::endl;
    cout<<Mat4(2.0f) * Vec4(1.0, 1.0, 1.0, 2.0)<<endl;
    std::cin>>b;
    std::cout<<a.dot(b)<<std::endl;
    std::cout<<KEngine::KFunction::dot(a, b)<<std::endl;
    std::cout<<a.normalize()<<std::endl;
    std::cout<<KEngine::KFunction::normalize(b)<<std::endl;
    std::cout<<b-a<<std::endl;
    std::cout<<KEngine::KFunction::distance(a, b)<<std::endl;
    std::cout<<a.length<int>()<<" "<<a.length()<<std::endl;
    cout<<PointLight(0)<<endl;

//    LightController controller;
//    controller.addAmbientLight(Color(1.0), 0.9);
//    cout << controller.getLight(0) << endl;

    return 0;
}