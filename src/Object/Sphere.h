//
// Created by KingSun on 2018/05/24
//

#ifndef KENGINE_SPHERE_H
#define KENGINE_SPHERE_H

#include <cmath>
#include <vector>
#include "../KHeader.h"
#include "../Core/Material/Material.h"
#include "./Object3D.h"
#include "./Face.h"

namespace KEngine {
	namespace KObject {
		class Sphere : public Object3D {
			using tvec3 = KVector::Vec3;
			using tvec2 = KVector::Vec2;

		private:
			Kfloat radius;
			Ksize count;

			std::vector<tvec3>* vertices; //remember sphere's normal is its position when it center is (0, 0, 0)
			std::vector<tvec2>* texcoords;
			std::vector<Kushort>* indices; //Kushort can just store 65526 vertices(with 180 aslices and 180 rsclices)

			KMaterial::Material* material;

			void generate(Ksize aslices, Ksize rslices) {
				count = ((rslices - 1) * 4 + 1) * aslices + 1;
				Ksize v_count = (aslices + 1) * (rslices - 1) * 2 + 2;
				vertices = new std::vector<tvec3>();
				vertices->reserve(v_count);
				texcoords = new std::vector<tvec2>();
				texcoords->reserve(v_count);
				indices = new std::vector<Kushort>();
				indices->reserve(count);

				Kfloat per_xangle = PI * 2.0f / aslices;
				Kfloat per_yangle = PI / (rslices * 2.0f);
				Kfloat pertx = 1.0f / aslices;
				Kfloat perty = 0.5f / rslices;
				Kfloat yangle = PI - per_yangle; //form bottom to top(except two poles).
				Kfloat ty = 1.0f - perty;
				for (int i = 0; i < ((rslices - 1) << 1); ++i, yangle -= per_yangle, ty -= perty) {
					Kfloat y = radius * cos(yangle);
					Kfloat r = radius * sin(yangle);
					Kfloat xangle = 0; //draw a circle
					Kfloat tx = 0;
					for (int j = 0; j <= aslices; ++j, xangle += per_xangle, tx += pertx) {
						vertices->emplace_back(cos(xangle) * r, y, sin(xangle) * r);
						texcoords->emplace_back(1.0 - tx, ty);
					}
				}
				vertices->emplace_back(0, -radius, 0); //bottom pole [v_count - 2]
				texcoords->emplace_back(1, 1);
				vertices->emplace_back(0, radius, 0); //top pole [v_count - 1]
				texcoords->emplace_back(0, 0);

				int rt = aslices + 1;
				indices->emplace_back(v_count - 2);
				Ksize index = 0;
				for (Ksize at = 0; at < aslices; ++at, ++index) {
					for (int i = 0; i < ((rslices - 1) << 1); ++i, index += rt) {
						indices->emplace_back(index);
						indices->emplace_back(index + 1);
					}
					index -= rt;
					if (rt > 0) indices->emplace_back(v_count - 1);
					else indices->emplace_back(v_count - 2);
					rt = -rt;
				}
			}

			void initArray() {
				vao = new KBuffer::VertexArray();

				vbo = new KBuffer::VertexBuffer(vertices->size() * sizeof(tvec3), vertices->data());
				vao->allocate(vbo, A_POSITION, 3, GL_FLOAT);

				tbo = new KBuffer::VertexBuffer(texcoords->size() * sizeof(tvec2), texcoords->data());
				vao->allocate(tbo, A_TEXCOORD, 2, GL_FLOAT);

				vao->allocate(vbo, A_NORMAL, 3, GL_FLOAT);

				ibo = new KBuffer::VertexBuffer(indices->size() * sizeof(Kushort),
					indices->data(), KBuffer::INDEX);

				delete vertices; vertices = nullptr;
				delete texcoords; texcoords = nullptr;
				delete indices; indices = nullptr;
			}

		public:
			Sphere(Kfloat radius = 1.0f, Ksize aslices = 20, Ksize rslices = 20):
				Object3D("sphere"), radius(radius), vertices(nullptr), texcoords(nullptr),
				indices(nullptr), material(nullptr) {
				material = new KMaterial::Material();
				material->addTexture(RES_PATH + "earth.jpg");
				material->addTexture(RES_PATH + "earth.jpg", KMaterial::SPECULAR);
				generate(aslices, rslices);
				initArray();
			}

			~Sphere()override {
				delete vertices;
				delete texcoords;
				delete indices;
				delete material;
			}

			void bind()const override {
				Object3D::bind();
				if(material != nullptr) material->bind();
			}

			void unBind()const override {
				Object3D::unBind();
				if (material != nullptr) material->activeTextures(false);
			}

			void render(const KRenderer::Shader* shader = nullptr,
				const KRenderer::Shader* contourShader = nullptr)const override {
				bind(); 

				if (contourShader != nullptr) {
					glClear(GL_STENCIL_BUFFER_BIT);
					glStencilFunc(GL_ALWAYS, 1, 0xFF);
					glStencilMask(0xFF);
				}
				glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_SHORT, nullptr);

				if (contourShader != nullptr) {
					contourShader->bind();
					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
					glStencilMask(0x00);
					glLineWidth(3.0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElements(GL_TRIANGLE_STRIP, count, GL_UNSIGNED_SHORT, nullptr);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glStencilMask(0xFF);
					shader->bind();
				}

				unBind();
			}

			Kfloat getRadius()const {
				return radius;
			}

			Ksize getCount()const override {
				return count;
			}

#if IMGUI_ENABLE
			void drawGui() {
				tvec3 dir = rotation * tvec3(1, 0, 0);
				ImGui::Text("Your sphere's direction:\n%.2f, %.2f, %.2f.", dir.x, dir.y, dir.z);
			}
#endif // IMGUI_ENABLE

		};
	}
}

#endif //KENGINE_SPHERE_H
