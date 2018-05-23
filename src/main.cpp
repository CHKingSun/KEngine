 /**
  * just for test
  * */
 
#include <iostream>
#include <iomanip>
#include <string>
#include <GL/glew.h>

#include "./KHeader.h"
#include "./Log.h"
#include "./Window.h"
#include "./Object/Plane.h"
#include "./Object/Maze.h"
#include "./Render/Shader.h"
#include "./Core/Matrix/Quaternion.h"
#include "./Core/Camera/Camera.h"
#include "./Core/Camera/FirstCamera.h"
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
	using namespace KEngine::KMaterial;

	auto window = new KEngine::KWindow::Window("KEngine", 1000, 700);
	auto shader = new Shader(RES_PATH + "phong.vert", RES_PATH + "phong.frag");
	//auto shader = new Shader(RES_PATH + "base.vert", RES_PATH + "base.frag");

	auto camera = new KCamera::FirstCamera(60, 1.0, 0.1, 1000);
	auto camera2 = new KCamera::Camera(120, 1.0, 0.1, 1000);

	auto light = new PointLight(Vec3(0, 5, 0));
	auto light1 = new SpotLight(Vec3(0, 6, 0), Vec3(0, -1, 1));
	auto light2 = new SpotLight(Vec3(0, 2, 0), Vec3(1, -3, 0));
	light2->innerCutOff = 0.999978;
	light2->outerCutOff = 0.999;

	auto plane = new KEngine::KObject::Plane(30, 20, 1, 1);
	auto plane1 = new KEngine::KObject::Plane(30, 20, 3, 2);
	auto plane2 = new KEngine::KObject::Plane(20, 10, 2, 1);

	shader->bind();
	KObject::Object3D::bindUniform(shader);
	KMaterial::Material::bindUniform(shader);
	KCamera::Camera::bindUniform(shader);

	//plane->setPosition(Vec3(0, 5, -10));
	//auto material = new KMaterial::Material(/*KMaterial::Color(0.4, 0.4, 0.4, 1.0)*/);
	//material->addTexture(RES_PATH + "wall_texture.jpg");
	//plane->setMaterial(material);

	//plane1->addTexture(RES_PATH + "stone.png", KMaterial::TextureType::DIFFUSE);
	//plane1->addTexture(RES_PATH + "stone.png");
	//plane1->setRotation(-90, Vec3(1, 0, 0));
	//plane1->setPosition(Vec3(0, -5, 0));

 //	plane2->setRotation(30, Vec3(-1, 0, 0));
 //	plane2->setPosition(Vec3(0, -2.5, -7.85));
	//auto material1 = new Material(GREY, GREY, WHITE, 20);
	//material1->addTexture(RES_PATH + "floor.jpg", KMaterial::TextureType::SPECULAR);
	//material1->addTexture(RES_PATH + "floor.jpg");
 //	plane2->setMaterial(material1);

	camera->rotateView(90, Vec3(0, -1, 0));
	camera2->rotateView(90, Vec3(-1, 0, 0));

	Kint w = 20, h = 20;
	auto maze = new KObject::Maze(w, h);
	camera2->setPosition(Vec3(0, (w > h ? w : h) / 2.0, 0));
	camera->setPosition(maze->getStartPosition());
	light2->setPosition(Vec3(0, 1, 0) += camera->getPosition());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

	//light->bind(shader, 0);
	//light1->bind(shader, 0);
	light2->bind(shader, 1);
	Vec2 last_pos = window->getMouse();
	Vec2 now_pos;
	Vec2 screenSize;
	Kboolean change_camera = false;
	Kfloat speed = 0.03;

#if IMGUI_ENABLE
	Kboolean movable = false;
	Kboolean resizable = false;
	Kboolean layout_mode = true;
#endif

	while (!window->closed()) {
		window->clear();

		now_pos = window->getMouse();
		screenSize = window->getWindowSize();

		if (!window->actived()) {
#if IMGUI_ENABLE
			glStencilMask(0x00);
			ImGui_ImplGlfwGL3_NewFrame();
			ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
			if (!movable) flags |= ImGuiWindowFlags_NoMove;
			if (!resizable) flags |= ImGuiWindowFlags_NoResize;
			ImGui::Begin("GUI", nullptr, flags);
			ImGui::SetWindowFontScale(1.2);
			if (!movable) {
				if (layout_mode) ImGui::SetWindowPos(ImVec2(screenSize.x - 300, 0));
				else ImGui::SetWindowPos(ImVec2(0, 0));
			}
			if (!resizable) {
				if (layout_mode) ImGui::SetWindowSize(ImVec2(300, screenSize.y));
				else ImGui::SetWindowSize(ImVec2(screenSize.x, 200));
			}
			ImGui::Checkbox("movable", &movable);
			ImGui::SameLine(100);
			ImGui::Checkbox("resizable", &resizable);
			ImGui::SameLine(200);
			ImGui::Checkbox("layout", &layout_mode);
			ImGui::Text("Your screen now is %.2f fps.", ImGui::GetIO().Framerate);
			ImGui::Text("Your mouse pos is %.0f, %.0f", now_pos.x, now_pos.y);

			ImGui::Checkbox("camera", &change_camera);
			ImGui::InputInt("width", &w);
			ImGui::InputInt("height", &h);
			if (ImGui::Button("OK")) {
				if (w < 2 && h < 2) {
					ImGui::Text("width or height is less than 2!");
				}
				else {
					maze->resetMaze(w, h);
					camera2->setPosition(Vec3(0, (w > h ? w : h) / 2.0, 0));
					camera->setPosition(maze->getStartPosition());
				}
			}
			Vec3 dir = camera->getDirection(KEngine::KCamera::FORWARD);
			ImGui::Text("Your camera direction:\n%.2f, %.2f, %.2f.", dir.x, dir.y, dir.z);
			dir = camera->getDirection(KEngine::KCamera::RIGHT);
			ImGui::Text("Your camera's right:\n%.2f, %.2f, %.2f.", dir.x, dir.y, dir.z);
			if (ImGui::Button("Exit")) window->closeWindow();

			ImGui::End();
			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

			if (layout_mode) glViewport(0, 0, screenSize.x - 300, screenSize.y);
			else glViewport(0, 0, screenSize.x, screenSize.y);
#endif
		} else {
			glViewport(0, 0, screenSize.x, screenSize.y);
		}

		if (window->actived()) {
			if (window->getKeyStatus('W') || window->getKeyStatus(GLFW_KEY_UP)) {
				camera->translate(camera->getDirection(KCamera::DirectionType::FORWARD) *= Vec3(speed, 0, speed));
			}
			else if (window->getKeyStatus('S') || window->getKeyStatus(GLFW_KEY_DOWN)) {
				camera->translate(camera->getDirection(KCamera::DirectionType::BACK) *= Vec3(speed, 0, speed));
			}
			else if (window->getKeyStatus('A') || window->getKeyStatus(GLFW_KEY_LEFT)) {
				camera->translate(camera->getDirection(KCamera::DirectionType::LEFT) *= Vec3(speed, 0, speed));
			}
			else if (window->getKeyStatus('D') || window->getKeyStatus(GLFW_KEY_RIGHT)) {
				camera->translate(camera->getDirection(KCamera::DirectionType::RIGHT) *= Vec3(speed, 0, speed));
			}
			light2->setPosition(Vec3(0, 1, 0) += camera->getPosition());
			light2->bindPosition(shader);
			if (last_pos != now_pos) {
				now_pos -= last_pos;
				Kfloat radius = screenSize.length();
				Kfloat p2xy = now_pos.length();
				//the default center direction is (0, 0, 1) so we just put another direction int to function.
				camera->rotateView(Vec3(-now_pos.x, now_pos.y, KFunction::distance(screenSize, now_pos)));
				//if (now_pos.x != 0) camera->rotateView(Vec3(-now_pos.x, 0, sqrt(radius * radius - now_pos.x * now_pos.x)));
				//if (now_pos.y != 0) camera->rotateView(Vec3(0, now_pos.y, sqrt(radius * radius - now_pos.y * now_pos.y)));
				if (!KFunction::isZero(now_pos.x)) light2->rotate(
					KMatrix::Quaternion(Vec3(0, 0, 1).getAngle(Vec3(now_pos.x, 0, KFunction::distance(screenSize, now_pos))),
						Vec3(0, -now_pos.x, 0)));
				light2->bindDirection(shader);
				last_pos += now_pos;
			}
		} else {
			last_pos = now_pos;
		}
		if (window->getKeyStatus('V')) {
			change_camera = !change_camera;
		}
		if (!change_camera) camera->bind();
		else camera2->bind();

		//light1->rotate(3, Vec3(0, 1, 0));
		//light1->bindDirection(shader);

		maze->render(shader);

		//plane->render(shader);
		//plane1->render(shader);
		//plane2->render(shader);

		window->update();
	}

	delete plane;
	delete plane1;
	delete plane2;
	delete camera;
	delete light;
	delete light1;
	delete maze;
	delete shader;
	delete window;
	KMaterial::Texture::deleteAllTextures();

#endif
 
	return 0;
 }