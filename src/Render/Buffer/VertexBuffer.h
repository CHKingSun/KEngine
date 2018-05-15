//
// Created by KingSun on 2018/05/09
//

#ifndef KENGINE_VERTEX_BUFFER_H
#define KENGINE_VERTEX_BUFFER_H

#include <unordered_set>
#include <GL/glew.h>
#include "../../KHeader.h"

namespace KEngine {
	namespace KBuffer {
		enum BufferType {
			VERTEX, INDEX
		};

		class VertexBuffer {
		private:
			Kuint id;
			GLenum type;

		public:
			VertexBuffer(const void *data, Ksize size, BufferType bufferType = VERTEX) {
				if (bufferType == VERTEX) this->type = GL_ARRAY_BUFFER;
				else this->type = GL_ELEMENT_ARRAY_BUFFER;
				glGenBuffers(1, &id);
				glBindBuffer(this->type, id);
				glBufferData(this->type, size, data, GL_STATIC_DRAW);
			}

			VertexBuffer(Ksize size, BufferType bufferType = VERTEX) {
				if (bufferType == VERTEX) this->type = GL_ARRAY_BUFFER;
				else this->type = GL_ELEMENT_ARRAY_BUFFER;
				glGenBuffers(1, &id);
				glBindBuffer(this->type, id);
				glBufferData(this->type, size, nullptr, GL_STATIC_DRAW);
			}

			~VertexBuffer() {
				if (glIsBuffer(id)) glDeleteBuffers(1, &id);
			}

			void bind()const {
				glBindBuffer(type, id);
			}

			void unBind()const {
				glBindBuffer(type, 0);
			}

			void allocate(Kuint offset, Ksize size, const void* data) {
				glBindBuffer(GL_ARRAY_BUFFER, id);
				glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
			}
		};
	}
}

#endif //KENGINE_VERTEX_BUFFER_H
