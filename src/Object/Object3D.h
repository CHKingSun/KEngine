//
// Created by KingSun on 2018/05/11
//

#ifndef KENGINE_OBJECT3D_H
#define KENGINE_OBJECT3D_H

//大部分情况我们以物体本身作为中心进行缩放平移旋转
//Nmatrix = mat3(modelMatrix)

#include <string>
#include <memory>
#include "../KHeader.h"
#include "../Core/Vector/Vec3.h"
#include "../Core/Matrix/Quaternion.h"
#include "../Render/Shader.h"
#include "../Render/Buffer/VertexArray.h"
#include "../Render/Buffer/VertexBuffer.h"
#include "../Render/Buffer/UniformBlock.h"

//Maybe later we beed a AABB box or Sphere box and then we need it center
//Maybe need parent Object3D

namespace KEngine {
	namespace KObject {
		class Object3D {
		protected :
			KVector::Vec3 position;
			KMatrix::Quaternion rotation; //Maybe it's better to use matrix now.
			KVector::Vec3 m_scale;

			KBuffer::VertexArray* vao;
			KBuffer::VertexBuffer* ibo;

			std::string type;

			static std::shared_ptr<KBuffer::UnifromBlock> block;

			const static std::string MODEL; //model block name
			const static std::string U_POSITION; //u_mPos
			const static std::string U_ROTATION; //u_mRotate
			const static std::string U_SCALE; //u_mScale

			const static Kint A_POSITION; // = 1; a_position
			const static Kint A_NORMAL; // = 2; a_normal
			const static Kint A_TEXCOORD; // = 3; a_texcoord
			const static Kint A_MATRIX; // = 4; a_matrix

		protected:
			Object3D(std::string type) : type(type), vao(nullptr), ibo(nullptr),
				position(KVector::Vec3(0)), rotation(KMatrix::Quaternion()),
				m_scale(KVector::Vec3(1.0f)){};

		public:
			virtual ~Object3D() {
				std::cout << type << std::endl;
				delete vao;
				delete ibo;
			}

			const std::string& getType()const {
				return type;
			}

			static void bindUniform(const KRenderer::Shader* shader) {
				block = std::make_shared<KBuffer::UnifromBlock>(shader, MODEL.c_str());
				block->prepare(std::vector<const char*>{
					U_POSITION.c_str(), U_ROTATION.c_str(), U_SCALE.c_str()
				});
			}

			virtual void bind()const {
				if (vao != nullptr) {
					vao->bind();
					vao->enableVertexArray();
				}
				if(ibo != nullptr) ibo->bind();
				if (block != nullptr) {
					block->allocate(std::vector<KBuffer::BlockData>{
						{ U_POSITION.c_str(), position.data() },
						{ U_ROTATION.c_str(), rotation.toMat4().data() },
						{ U_SCALE.c_str(), m_scale.data() }
					});
				}
			}

			void unBind()const {
				if (vao != nullptr) {
					vao->unBind();
					vao->disableVertexArray();
				}
				if (ibo != nullptr) ibo->unBind();
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

			inline void setRenderMode(GLenum face = GL_FRONT_AND_BACK,
				GLenum mode = GL_FILL)const {
				glPolygonMode(face, mode);
			}

			virtual Ksize getCount()const = 0;
			virtual GLenum getComponent()const = 0;
			virtual void render(const KRenderer::Shader* shader = nullptr)const = 0;
		};

		const std::string Object3D::MODEL("model");
		const std::string Object3D::U_POSITION("u_mPos");
		const std::string Object3D::U_ROTATION("u_mRotate");
		const std::string Object3D::U_SCALE("u_mScale");

		const Kint Object3D::A_POSITION = 1;
		const Kint Object3D::A_NORMAL = 2;
		const Kint Object3D::A_TEXCOORD = 3;
		const Kint Object3D::A_MATRIX = 4;

		std::shared_ptr<KBuffer::UnifromBlock> Object3D::block(nullptr);
	}
}

#endif //KENGINE_OBJECT3D_H
