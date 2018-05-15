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
#include "./Render/Renderer.h"
#include "./Render/Buffer/BackBuffer.h"

const std::string ROOT_PATH = "./";

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

	auto *window = new KEngine::KWindow::Window("KEngine");
	auto *renderer = new KEngine::KRenderer::Renderer(ROOT_PATH + "res/base.vert", ROOT_PATH + "res/base.frag");
	//auto *renderer = new KEngine::KRenderer::Renderer(ROOT_PATH + "res/vertex.glsl", ROOT_PATH + "res/fragment.glsl");
	auto *plane = new KEngine::KObject::Plane();
	renderer->addObject(plane);
	plane->translate(KEngine::KVector::Vec3(0.2, 0.2, 0.0));
	//plane->reBindScale();
	//plane = nullptr;

#if 1
	auto vertices = new Kfloat[8]{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	auto texCoords = new Kfloat[8]{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	auto indices = new Kushort[6]{
		0, 1, 2,
		2, 3, 0
	};
#else
	Kfloat vertices[8] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f,  0.5f,
		-0.5f,  0.5f
	};

	Kfloat texCoords[8] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	Kushort indices[6] = {
		0, 1, 2,
		2, 3, 0
	};
#endif

	/*GLuint vao, vbo, tbo, ibo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	std::cout << sizeof(Kfloat) * 8 << ", " << sizeof(vertices) << std::endl;
	std::cout << sizeof(Kushort) * 6 << ", " << sizeof(indices) << std::endl;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Kfloat) * 8, vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &tbo);
	glBindBuffer(GL_ARRAY_BUFFER, tbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Kfloat) * 8, texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Kushort) * 6, indices, GL_STATIC_DRAW);*/

	glCall(auto vao = new VertexArray(););
	glCall(vao->allocate(new VertexBuffer(vertices, sizeof(Kfloat) * 8), 1, 2, GL_FLOAT));
	glCall(vao->allocate(new VertexBuffer(texCoords, sizeof(Kfloat) * 8), 3, 2, GL_FLOAT));
	glCall(auto ibo = new VertexBuffer(indices, sizeof(Kushort) * 6, KEngine::KBuffer::INDEX));

	{
		glEnable(GL_RASTERIZER_DISCARD);
		auto backShader = new KRenderer::Shader();
		backShader->addShader(GL_VERTEX_SHADER, ROOT_PATH + "res/base.vert");
		std::vector<std::string> varyings = {
			"b_pos",
			"b_scale",
			"b_rotate"
			//"b_out"
		};
		using OutData = /*KMatrix::Mat4; //*/KVector::Vec3;
		auto tfbo = new BackBuffer(renderer->getShader(), varyings, sizeof(OutData) * 5 * 6);
		backShader->bind();
		tfbo->bind();
		glCall(;)

		vao->bind();
		vao->enableVertexArray();
		ibo->bind();
		tfbo->enable(GL_TRIANGLES);
		glPointSize(10.0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
		tfbo->disable();
		vao->disableVertexArray();

		const auto data = tfbo->getData<OutData>();
		Kuint size = tfbo->getBufferSize() / sizeof(OutData);
		for (int i = 0; i < size; ++i) {
			std::cout << data[i] << std::endl;
		}
		
		delete tfbo;
		delete backShader;
		glDisable(GL_RASTERIZER_DISCARD);
	}

	while (!window->closed())
	{
		window->clear();
		renderer->render();

		vao->bind();
		vao->enableVertexArray();
		ibo->bind();

		plane->setRotation(window->getRunTime(), KVector::Vec3(1, 0, 0));
		plane->reBindRotation();

		//glBindVertexArray(vao);
		//glEnableVertexAttribArray(1);
		//glEnableVertexAttribArray(2);
		//glEnableVertexAttribArray(3);
		//glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		glPointSize(10.0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);

		vao->disableVertexArray();

		window->update();
	}

	//glCall(std::cin.get();)

	delete renderer;
	delete window;

#endif
 
	return 0;
 }