//
// Create by KingSun on 2018/05/10
//

#ifndef KENGINE_VERTEX_ARRAY_H
#define KENGINE_VERTEX_ARRAY_H

#include <unordered_set>
#include <GL/glew.h>
#include "../../KHeader.h"
#include "./VertexBuffer.h"

namespace KEngine {
	namespace KBuffer {
		class VertexArray {
		private:
			Kuint id;
			std::unordered_set<Kint> *locations;

		public:
			VertexArray() {
				glGenVertexArrays(1, &id);
				glBindVertexArray(id);
				locations = nullptr;
			}
			~VertexArray() {
				if (glIsVertexArray(id)) glDeleteVertexArrays(1, &id);
				delete locations;
			}

			void bind()const {
				glBindVertexArray(id);
			}

			void unBind()const {
				glBindVertexArray(0);
			}

			void allocate(const VertexBuffer& vb, Kint location, Kuint size, GLenum type,
				bool normalized = false, Kuint stride = 0, Kuint offset = 0) {
				bind();
				vb.bind();
				if (locations == nullptr) locations = new std::unordered_set<Kint>();
				locations->insert(location);
				glVertexAttribPointer(location, size, type, normalized, stride, (void*)offset);
			}

			void clearLocation() {
				delete locations;
				locations = nullptr;
			}

			void enableVertexArray() {
				for (auto loc : *locations) {
					glEnableVertexAttribArray(loc);
				}
			}

			void disableVertexArray() {
				for (auto loc : *locations) {
					glDisableVertexAttribArray(loc);
				}
			}
		};
	}
}

#endif //KENGINE_VERTEX_ARRAY_H