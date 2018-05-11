 /**
  * just for test
  * */
 
#include <iostream>
#include <string>
#include <GL/glew.h>
#include "Window.h"
#include "./Render/Renderer.h"

const std::string ROOT_PATH = "./";

#ifdef TEST

#include "./KHeader.h"
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
#include "./Core/Material/Material.h"
#include "./Controller/LightController.h"
#include "./Core/Camera/Camera.h"
#include "Render/Shader.h"
#include "Render/Renderer.h"


void test() {

	using namespace std;
	using namespace KEngine;
	using namespace KEngine::KColor;
	using namespace KEngine::KFunction;
	using namespace KEngine::KVector;
	using namespace KEngine::KMatrix;
	using namespace KEngine::KLight;
	using namespace KEngine::KController;
	using namespace KEngine::KCamera;
	using namespace KEngine::KRenderer;

	std::cout << "Hello, World!" << std::endl;
	std::cout << sizeof(KEngine::KVector::Vec2) << std::endl;
	cout << (-0.0 == 0.0) << endl;
	offsetof(Vec4, r);
	cout << ((Mat4*)nullptr == nullptr) << endl;

	Vec2 a = Vec2(), b = Vec2(2.0f);
	Color c = Color(255).setFromHex(0Xff00ff00);
	c *= 2;
	a + b;
	2.0f * a;
	3.f * Vec2(9.0);
	a *= b;
	Mat4 m = translate(Vec3(9));
	float const *d = Mat3().data();
	std::cout << Mat4(static_cast<float>(KNAN)) << std::endl;
	cout << Mat4(2.0f) * Vec4(1.0, 1.0, 1.0, 2.0) << endl;
	//    std::cin>>b;
	std::cout << a.dot(b) << std::endl;
	std::cout << KEngine::KFunction::dot(a, b) << std::endl;
	std::cout << a.normalize() << std::endl;
	std::cout << KEngine::KFunction::normalize(b) << std::endl;
	std::cout << b - a << std::endl;
	std::cout << KEngine::KFunction::distance(a, b) << std::endl;
	std::cout << a.length<int>() << " " << a.length() << std::endl;
	cout << Mat3(
		2, 1, -1,
		1, 4, 2,
		5, -3, 1
	).determinant() << endl;
	cout << PointLight() << endl;
	Mat4 rot = rotate(23.5, Vec3(1.9, 0, 1.2));
	//    cout<<Mat4(rot).inverse() * rot<<endl<<endl;
	//    cout<<rot.inverse()<<endl<<endl;
	cout << Quaternion().fromMatrix(rot) << endl << endl;
	cout << Quaternion(23.5, Vec3(1.9, 0, 1.2)) << endl << endl;
	cout << Quaternion(120, Vec3(1, 0, 1)) * Vec3(0, 0, 2) << endl << endl;
	cout << lookAt(Vec3(10, 0, 0), Vec3(0, 0, -10), Vec3(0, 1, 0)) << endl << endl;
	Camera camera(Vec3(0, 0, 10), Vec3(0, 0, 0), Vec3(0, 1, 0));
	camera.setPosition(Vec3(10, 0, 0)).rotateView(45, Vec3(0, -1, 0));
	cout << camera.getView() << endl << endl;

	//static auto *controller = new Renderer();
	//controller->lightController->addLight(new DirectionLight(Vec3(1, 0, 1)));
	//controller->lightController->addLight(new SpotLight(Vec3(1, 1, 1), Vec3(0, 0, 1)));
	//controller->lightController->deleteLight(0);
	//cout<<*(controller->lightController->at(0))<<endl;
}

#endif // TEST
 
 int main() {

#ifdef TEST
	 //test();
#endif // TEST

	 auto *window = new KEngine::KWindow::Window("Kengine");
	 auto *renderer = new KEngine::KRenderer::Renderer(ROOT_PATH + "res/base.vert", ROOT_PATH + "res/base.frag");

	 while (!window->closed())
	 {
		 renderer->clear();
		 renderer->render();
		 window->update();
	 }

	 delete renderer;
	 delete window;
 
     return 0;
 }