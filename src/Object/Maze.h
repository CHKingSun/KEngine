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
			Kuint count;

			tvec3* vertices;
			tvec2* tex_coords;
			tvec3* normals;
			std::vector<Face<Kubyte>>* indices; //we just need 36 point
			std::vector<tmat4>* matrices;

			KBuffer::VertexBuffer* mbo;

			KMaterial::Material* material;

			Kboolean test;

			Ksize entry, exit;
			Kboolean* maze;
			//We can use a stack to save the track later.

			void generateBox(Kfloat width, Kfloat height, Kfloat depth) {
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

			Ksize generateMaze() {
				delete maze;
				maze = new Kboolean[width * height];
				memset(maze, 0, width * height); //sizeof(bool) = sizeof(char) = 1
				int a; //generate entry of the maze.
				srand(time(nullptr));
				do { a = rand() % (height - 1); } while (!a); //do not use 0 or width-1 column as the entry.
				entry = width * a;
				maze[entry] = true;
				exit = 0;
				return width * height - 1 - mazeGenerator(entry + 1);
			}

			Ksize mazeGenerator(Ksize now_mouse) {
				maze[now_mouse] = true;

				int a[4] = { 0, 1, 2, 3 };//0 = right; 1 = up; 2 = left; 3 = down.
				for (int i = 0; i < 4; i++) { //for random direction.
					int j = rand() % 4;
					if (i == j) {
						i--; continue;
					}
					int t = a[i];
					a[i] = a[j];
					a[j] = t;
				}

				Ksize count = 1;
				for (int dir : a) {
					switch (dir) {
					case 0:
						if (now_mouse % width < width - 2 &&
							maze[now_mouse + 1] == false && //next right
							maze[now_mouse + 2] == false && //next right's right
							maze[now_mouse + width + 1] == false && //next right's up
							maze[now_mouse - width + 1] == false) //next right's down
							count += mazeGenerator(now_mouse + 1); //right can be the next step.
						else if (now_mouse % width == width - 2 && exit == 0) { //generate an exit if not exists.
							exit = now_mouse + 1;
							maze[exit] = true;
							++count;
						}
						break;
					case 1:
						if (now_mouse < width * (height - 2) &&
							maze[now_mouse + width] == false &&
							maze[now_mouse + width + 1] == false &&
							maze[now_mouse + 2 * width] == false &&
							maze[now_mouse + width - 1] == false)
							count += mazeGenerator(now_mouse + width);
						break;
					case 2:
						if (now_mouse % width > 1 &&
							maze[now_mouse - 1] == false &&
							maze[now_mouse + width - 1] == false &&
							maze[now_mouse - 2] == false &&
							maze[now_mouse - width - 1] == false)
							count += mazeGenerator(now_mouse - 1);
						break;
					case 3:
						if (now_mouse > 2 * width &&
							maze[now_mouse - width] == false &&
							maze[now_mouse - width + 1] == false &&
							maze[now_mouse - width - 1] == false &&
							maze[now_mouse - 2 * width] == false)
							count += mazeGenerator(now_mouse - width);
						break;
					}
				}
				return count;
			}

			void initArray() {
				vao = new KBuffer::VertexArray();

				vbo = new KBuffer::VertexBuffer(vertices, sizeof(tvec3) * 24);
				vao->allocate(vbo, A_POSITION, 3, GL_FLOAT);

				tbo = new KBuffer::VertexBuffer(tex_coords, sizeof(tvec2) * 24);
				vao->allocate(tbo, A_TEXCOORD, 2, GL_FLOAT);

				nbo = new KBuffer::VertexBuffer(normals, sizeof(tvec3) * 24);
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

		public:
			Maze(Ksize w = 20, Ksize h = 20):
				Object3D("Maze", true), vertices(nullptr), tex_coords(nullptr),
				normals(nullptr), indices(nullptr), matrices(nullptr), material(nullptr),
				mbo(nullptr), maze(nullptr) {
				generateBox(1.0, 1.0, 1.0);
				initArray();
				resetMaze(w, h);
				material = new KMaterial::Material();
				material->addTexture(RES_PATH + "stone.png", KMaterial::DIFFUSE);
				material->addTexture(RES_PATH + "stone.png");
			}

			~Maze()override {
				delete[] vertices;
				delete[] tex_coords;
				delete[] normals;
				delete[] maze;
				delete indices;
				delete matrices;
				delete material;
				delete mbo;
			}

			void resetMaze(Ksize w = 20, Ksize h = 20) {
				//note: remember to transpose for our matrix if row-first
				//but in mat4 must be column-first.
				width = w + 2; height = h + 2;
				count = width * height;
				count += generateMaze();
				delete matrices;
				matrices = new std::vector<tmat4>();
				matrices->reserve(count);
				Kfloat px = -Kfloat(width - 1) / 2.0;
				Kfloat pz = Kfloat(height - 1) / 2.0;
				for (int i = 0; i < height; ++i) {
					for (int j = 0; j < width; ++j) {
						matrices->emplace_back(KFunction::translate(KVector::Vec3(px + j, 0, pz - i)).transpose());
						if (!maze[i * width + j]) matrices->emplace_back(KFunction::translate(
										  KVector::Vec3(px + j, 1, pz - i)).transpose());
					}
				}
				bindMatrices();
			}

			tvec3 getStartPosition()const {
				return tvec3(-Kfloat(width - 1) / 2.0, 1, Kfloat(height - 1) / 2.0 - entry / width);
			}

			void bind()const override {
				Object3D::bind();
				if(material != nullptr) material->bind();
			}

			void unBind()const override {
				Object3D::unBind();
				if (material != nullptr) material->activeTextures(false);
			}

			void render(const KRenderer::Shader* shader= nullptr,
				const KRenderer::Shader* contourShader = nullptr)const override {
				bind();

				if (contourShader != nullptr) {
					glStencilFunc(GL_ALWAYS, 1, 0xFF);
					glStencilMask(0xFF);
				}
				glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr, count);

				if (contourShader != nullptr) {
					contourShader->bind();
					glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
					glStencilMask(0x00);
					glLineWidth(3.0);
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, nullptr, count);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glStencilMask(0xFF);
					shader->bind();
				}

				unBind();
			}

			Ksize getCount()const override {
				return count;
			}

#ifdef IMGUI_ENABLE
			void drawGui() {
				ImGui::Checkbox("change", &test);
			}
#endif // IMGUI_ENABLE
		};
	}
}

#endif //KENGINE_MAZE_H
