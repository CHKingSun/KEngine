//
// Created by KingSun on 2018/05/27
//

#ifndef KENGINE_MAZE_RENDERER_H
#define KENGINE_MAZE_RENDERER_H

#include "../KHeader.h"
#include "../Window.h"
#include "./Shader.h"
#include "./Renderer.h"
#include "../Object/Maze.h"
#include "../Object/Sphere.h"
#include "../Core/Camera/Camera.h"
#include "../Core/Camera/FirstCamera.h"
#include "../Core/Camera/ThirdCamera.h"
#include "../Core/Light/PointLight.h"
#include "../Core/Light/SpotLight.h"
#include "../Core/Vector/Vec3.h"

namespace KEngine {
	namespace KRenderer {
		using tvec2 = KVector::Vec2;
		using tvec3 = KVector::Vec3;
		class MazeRenderer : public Renderer {

		private:
			KCamera::Camera* viewCamera;
			KCamera::FirstCamera* firstCamera;
			KCamera::ThirdCamera* thirdCamera;

			KLight::PointLight* viewLight;
			KLight::SpotLight* assistLight;
			KLight::SpotLight* followLight;

			KObject::Maze* maze;
			KObject::Sphere* sphere;

			Ksize mWidth, mHeight;
			tvec2 last_mouse;
			Kubyte change_camera;

			Kboolean success;

			void keyEvent(Kint key, Kint action)override {
				Renderer::keyEvent(key, action);
				if (key == 'V' && action == GLFW_PRESS) {
					change_camera = (++change_camera) % 3;
				}
			}

			void mouseWheelEvent(Kdouble yoffset)override {
				Renderer::mouseWheelEvent(yoffset);
				if (yoffset > 0 && change_camera == 2) {
					viewCamera->translate(tvec3(0, -0.3f, 0));
				}
				else if (yoffset < 0 && change_camera == 2) {
					viewCamera->translate(tvec3(0, 0.3f, 0));
				}
			}

			void move() {
				static tvec3 speed(0.03, 0, 0.03);

				Kboolean flag = false;
				if (keys['W'] || keys[GLFW_KEY_UP]) {
					sphere->translate(firstCamera->getDirection(
						KCamera::DirectionType::FORWARD) *= speed, true);
					flag = true;
				}
				else if (keys['S'] || keys[GLFW_KEY_DOWN]) {
					sphere->translate(firstCamera->getDirection(
						KCamera::DirectionType::BACK) *= speed, true);
					flag = true;
				}
				else if (keys['A'] || keys[GLFW_KEY_LEFT]) {
					sphere->translate(firstCamera->getDirection(
						KCamera::DirectionType::LEFT) *= speed, true);
					flag = true;
				}
				else if (keys['D'] || keys[GLFW_KEY_RIGHT]) {
					sphere->translate(firstCamera->getDirection(
						KCamera::DirectionType::RIGHT) *= speed, true);
					flag = true;
				}

				if (flag) {
					sphere->translate(maze->circleCollisionDetection(
						sphere->getPosition(), sphere->getRadius()));
					firstCamera->setPosition(sphere->getPosition());
				}
			}

			Kboolean judgePass() {
				if (maze->isExit(sphere->getPosition(), sphere->getRadius())) {
					std::cout << "You pass the maze!" << std::endl;
					setActived(false);
					success = true;
					return true;
				}
				return false;
			}

		public:
			MazeRenderer(std::string title, Ksize swidth = 1000, Ksize sheight = 700,
				Ksize mwidth = 20, Ksize mheight = 20): mWidth(mwidth), mHeight(mheight),
				Renderer(RES_PATH + "phong.vert", RES_PATH + "phong.frag", title, swidth, sheight),
				success(false) {
				Renderer::hideMouse();

				KObject::Object3D::bindUniform(shader);
				KMaterial::Material::bindUniform(shader);
				KCamera::Camera::bindUniform(shader);
				KLight::Light::bindUniform(shader);
				shader->bind();

				viewLight = new KLight::PointLight(tvec3(0, 6, 0));
				assistLight = new KLight::SpotLight(tvec3(0, 6, 0), tvec3(0, -1, -1));
				followLight = new KLight::SpotLight(tvec3(0, 2, 0), tvec3(1, -3, 0));

				viewCamera = new KCamera::Camera(120, 1.0, 0.1, 1000);
				viewCamera->rotateView(90, tvec3(-1, 0, 0));
				firstCamera = new KCamera::FirstCamera(60, Kfloat(swidth) / Kfloat(sheight), 0.1, 1000);
				firstCamera->rotateView(90, tvec3(0, -1, 0));
				thirdCamera = new KCamera::ThirdCamera(60, Kfloat(swidth) / Kfloat(sheight), 0.1, 1000, 45, 0.48);
				thirdCamera->rotateCamera(90, tvec3(0, -1, 0));

				sphere = new KObject::Sphere(0.18, 60, 60);
				thirdCamera->setTarget(sphere);

				maze = new KObject::Maze(mwidth, mheight);
				viewCamera->setPosition(tvec3(0, (mwidth > mheight ? mwidth : mheight) / 2.0f, 0));
				firstCamera->setPosition(maze->getStartPosition());
				sphere->setPosition(maze->getStartPosition() + tvec3(0, -0.3f, 0));
				followLight->setPosition(thirdCamera->getPosition() + tvec3(0, 2, 0));
			}
			~MazeRenderer() override {
				delete sphere;
				delete maze;
				delete followLight;
				delete assistLight;
				delete viewLight;
				delete thirdCamera;
				delete firstCamera;
				delete viewCamera;
			}

			void exec() override {
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_STENCIL_TEST);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);

				auto testShader = new Shader(RES_PATH + "test.vert", RES_PATH + "test.frag");

				KObject::Object3D::bindUniform(testShader);
				KCamera::Camera::bindUniform(testShader);

				viewLight->bind(0);
				assistLight->bind(0);
				followLight->bind(1);

				last_mouse = mouse_pos;
				tvec3 speed(0.03, 0, 0.03);
				Kboolean pass_gui = false;
#if IMGUI_ENABLE
				Kboolean movable = false;
				Kboolean resizable = false;
				Kboolean layout_mode = true;
#endif

				while (!window->closed()) {
					window->clear();

					tvec2 now_mouse(mouse_pos);
					tvec2 wSize(window->getWindowSize());
					//if (!window->actived()) {
#if IMGUI_ENABLE
						glStencilMask(0x00);
						ImGui_ImplGlfwGL3_NewFrame();
						ImGuiWindowFlags flags = ImGuiWindowFlags_HorizontalScrollbar;
						if (!movable) flags |= ImGuiWindowFlags_NoMove;
						if (!resizable) flags |= ImGuiWindowFlags_NoResize;
						ImGui::Begin("GUI", nullptr, flags);
						ImGui::SetWindowFontScale(1.2);
						if (!movable) {
							if (layout_mode) ImGui::SetWindowPos(ImVec2(wSize.x - 300, 0));
							else ImGui::SetWindowPos(ImVec2(0, 0));
						}
						if (!resizable) {
							if (layout_mode) ImGui::SetWindowSize(ImVec2(300, wSize.y));
							else ImGui::SetWindowSize(ImVec2(wSize.x, 200));
						}
						ImGui::Checkbox("movable", &movable);
						ImGui::SameLine(100);
						ImGui::Checkbox("resizable", &resizable);
						ImGui::SameLine(200);
						ImGui::Checkbox("layout", &layout_mode);
						ImGui::Text("Your screen now is %.2f fps.", ImGui::GetIO().Framerate);
						ImGui::Text("Your mouse pos is %.0f, %.0f", now_mouse.x, now_mouse.y);

						ImGui::InputInt("width", (Kint*)&mWidth);
						ImGui::InputInt("height", (Kint*)&mHeight);
						if (ImGui::Button("OK")) {
							if (mWidth < 2 && mHeight < 2) {
								ImGui::Text("width or height is less than 2!");
							}
							else {
								success = false;
								maze->resetMaze(mWidth, mHeight);
								viewCamera->setPosition(tvec3(0, (mWidth > mHeight ? mWidth : mHeight) / 2.0f, 0));
								firstCamera->setPosition(maze->getStartPosition());
								sphere->setPosition(maze->getStartPosition() + tvec3(0, -0.3f, 0));
								followLight->setPosition(thirdCamera->getPosition() + tvec3(0, 2, 0));
								followLight->bindPosition();
							}
						}
						tvec3 dir = thirdCamera->getDirection(KEngine::KCamera::FORWARD);
						ImGui::Text("Your camera's direction:\n%.2f, %.2f, %.2f.", dir.x, dir.y, dir.z);
						ImGui::Text("Your angle is: %.2f", dir.getAngle(KVector::Vec3(0, 1, 0)));
						sphere->drawGui();
						if (ImGui::Button("Exit")) window->closeWindow();

						if (pass_gui) {
							ImGui::SetWindowFontScale(1.8f);

							ImGui::Text("You pass the maze!");
							if (ImGui::Button("I Know")) pass_gui = false;
						}

						ImGui::End();

						ImGui::Render();
						ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

						if (layout_mode && !movable) {
							glViewport(0, 0, wSize.x - 300, wSize.y);
							firstCamera->setPerspective(60, Kfloat(wSize.x - 300) / Kfloat(wSize.y), 0.1, 1000);
							thirdCamera->setPerspective(60, Kfloat(wSize.x - 300) / Kfloat(wSize.y), 0.1, 1000);
						} else {
							glViewport(0, 0, wSize.x, wSize.y);
							firstCamera->setPerspective(60, Kfloat(wSize.x) / Kfloat(wSize.y), 0.1, 1000);
							thirdCamera->setPerspective(60, Kfloat(wSize.x) / Kfloat(wSize.y), 0.1, 1000);
						}
#endif
					//} else {
					//	glViewport(0, 0, wSize.x, wSize.y);
					//}

					if (window->actived()) {
						if (!success && judgePass()) {
							pass_gui = true;
						}

						if (last_mouse != now_mouse) {
							now_mouse -= last_mouse;
							//the default center direction is (0, 0, 1) so we just put another direction int to function.
							firstCamera->rotateView(tvec3(-now_mouse.x, now_mouse.y, KFunction::distance(wSize, now_mouse)));
							thirdCamera->rotateView(tvec3(-now_mouse.x, now_mouse.y, KFunction::distance(wSize, now_mouse)));
							if (!KFunction::isZero(now_mouse.x)) {
								followLight->rotate(tvec3(0, 0, 1).getAngle(tvec3(now_mouse.x, 0,
									KFunction::distance(wSize, now_mouse))),
									tvec3(0, -now_mouse.x, 0));
							}
							followLight->bindDirection();
							last_mouse += now_mouse;
						}

						move();

						followLight->setPosition(tvec3(0, 2, 0) += thirdCamera->getPosition());
						followLight->bindPosition();
					} else {
						last_mouse = now_mouse;
					}

					if (change_camera == 0) thirdCamera->bind();
					else if (change_camera == 1) firstCamera->bind();
					else if (change_camera == 2) viewCamera->bind();

					assistLight->rotate(1.5, tvec3(0, 1, 0));
					assistLight->bindDirection();

					if (change_camera == 0) sphere->render(shader, testShader);
					maze->render(shader, testShader);

					window->update();
				}
			}

			void resize(Kint w, Kint h)override {
				Renderer::resize(w, h);
				firstCamera->setPerspective(60, Kfloat(w) / Kfloat(h), 0.1, 1000);
				thirdCamera->setPerspective(60, Kfloat(w) / Kfloat(h), 0.1, 1000);
			}
		};
	}
}

#endif // !KENGINE_MAZE_RENDERER_H
