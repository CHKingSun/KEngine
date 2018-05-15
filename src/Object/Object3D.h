//
// Created by KingSun on 2018/05/11
//

#ifndef KENGINE_OBJECT3D_H
#define KENGINE_OBJECT3D_H

//大部分情况我们以物体本身作为中心进行缩放平移旋转
//Nmatrix = mat3(modelMatrix)

#include <string>
#include "../KHeader.h"
#include "../Core/Vector/Vec3.h"
#include "../Core/Matrix/Quaternion.h"
#include "../Render/Shader.h"
#include "../Render/Buffer/VertexArray.h"
#include "../Render/Buffer/VertexBuffer.h"
#include "../Render/Buffer/UniformBlock.h"

//Maybe later we beed a AABB box or Sphere box and then we need it center

namespace KEngine {
	namespace KObject {
		class Object3D {
		protected :
			KVector::Vec3 position;
			KMatrix::Quaternion rotation;
			KVector::Vec3 m_scale;

			KBuffer::VertexArray* vao;
			KBuffer::VertexBuffer* ibo;
			mutable KBuffer::UnifromBlock* block;

			std::string type;

			const static std::string MODEL;
			const static std::string U_POSITION;
			const static std::string U_ROTATION;
			const static std::string U_SCALE;

			const static Kint A_POSITION; // = 1;
			const static Kint A_NORMAL; // = 2;
			const static Kint A_TEXCOORD; // = 3;

		protected:
			Object3D(std::string type) : type(type), vao(nullptr), ibo(nullptr),
				position(KVector::Vec3(0)), rotation(KMatrix::Quaternion()),
				m_scale(KVector::Vec3(1.0f)), block(nullptr){};

		public:
			virtual ~Object3D() {
				std::cout << type << std::endl;
				delete vao;
				delete ibo;
			}

			const std::string& getType()const {
				return type;
			}

			void bind()const {
				if (vao != nullptr) {
					vao->bind();
					vao->enableVertexArray();
				}
				if(ibo != nullptr) ibo->bind();
			}

			void unBind()const {
				if (vao != nullptr) {
					vao->unBind();
					vao->disableVertexArray();
				}
				if(ibo != nullptr) ibo->unBind();
			}

			void setPosition(const KVector::Vec3& v) {
				position = v;
			}

			void setRotation(Kfloat angle, const KVector::Vec3& v) {
				rotation = KMatrix::Quaternion(angle, v);
			}

			void setScale(const KVector::Vec3& v) {
				m_scale = v;
			}

			void translate(const KVector::Vec3& v) {
				position += v;
			}

			void rotate(Kfloat angle, const KVector::Vec3& v) {
				rotation *= KMatrix::Quaternion(angle, v);
			}

			void scale(const KVector::Vec3 &v) {
				m_scale *= v;
			}

			virtual void bindUniform(const KRenderer::Shader* shader)const {
				if (block == nullptr) {
					using namespace KBuffer;
					block = new UnifromBlock(shader, MODEL.c_str());
					std::vector<BlockData> data;
					data.reserve(3);
					data.emplace_back(U_POSITION.c_str(), position.data());
					data.emplace_back(U_ROTATION.c_str(), rotation.toMat4().data());
					data.emplace_back(U_SCALE.c_str(), m_scale.data());
					block->allocate(data);
				}
			}

			void reBindPosition() {
				if (block == nullptr) return;
				block->reAllocate(KBuffer::BlockData(U_POSITION.c_str(), position.data()));
			}

			void reBindRotation() {
				if (block == nullptr) return;
				block->reAllocate(KBuffer::BlockData(U_ROTATION.c_str(), rotation.toMat4().data()));
			}			
			
			void reBindScale() {
				if (block == nullptr) return;
				block->reAllocate(KBuffer::BlockData(U_SCALE.c_str(), m_scale.data()));
			}

			virtual Ksize getCount()const = 0;
			virtual GLenum getComponent()const = 0;
			virtual void render()const = 0;
		};

		const std::string Object3D::MODEL("model");
		const std::string Object3D::U_POSITION("u_mPos");
		const std::string Object3D::U_ROTATION("u_mRotate");
		const std::string Object3D::U_SCALE("u_mScale");

		const Kint Object3D::A_POSITION = 1;
		const Kint Object3D::A_NORMAL = 2;
		const Kint Object3D::A_TEXCOORD = 3;
	}
}

#endif //KENGINE_OBJECT3D_H
