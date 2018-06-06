//
// Created by KingSun on 2018/05/23
//

#ifndef KENGINE_FRAME_BUFFER_H
#define KENGINE_FRAME_BUFFER_H

#include <GL/glew.h>
#include "../../KHeader.h"
#include "../../Core/Material/Texture.h"

namespace KEngine {
	namespace KBuffer {
		class FrameBuffer {
		private:
			Kuint id;
			GLenum type;

		public:
			FrameBuffer(GLenum type = GL_FRAMEBUFFER): type(type) {
				glGenFramebuffers(1, &id);
				glBindFramebuffer(this->type, id);
			}
			~FrameBuffer() {
				if (glIsFramebuffer(id)) glDeleteFramebuffers(1, &id);
			}

			void bind()const {
				glBindFramebuffer(type, id);
			}

			void unBind()const {
				glBindFramebuffer(type, 0);
			}

			Kboolean check()const {
				return glCheckFramebufferStatus(type) == GL_FRAMEBUFFER_COMPLETE;
			}

			void bindTexture(KMaterial::TextureType tex_type, Kuint tex_id, GLenum attachment = GL_COLOR_ATTACHMENT0)const {
				//attachment is just for color framebuffer.
				bind();
				using namespace KMaterial;
				switch (tex_type)
				{
				case TextureType::DEPTH:
					attachment = GL_DEPTH_ATTACHMENT;
					break;
				case TextureType::STENCIL:
					attachment = GL_STENCIL_ATTACHMENT;
					break;
				case TextureType::DEPTH_AND_STENCIL:
					attachment = GL_DEPTH_STENCIL_ATTACHMENT;
					break;
				default:
					break;
				}
				glFramebufferTexture2D(type, attachment, GL_TEXTURE_2D, tex_id, 0);
				if (tex_type == DEPTH || tex_type == STENCIL || tex_type == DEPTH_AND_STENCIL) {
					glDrawBuffer(GL_NONE);
				}
				else {
					glDrawBuffer(attachment);
				}
			}

			void bindRenderBuffer(Kuint render, GLenum attachment = GL_COLOR_ATTACHMENT0)const {
				bind();
				glFramebufferRenderbuffer(type, attachment, GL_RENDERBUFFER, render);
			}
		};
	}
}


#endif //KENGINE_FRAME_BUFFER_H
