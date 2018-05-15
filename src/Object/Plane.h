//
// Created by KingSun 0n 2018/05/11
//

#ifndef KENGINE_PLANE_H
#define KENGINE_PLANE_H

#include <vector>
#include "../KHeader.h"
#include "./Object3D.h"
#include "./Face.h"
#include "../Core/Vector/Vec3.h"
#include "../Core/Vector/Vec2.h"
#include "../Core/Material/Material.h"

namespace KEngine {
	namespace KObject {
		using tvec2 = KVector::Vec2;
		using tvec3 = KVector::Vec3;

		//a plane which center is in origin and normal is vec3(0, 1, 0)
		class Plane : public Object3D {
		private:
			tvec2* vertices; //left z to default(0)
			tvec2* tex_coord;
			Face<Kushort>* indices;
			Kuint count;
			KMaterial::Material* material;

			void generate(Kfloat width, Kfloat height, Kuint xslices, Kuint yslices) {
				Kfloat px = -width / 2.0f;
				Kfloat py = -height / 2.0f;
				Kfloat perx = width / xslices;
				Kfloat pery = height / yslices;
				for (int i = 0; i < xslices; ++i) {
					for (int j = 0; j < yslices; ++j) {
						Kuint t = (i * xslices + j) * 4;
						Kfloat offsetx = perx * i;
						Kfloat offsety = pery * j;

						vertices[t] = tvec2(px, py);
						vertices[t + 1] = tvec2(px + offsetx, py);
						vertices[t + 2] = tvec2(px + offsetx, py + offsety);
						vertices[t + 3] = tvec2(px, py + offsety);

						tex_coord[t] = tvec2(0.0f, 0.0f);
						tex_coord[t + 1] = tvec2(1.0f, 0.0f);
						tex_coord[t + 2] = tvec2(1.0f, 1.0f);
						tex_coord[t + 3] = tvec2(0.0f, 1.0f);

						indices[t / 2] = Face<Kushort>(t, t + 1, t + 2);
						indices[t / 2 + 1] = Face<Kushort>(t + 2, t + 3, t);
					}
				}
			}

			void initArray() {
				vao = new KBuffer::VertexArray();
				KBuffer::VertexBuffer(vertices, sizeof(tvec2) * count * 4);
				vao->allocate(new KBuffer::VertexBuffer(vertices, sizeof(tvec2) * count * 4), A_POSITION, 2, GL_FLOAT);
				KBuffer::VertexBuffer tbo(tex_coord, sizeof(tvec2) * count * 4);
				vao->allocate(new KBuffer::VertexBuffer(tex_coord, sizeof(tvec2) * count * 4), A_TEXCOORD, 2, GL_FLOAT);
				vao->setVertexAttrib3f(A_NORMAL, tvec3(0.0f, 1.0f, 0.0f));
				ibo = new KBuffer::VertexBuffer(indices, sizeof(Kushort) * count * 2, KBuffer::BufferType::INDEX);
				vao->setVertexAttrib3f(A_NORMAL, KVector::Vec3(0, 1, 0));
			}

		public:
			Plane(Kfloat width = 1.0f, Kfloat height = 1.0f,
				Ksize xslices = 1, Ksize yslices = 1): Object3D("Plane") {
				count = xslices * yslices;
				vertices = new tvec2[count * 4];
				tex_coord = new tvec2[count * 4];
				indices = new Face<Kushort>[count * 2];
				material = new KMaterial::Material();

				generate(width, height, xslices, yslices);
				initArray();
			}
			~Plane()override {
				delete material;
				delete vertices;
				delete tex_coord;
				delete indices;
			}

			void setMaterial(KMaterial::Material* material) {
				if (material != nullptr) {
					delete this->material;
					this->material = material;
				}
			}

			void bindUniform(const KRenderer::Shader* shader)const override {
				Object3D::bindUniform(shader);
				material->bind(shader);
			}

			void render()const override {
				bind();

				glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_SHORT, nullptr);

				unBind();
			}

			Ksize getCount()const override {
				return count * 6;
			}
			GLenum getComponent()const override {
				return GL_UNSIGNED_SHORT;
			}
		};
	}
}

#endif //KENGINE_PLANE_H
