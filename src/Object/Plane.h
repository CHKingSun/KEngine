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
			std::vector<tvec2>* vertices; //left z to default(0)
			std::vector<tvec2>* tex_coord;
			std::vector<Face<Kuint>>* indices;
			Kuint count;
			KMaterial::Material* material;

			void generate(Kfloat width, Kfloat height, Kuint xslices, Kuint yslices) {
				Kfloat px = -width / 2.0f;
				Kfloat py = -height / 2.0f;
				Kfloat perx = width / xslices;
				Kfloat pery = height / yslices;
				for (int i = 0; i < xslices; ++i) {
					for (int j = 0; j < yslices; ++j) {
						Kfloat x = px + perx * i;
						Kfloat y = py + pery * j;

						vertices->emplace_back(x, y);
						vertices->emplace_back(x + perx, y);
						vertices->emplace_back(x + perx, y + pery);
						vertices->emplace_back(x, y + pery);

						tex_coord->emplace_back(0.0f, 0.0f);
						tex_coord->emplace_back(1.0f, 0.0f);
						tex_coord->emplace_back(1.0f, 1.0f);
						tex_coord->emplace_back(0.0f, 1.0f);

						Kuint t = (i * xslices + j) * 4;
						indices->emplace_back(t, t + 1, t + 2);
						indices->emplace_back(t + 2, t + 3, t);
					}
				}

				//for (auto it : *vertices) {
				//	std::cout << it << std::endl;
				//}
				//for (auto it : *indices) {
				//	std::cout << it << std::endl;
				//}
			}

			void initArray() {
				vao = new KBuffer::VertexArray();
				vao->allocate(new KBuffer::VertexBuffer(vertices->data(), sizeof(tvec2) * count * 4), A_POSITION, 2, GL_FLOAT);
				vao->allocate(new KBuffer::VertexBuffer(tex_coord->data(), sizeof(tvec2) * count * 4), A_TEXCOORD, 2, GL_FLOAT);
				vao->setVertexAttrib3f(A_NORMAL, tvec3(0.0f, 1.0f, 0.0f));
				ibo = new KBuffer::VertexBuffer(indices->data(), sizeof(Kuint) * count * 6, KBuffer::BufferType::INDEX);
			}

		public:
			Plane(Kfloat width = 1.0f, Kfloat height = 1.0f,
				Ksize xslices = 1, Ksize yslices = 1): Object3D("Plane"),
				vertices(nullptr), tex_coord(nullptr), indices(nullptr){
				count = xslices * yslices;
				vertices = new std::vector<tvec2>();
				vertices->reserve(count * 4);
				tex_coord = new std::vector<tvec2>();
				tex_coord->reserve(count * 4);
				indices = new std::vector<Face<Kuint>>();
				indices->reserve(count * 2);
				material = new KMaterial::Material();
				material->addTexture(RES_PATH + "wall_texture.jpg");

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

			void bind()const override {
				Object3D::bind();
				material->bind();
			}

			void bindTextures(const KRenderer::Shader* shader)const {
				material->bindTextures(shader);
			}

			void render()const override {
				bind();

				glDrawElements(GL_TRIANGLES, count * 6, GL_UNSIGNED_INT, nullptr);

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
