//
// Created by KingSun on 2018/05/18
//

#ifndef KENGINE_MAZE_H
#define KENGINE_MAZE_H

#include <vector>

#include "../KHeader.h"
#include "../Core/Vector/Vec2.h"
#include "../Core/Vector/Vec3.h"
#include "../Core/Vector/Vec4.h"
#include "../Core/Matrix/Mat4.h"
#include "../Core/transform.h"
#include "../Core/Material/Material.h"
#include "./Face.h"
#include "./Object3D.h"

namespace KEngine {
	namespace KObject {
		//a maze with mutiple render
		class Maze : public Object3D {
			using tvec2 = KVector::Vec2;
			using tvec3 = KVector::Vec3;
			using tmat4 = KMatrix::Mat4;

		private:
			Ksize width, height;
			tvec3* vertices;
			tvec2* tex_coords;
			tvec3* normals;
			std::vector<Face<Kubyte>>* indices; //we just need 36 point
			std::vector<tmat4>* matrices;
			KBuffer::VertexBuffer* vbo;
			KBuffer::VertexBuffer* tbo;
			KBuffer::VertexBuffer* nbo;
			KBuffer::VertexBuffer* mbo;
			Kuint count;
			KMaterial::Material* material;

			void generate(Kfloat width, Kfloat height, Kfloat depth) {
				Kfloat x = width / 2.0f;
				Kfloat y = height / 2.0f;
				Kfloat z = depth / 2.0f;
				vertices = new tvec3[24]{
					tvec3(-x, -y, z), tvec3(x, -y, z), tvec3(x, y, z), tvec3(-x, y, z),
					tvec3(x, -y, z), tvec3(x, -y, -z), tvec3(x, y, -z), tvec3(x, y, z),
					tvec3(x, -y, -z), tvec3(-x, -y, -z), tvec3(-x, y, -z), tvec3(x, y, -z),
					tvec3(-x, -y, -z), tvec3(-x, -y, z), tvec3(-x, y, z), tvec3(-x, y, -z),
					tvec3(-x, y, z), tvec3(x, y, z), tvec3(x, y, -z), tvec3(-x, y, -z),
					tvec3(x, -y, -z), tvec3(-x, -y, -z), tvec3(-x, -y, z), tvec3(x, -y, z)
				};

				Kfloat max_num = x > y ? x : y;
				max_num = max_num > z ? max_num : z;
				Kfloat tx = x / max_num;
				Kfloat ty = y / max_num;
				Kfloat tz = z / max_num;
				tex_coords = new tvec2[24]{
					tvec2(0, 0), tvec2(tx, 0), tvec2(tx, ty), tvec2(0, ty),
					tvec2(0, 0), tvec2(tz, 0), tvec2(tz, ty), tvec2(0, ty),
					tvec2(0, 0), tvec2(tx, 0), tvec2(tx, ty), tvec2(0, ty),
					tvec2(0, 0), tvec2(tz, 0), tvec2(tz, ty), tvec2(0, ty),
					tvec2(0, 0), tvec2(tx, 0), tvec2(tx, tz), tvec2(0, tz),
					tvec2(0, 0), tvec2(tx, 0), tvec2(tx, tz), tvec2(0, tz),
				};

				normals = new tvec3[24]{
					tvec3(0, 0, 1), tvec3(0, 0, 1), tvec3(0, 0, 1), tvec3(0, 0, 1),
					tvec3(1, 0, 0), tvec3(1, 0, 0), tvec3(1, 0, 0), tvec3(1, 0, 0),
					tvec3(0, 0, -1), tvec3(0, 0, -1), tvec3(0, 0, -1), tvec3(0, 0, -1),
					tvec3(-1, 0, 0), tvec3(-1, 0, 0), tvec3(-1, 0, 0), tvec3(-1, 0, 0),
					tvec3(0, 1, 0), tvec3(0, 1, 0), tvec3(0, 1, 0), tvec3(0, 1, 0),
					tvec3(0, -1, 0), tvec3(0, -1, 0), tvec3(0, -1, 0), tvec3(0, -1, 0)
				};

				indices = new std::vector<Face<Kubyte>>();
				indices->reserve(12);
				indices->emplace_back(0, 1, 2);
				indices->emplace_back(2, 3, 0);
				indices->emplace_back(4, 5, 6);
				indices->emplace_back(6, 7, 4);
				indices->emplace_back(8, 9, 10);
				indices->emplace_back(10, 11, 8);
				indices->emplace_back(12, 13, 14);
				indices->emplace_back(14, 15, 12);
				indices->emplace_back(16, 17, 18);
				indices->emplace_back(18, 19, 16);
				indices->emplace_back(20, 21, 22);
				indices->emplace_back(22, 23, 20);
			}

			void initArray() {
				vao = new KBuffer::VertexArray();

				vbo = new KBuffer::VertexBuffer(vertices, sizeof(tvec3) * 24);
				vao->allocate(vbo, A_POSITION, 3, GL_FLOAT);

				tbo = new KBuffer::VertexBuffer(tex_coords, sizeof(tvec2) * 24);
				vao->allocate(tbo, A_TEXCOORD, 2, GL_FLOAT);

				nbo = new KBuffer::VertexBuffer(normals, sizeof(tvec2) * 24);
				vao->allocate(nbo, A_NORMAL, 3, GL_FLOAT);

				ibo = new KBuffer::VertexBuffer(indices->data(),
					sizeof(Face<Kubyte>) * 12, KBuffer::BufferType::INDEX);

				delete[] vertices; vertices = nullptr;
				delete[] tex_coords; tex_coords = nullptr;
				delete[] normals; normals = nullptr;
				delete indices; indices = nullptr;
			}

			void bindMatrices() {
				if (matrices == nullptr) return;
				delete mbo;
				mbo = new KBuffer::VertexBuffer(matrices->data(),
					matrices->size() * sizeof(tmat4));
				vao->allocate(mbo, A_MATRIX, 4, GL_FLOAT, false, sizeof(tmat4), 0);
				glVertexAttribDivisor(A_MATRIX, 1);
				vao->allocate(mbo, A_MATRIX + 1, 4, GL_FLOAT, false,
					sizeof(tmat4), sizeof(KVector::Vec4));
				glVertexAttribDivisor(A_MATRIX + 1, 1);
				vao->allocate(mbo, A_MATRIX + 2, 4, GL_FLOAT, false,
					sizeof(tmat4), sizeof(KVector::Vec4) * 2);
				glVertexAttribDivisor(A_MATRIX + 2, 1);
				vao->allocate(mbo, A_MATRIX + 3, 4, GL_FLOAT, false,
					sizeof(tmat4), sizeof(KVector::Vec4) * 3);
				glVertexAttribDivisor(A_MATRIX + 3, 1);

				delete matrices; matrices = nullptr;
			}

			bool isEdge(Ksize i, Ksize j) {
				return i == 0 || (j == width + 1 && i != 0) || (i == height + 1 && j != width + 1) ||
					   (j == 0 && i > 1 && i < width + 1);
			}

		public:
			Maze(Ksize w = 20, Ksize h = 20):
				Object3D("Maze"), vertices(nullptr), tex_coords(nullptr),
				normals(nullptr), indices(nullptr), matrices(nullptr), material(nullptr),
				vbo(nullptr), tbo(nullptr), nbo(nullptr), mbo(nullptr),
				width(w), height(h) {
				generate(1.0, 1.0, 1.0);
				initArray();
				resetMaze(w, h);
				material = new KMaterial::Material();
				material->addTexture(RES_PATH + "stone.png");
			}

			~Maze()override {
				delete[] vertices;
				delete[] tex_coords;
				delete[] normals;
				delete indices;
				delete matrices;
				delete material;
				delete vbo;
				delete tbo;
				delete mbo;
			}

			void resetMaze(Ksize w = 20, Ksize h = 20) {
				//note: remember to transpose for our matrix if row-first
				//but in mat4 must be column-first.
				width = w; height = h;
				count = (width + 2) * (height + 2) + (width + height + 4) * 2 - 4 - 1;
				delete matrices;
				matrices = new std::vector<tmat4>();
				matrices->reserve(count);
				Kfloat px = -Kfloat(width + 1) / 2.0;
				Kfloat py = Kfloat(height + 1) / 2.0;
				for (int i = 0; i < height + 2; ++i) {
					for (int j = 0; j < width + 2; ++j) {
						matrices->emplace_back(KFunction::translate(KVector::Vec3(px + j, 0, py - i)).transpose());
						if (isEdge(i, j)) matrices->emplace_back(KFunction::translate(
										  KVector::Vec3(px + j, 1, py - i)).transpose());
					}
				}
				bindMatrices();
			}

			void bind()const override {
				Object3D::bind();
				material->bind();
			}

			void bindTextures(const KRenderer::Shader* shader)const {
				if (shader == nullptr) return;
				material->bindTextures(shader);
			}

			void render(const KRenderer::Shader* shader= nullptr)const override {
				bind();
				bindTextures(shader);
				shader->bindUniform1i("u_is_multiple", true);

				glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr, count);

				//shader->bindUniform1i("u_draw_contour", true);
				//glLineWidth(3.0);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				//glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr, count);
				//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				//shader->bindUniform1i("u_draw_contour", false);

				shader->bindUniform1i("u_is_multiple", false);
				unBindTextures(shader);
				unBind();
			}

			void unBindTextures(const KRenderer::Shader* shader)const {
				if (shader == nullptr) return;
				material->unactiveTextures(shader);
			}

			Ksize getCount()const override {
				return count;
			}

			GLenum getComponent()const override {
				return GL_UNSIGNED_BYTE;
			}
		};
	}
}

#endif //KENGINE_MAZE_H