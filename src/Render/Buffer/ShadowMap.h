//
// Created by KingSun on 2018/06/01
//

#ifndef KENGINE_SHADOW_MAP_H
#define KENGINE_SHADOW_MAP_H

#include "./FrameBuffer.h"
#include "../../Core/Material/Texture.h"
#include "../../KHeader.h"

namespace KEngine {
	namespace KBuffer {
		class ShadowMap {
		private:
			FrameBuffer* fbo;
			Kuint tex_id;
		
		public:
			ShadowMap(Ksize width = 1024, Ksize height = 1024) {
				//It is only for cube map now.
				fbo = new FrameBuffer();

				tex_id = KMaterial::Texture::getCubeTextureId(KMaterial::DEPTH, width, height);

				fbo->bindTexture(KMaterial::DEPTH, tex_id);
			}

			void bind()const {
				fbo->bind();
			}

			void unBind()const {
				fbo->unBind();
			}

			void bindTexture(const KRenderer::Shader* shader) {
				glActiveTexture(GL_TEXTURE10);
				glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);
				shader->bindUniform1i("u_shadow_texture", GL_TEXTURE10);
			}
		};
	}
}

#endif // !KENGINE_SHADOW_MAP_H

