#include <iostream>

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
#include "./Core/KHeader.h"

using namespace std;
using namespace KEngine;
using namespace KEngine::KColor;
using namespace KEngine::KFunction;
using namespace KEngine::KController;

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::cout << sizeof(KEngine::KVector::Vec2<char>) << std::endl;
    cout << (typeid(1.0) == typeid(double)) << endl;

    vec2 a = vec2(), b = vec2(2.0f);
    Color<char> c = Color<char>(255).setHex(0Xff00ff00);
    c *= 2;
    a + b;
    2.0f * a;
    3.f * vec2(9.0);
    a *= b;
    mat4 m = translate<float>(vec3(9));
    float const *d = mat3().data();
    std::cout<<mat4(static_cast<float>(nan("Nan")))<<std::endl;
    cout<<mat4(2.0f) * vec4(1.0, 1.0, 1.0, 2.0)<<endl;
//    std::cin>>b;
    std::cout<<a.dot(b)<<std::endl;
    std::cout<<KEngine::KFunction::dot(a, b)<<std::endl;
    std::cout<<a.normalize()<<std::endl;
    std::cout<<KEngine::KFunction::normalize(b)<<std::endl;
    std::cout<<b-a<<std::endl;
    std::cout<<KEngine::KFunction::distance(a, b)<<std::endl;
    std::cout<<a.length<int>()<<" "<<a.length()<<std::endl;

//    LightController<float>::addAmbientLight(color(1.0), 0.9);
//    cout << LightController<float>::getLight(0) << endl;

    return 0;
}