 /**
  * just for test
  * */
 
#include <iostream>
#include <iomanip>
#include <string>
#include <GL/glew.h>
#include "./Log.h"
#include "./Window.h"
#include "./Object/Plane.h"
#include "./Render/Shader.h"
#include "./Core/Camera/Camera.h"
#include "./Core/Light/Light.h"
#include "./Core/Light/DirectionLight.h"
#include "./Core/Light/PointLight.h"
#include "./Core/Light/SpotLight.h"
#include "./Render/Buffer/BackBuffer.h"

//#define TEST
#ifdef TEST

#include "./KHeader.h"
#include "./Core/function.h"
#include "./Core/transform.h"
#include "./Core/Vector/Vec2.h"
#include "./Core/Vector/Vec3.h"
#include "./Core/Vector/Vec4.h"
#include "./Core/Matrix/Mat3.h"
#include "./Core/Matrix/Mat4.h"
#include "./Core/Material/Color.h"
#include "./Core/Material/Material.h"
#include "./Core/Light/Light.h"
#include "./Core/Light/PointLight.h"
#include "./Core/Light/DirectionLight.h"
#include "./Core/Light/SpotLight.h"
#include "./Core/Material/Material.h"
#include "./Controller/LightController.h"
#include "./Core/Camera/Camera.h"
#include "Render/Shader.h"
#include "Render/Renderer.h"

#include <memory>

void smart_prt() {
	using namespace KEngine::KVector;

	std::shared_ptr<Vec4> sptr = std::make_shared<Vec4>();
	std::cout << *sptr << std::endl;
	sptr.reset();
}

void vector_test() {
	using namespace KEngine::KVector;
	using namespace std;
	std::vector<Vec2> vs;
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
	vs.emplace_back();
	cout << vs.capacity() << endl;
}

void const_ptr() {
	const char* const* const strs = nullptr;
	char *a = nullptr;
	//strs[0][1] = 'a';
	//strs[0] = "Hello";
	//strs = &a;

	char chars[2][3];
	char chars1[2][3];
	//chars[0] = chars1[0];
}

void test() {

	using namespace std;
	using namespace KEngine;
	using namespace KEngine::KFunction;
	using namespace KEngine::KVector;
	using namespace KEngine::KMatrix;
	using namespace KEngine::KMaterial;
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
	 //smart_prt();

	 vector_test();

	 std::cin.get();
#else // TEST

	using namespace KEngine;
	using namespace KEngine::KBuffer;
	using namespace KEngine::KRenderer;
	using namespace KEngine::KVector;
	using namespace KEngine::KLight;

	std::cout << Vec3(0, 0, -1).dot(Vec3(0, -5, -5)) << std::endl;

	auto window = new KEngine::KWindow::Window("KEngine");
	auto shader = new Shader(RES_PATH + "base.vert", RES_PATH + "base.frag");

	auto camera = new KCamera::Camera(Vec3(0, 20, 30));

	auto light = new PointLight(Vec3(0, 0, -5));
	auto light1 = new SpotLight(Vec3(0, 6, 10), Vec3(0, 0, -1));

	auto plane = new KEngine::KObject::Plane(30, 20, 30, 20);
	auto plane1 = new KEngine::KObject::Plane(30, 20, 30, 20);

	shader->bind();
	KObject::Object3D::bindUniform(shader);
	KMaterial::Material::bindUniform(shader);
	KCamera::Camera::bindUniform(shader);

	camera->setPerspective(45, 1.0, 0.1, 100);
	camera->rotateView(30, Vec3(-1, 0, 0));

	//auto camera2 = new KCamera::Camera(Vec3(0, 20, -30));
	//camera2->setPerspective(45, 1.0, 0.1, 100);
	//camera2->rotateView(180, Vec3(0, 1, 0));
	//camera2->rotateView(30, Vec3(1, 0, 0));

	Kboolean flag = false;

	plane->setPosition(Vec3(0, 5, -10));
	auto material = new KMaterial::Material(KMaterial::Color(0.4, 0.4, 0.4, 1.0));
	//material->addTexture(RES_PATH + "floor.jpg");
	plane->setMaterial(material);

	//plane1->addTexture(RES_PATH + "stone.png", KMaterial::TextureType::DIFFUSE);
	//plane1->addTexture(RES_PATH + "stone.png");
	plane1->setRotation(-90, Vec3(1, 0, 0));
	plane1->setPosition(Vec3(0, -5, 0));

	glEnable(GL_DEPTH_TEST);

	//plane->setRenderMode(GL_FRONT_AND_BACK, GL_LINE);

#if 0
	glEnable(GL_RASTERIZER_DISCARD);
	auto backShader = new KRenderer::Shader();
	backShader->addShader(GL_VERTEX_SHADER, ROOT_PATH + "res/base.vert");
	backShader->bind();
	KObject::Object3D::bindUniform(backShader);
	KMaterial::Material::bindUniform(backShader);
	KCamera::Camera::bindUniform(backShader);
	camera->bind();
	light->bind(backShader, 0);
	light1->bind(backShader, 0);
	light1->bind(shader, 0);

	std::vector<std::string> varyings = {
		//"b_pos",
		//"b_scale",
		//"b_rotate"
		"b_out"
	};
	//using OutData = KVector::Vec3;
	using OutData = KMatrix::Mat4;
	auto tfbo = new BackBuffer(backShader, varyings, sizeof(OutData) * plane1->getCount());
	tfbo->bind();
	glCall(;);

	tfbo->enable(GL_TRIANGLES);
	plane1->render();
	tfbo->disable();

	const auto data = tfbo->getData<OutData>();
	Ksize size = tfbo->getBufferSize() / sizeof(OutData);
	for (int i = 0; i < size; ++i) {
		std::cout << data[i] << std::endl;
	}
		
	delete tfbo;
	delete backShader;
	shader->bind();
	KObject::Object3D::bindUniform(shader);
	KMaterial::Material::bindUniform(shader);
	KCamera::Camera::bindUniform(shader);
	glDisable(GL_RASTERIZER_DISCARD);
#endif // FEEDBACK

	camera->bind();
	light->bind(shader, 0);
	light1->bind(shader, 0);

	Kdouble now = window->getRunTime();
	Ksize count = 0;
	while (!window->closed())
	{
		++count;
		window->clear();

		//plane->setRotation(window->getRunTime() * 10, Vec3(0, 1, 0));

// 		camera->rotateCamera(1, Vec3(0, 1, 0));
// 		camera->bind();

		light1->rotate(1, Vec3(1, 0, 0));
		light1->bindDirection(shader);

		plane->bindTextures(shader);
		plane->render();
		plane1->bindTextures(shader);
		plane1->render();

		window->update();
		if (window->getRunTime() - now >= 1.0) {
			std::cout << count << "fps" << std::endl;
			count = 0;
			now = window->getRunTime();
			//if (flag) light1->active(shader);
			//else light1->unActive(shader);
			//flag = !flag;
		}
	}

	//glCall(std::cin.get();)

	delete shader;
	delete window;
	KMaterial::Texture::deleteAllTextures();

#endif
 
	return 0;
 }