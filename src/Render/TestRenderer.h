//
// Created by KingSun on 2018/06/01
//

#ifndef KENGINE_TEST_RENDERER_H
#define KENGINE_TEST_RENDERER_H

//just for test.

#include "./Renderer.h"
#include "../Core/Light/PointLight.h"
#include "../Core/Camera/Camera.h"
#include "../Object/Plane.h"

namespace KEngine {
	namespace KRenderer {
		using tvec3 = KVector::Vec3;

		class TestRenderer : public Renderer {
		private:
			Shader* testShader;
			KLight::PointLight* light;
			KCamera::Camera* camera;
			KObject::Plane* plane1;
			KObject::Plane* plane2;

		public:
			TestRenderer() : Renderer(RES_PATH + "phong.vert", RES_PATH + "phong.frag", "test") {
				testShader = new Shader(RES_PATH + "test.vert", RES_PATH + "test.frag");
				light = new KLight::PointLight(tvec3(4, 0, 0));
				camera = new KCamera::Camera(60, 1000.0f / 700.0f, 0.1, 1000);
				camera->rotateView(45, tvec3(-1, 0, 0));
				camera->setPosition(tvec3(0, 1, 1));

				plane1 = new KObject::Plane();
				plane1->addTexture(RES_PATH + "stone.png");
				plane1->translate(tvec3(0, 0, -0.5));
				plane2 = new KObject::Plane();
				plane2->rotate(90, tvec3(-1, 0, 0));
				plane2->translate(tvec3(0, -0.5, 0));
			}
			~TestRenderer()override {
				delete testShader;
				delete light;
				delete camera;
				delete plane1;
				delete plane2;
			}

			void exec() override {
				glEnable(GL_DEPTH_TEST);
				glEnable(GL_STENCIL_TEST);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

				KObject::Object3D::bindUniform(shader);
				KCamera::Camera::bindUniform(shader);
				KMaterial::Material::bindUniform(shader);
				KLight::Light::bindUniform(shader);
				light->bind(0);

				KObject::Object3D::bindUniform(testShader);
				KCamera::Camera::bindUniform(testShader);
				//Remember to bind shader later.

#if IMGUI_ENABLE
				Kboolean movable = false;
				Kboolean resizable = false;
				Kboolean layout_mode = true;
				Kboolean en_light = true;
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
					ImGui::Checkbox("light", &en_light);
					ImGui::Text("Your screen now is %.2f fps.", ImGui::GetIO().Framerate);
					ImGui::Text("Your mouse pos is %.0f, %.0f", now_mouse.x, now_mouse.y);
					if (ImGui::Button("Exit")) window->closeWindow();

					ImGui::End();
					ImGui::Render();
					ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

					if (layout_mode && !movable) {
						glViewport(0, 0, wSize.x - 300, wSize.y);
						camera->setPerspective(60, Kfloat(wSize.x - 300) / Kfloat(wSize.y), 0.1f, 1000.0f);
					}
					else {
						glViewport(0, 0, wSize.x, wSize.y);
						camera->setPerspective(60, Kfloat(wSize.x) / Kfloat(wSize.y), 0.1f, 1000.0f);
					}
					camera->bind();
					light->active(en_light);
#endif
					//} else {
					//	glViewport(0, 0, wSize.x, wSize.y);
					//}

					shader->bind();
					plane1->render(shader ,testShader);

					testShader->bind();
					plane2->render();

					window->update();
				}
			}

			void resize(Kint w, Kint h)override {
				Renderer::resize(w, h);
				camera->setPerspective(60, Kfloat(w) / Kfloat(h), 0.1f, 1000.0f);
			}
		};
	}
}

#endif // !KENGINE_TEST_RENDERER_H

