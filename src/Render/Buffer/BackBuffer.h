//
// Created by KingSun on 2018/05/13
//

#ifndef KENGINE_BACK_BUFFER_H
#define KENGINE_BACK_BUFFER_H

#include "../../KHeader.h"
#include "../Shader.h"

namespace KEngine {
	namespace KBuffer {
		//Transform feedback buffer

		class BackBuffer{
		private:
			Kuint program;
			Kuint bufferSize;
			std::vector<Kuint> buffers;

		public:
			BackBuffer(const KRenderer::Shader* shader, const std::vector<std::string>& varyings,
				Kuint bufferSize) : program(shader->program), bufferSize(bufferSize) {
				Kuint count = varyings.size();
				const auto names = new const char*[count];
				for (int i = 0; i < count; ++i) {
					names[i] = varyings[i].data();
				}
				glTransformFeedbackVaryings(program, count, names, GL_INTERLEAVED_ATTRIBS);
				glLinkProgram(program);

				Kuint tfbo;
				glGenBuffers(1, &tfbo);
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tfbo);
				glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
				buffers.emplace_back(tfbo);
			}

			BackBuffer(const KRenderer::Shader* shader, const std::vector<std::string>& varyings,
				const std::vector<Kuint>& buffers, GLenum bufferMode)
				: program(shader->program), buffers(buffers), bufferSize(0) {
				Kuint count = varyings.size();
				const auto names = new const char*[count];
				for (int i = 0; i < count; ++i) {
					names[i] = varyings[i].data();
				}
				glTransformFeedbackVaryings(program, count, names, bufferMode);
				glLinkProgram(program);
			}

			BackBuffer(const KRenderer::Shader* shader, const Kchar* const* varyings, Kuint count,
				Kuint bufferSize) : program(shader->program), bufferSize(bufferSize) {
				glTransformFeedbackVaryings(program, count, varyings, GL_INTERLEAVED_ATTRIBS);
				glLinkProgram(program);

				Kuint tfbo;
				glGenBuffers(1, &tfbo);
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, tfbo); //It also can be some other buffer you like
				glBufferData(GL_TRANSFORM_FEEDBACK_BUFFER, bufferSize, nullptr, GL_STATIC_DRAW);
				buffers.emplace_back(tfbo);
			}

			BackBuffer(const KRenderer::Shader* shader, const Kchar* const* varyings, Kuint count,
				const std::vector<Kuint>& buffers, GLenum bufferMode)
				: program(shader->program), buffers(buffers), bufferSize(0) {
				glTransformFeedbackVaryings(program, count, varyings, bufferMode);
				glLinkProgram(program);
			}

			void bind() {
				int index = 0;
				for (auto &it : buffers) {
					//What the index mean is the id of transfom feedback id,
					//but how to get it in OpenGL 3.3 ? I don't know.
					glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index++, it);
				}
			}

			void enable(GLenum type) {
				glBeginTransformFeedback(type);
			}

			void disable() {
				glEndTransformFeedback();
			}

			template <typename T>
			const T* getData() {
				static_assert(false, "BlockBuffer::getData<T> is just for some type");
				//compile error
			}

			template<>
			const Kfloat* getData<Kfloat>() {
				if (bufferSize == 0) {
					std::cerr << "Cannot manage buffer size!" << std::endl;
					return nullptr;
				}
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffers[0]);
				auto data = new float[bufferSize / sizeof(Kfloat)];
				glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferSize, data);
				return data;
			}

			template<>
			const KVector::Vec2* getData<KVector::Vec2>() {
				if (bufferSize == 0) {
					std::cerr << "Cannot manage buffer size!" << std::endl;
					return nullptr;
				}
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffers[0]);
				auto data = new KVector::Vec2[bufferSize / sizeof(KVector::Vec2)];
				glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferSize, data);
				return data;
			}

			template<>
			const KVector::Vec3* getData<KVector::Vec3>() {
				if (bufferSize == 0) {
					std::cerr << "Cannot manage buffer size!" << std::endl;
					return nullptr;
				}
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffers[0]);
				auto data = new KVector::Vec3[bufferSize / sizeof(KVector::Vec3)];
				glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferSize, data);
				return data;
			}

			template<>
			const KVector::Vec4* getData<KVector::Vec4>() {
				if (bufferSize == 0) {
					std::cerr << "Cannot manage buffer size!" << std::endl;
					return nullptr;
				}
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffers[0]);
				auto data = new KVector::Vec4[bufferSize / sizeof(KVector::Vec4)];
				glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferSize, data);
				return data;
			}

			template<>
			const KMatrix::Mat3* getData<KMatrix::Mat3>() {
				if (bufferSize == 0) {
					std::cerr << "Cannot manage buffer size!" << std::endl;
					return nullptr;
				}
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffers[0]);
				auto data = new KMatrix::Mat3[bufferSize / sizeof(KMatrix::Mat3)];
				glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferSize, data);
				return data;
			}

			template<>
			const KMatrix::Mat4* getData<KMatrix::Mat4>() {
				if (bufferSize == 0) {
					std::cerr << "Cannot manage buffer size!" << std::endl;
					return nullptr;
				}
				glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, buffers[0]);
				auto data = new KMatrix::Mat4[bufferSize / sizeof(KMatrix::Mat4)];
				glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, bufferSize, data);
				return data;
			}

			const Kuint getBufferSize()const {
				return bufferSize;
			}
		};
	}
}

#endif //KENGINE_BACK_BUFFER_H
