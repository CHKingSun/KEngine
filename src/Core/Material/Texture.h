//
// Created by KingSun on 2018/05/11
//

#ifndef KENGINE_TEXTURE_H
#define KENGINE_TEXTURE_H

#include <string>
#include <unordered_map>
#include <GL/glew.h>
#include <stb_image.h>
#include <iostream>
#include "../../KHeader.h"
#include "../../Render/Shader.h"

namespace KEngine {
	namespace KMaterial {
		enum TextureType {
			AMBIENT = 1, DIFFUSE, SPECULAR,
			COLOR, DEPTH, STENCIL, DEPTH_AND_STENCIL
		};

		class Texture {
			friend class KBuffer::FrameBuffer;

		private:
			Kuint id;
			mutable Kuint activeId;
			std::string path;
			TextureType type;

			static Kint max_num; // = 0;
			static std::unordered_map<std::string, Kuint> texPaths;
			const static std::string TEX_HEAD; // = "u_textures[";
			const static std::string TEX_TEXTURE; // = "].tex";
			const static std::string TEX_TYPE; // = "].type";
			const static std::string TEX_ENABLE; // = "].enable";

		public:

			Texture(const std::string &path, TextureType type = AMBIENT):
				path(path), type(type) {
				if (max_num == 0) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_num);
				id = getTextureId(path);
			}
			Texture(TextureType type = COLOR, Ksize width = 1024, Ksize height = 1024): type(type) {
				if (max_num == 0) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_num);
				id = getTextureId(type, width, height);
			}
			~Texture() {
				//if (glIsTexture(id)) glDeleteTextures(1, &id);
				//remember that you share your textures with other objects,
				//so some day when you get wrong with texture, maybe here.
				//Oh yeah I got no texture hours later.
				//texPaths.erase(path);
			}

			static void deleteAllTextures() {
				for (auto &it : texPaths) {
					glDeleteTextures(1, &(it.second));
				}
				texPaths.clear();
			}

			Kuint getTextureId(const std::string &path)const {
				auto fit = texPaths.find(path);
				if (fit != texPaths.end()) return fit->second;
				GLuint tex_id;
				glGenTextures(1, &tex_id);
				//OpenGL 4.5 glCreateTextures(target, size, *textures);

				int width, height, component;
				GLubyte *data = stbi_load(path.data(), &width, &height, &component, 0);
				if (data != nullptr) {
					GLenum format;
					switch (component) {
					case 1:
						format = GL_RED;
						break;
					case 3:
						format = GL_RGB;
						break;
					case 4:
						format = GL_RGBA;
						break;
					default:
						format = GL_RGB;
					}

					glBindTexture(GL_TEXTURE_2D, tex_id);
					glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 6);
					//OpenGL 4.5 glGenerateTextureMipmap(texture);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

					stbi_image_free(data);
				}
				else {
					std::cerr << "Load texture file: " << path << "failed!" << std::endl;
					glDeleteTextures(1, &tex_id);
				}
				texPaths.emplace(path, tex_id);
				return tex_id;
			}

			Kuint getTextureId(TextureType type = COLOR, Ksize width = 1024, Ksize height = 1024)const {
				Kuint tex_id;
				glGenTextures(1, &tex_id);
				glBindTexture(GL_TEXTURE_2D, tex_id);

				GLenum format = GL_RGBA;
				switch (type)
				{
				case KEngine::KMaterial::DEPTH:
					format = GL_DEPTH_COMPONENT;
					break;
				case KEngine::KMaterial::STENCIL:
					format = GL_STENCIL_INDEX;
					break;
				case KEngine::KMaterial::DEPTH_AND_STENCIL:
					format = GL_DEPTH_STENCIL;
					break;
				default:
					break;
				}
				glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_FLOAT, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				return tex_id;
			}

			void bind(const KRenderer::Shader *shader, Kuint activeId = 0XFFFFFFFF)const {
				if (activeId < max_num) this->activeId = activeId;

				glActiveTexture(GL_TEXTURE0 + activeId);
				glBindTexture(GL_TEXTURE_2D, id);
				const std::string index = std::to_string(activeId);
				shader->bindUniform1i(TEX_HEAD + index + TEX_TEXTURE, GL_TEXTURE0 + activeId);
				shader->bindUniform1i(TEX_HEAD + index + TEX_TYPE, type);
				shader->bindUniform1i(TEX_HEAD + index + TEX_ENABLE, 1);
			}

			void active(const KRenderer::Shader *shader) {
				shader->bindUniform1i(std::string(TEX_HEAD + std::to_string(activeId) + TEX_ENABLE), 1);
			}

			void unActive(const KRenderer::Shader *shader)const {
				shader->bindUniform1i(std::string(TEX_HEAD + std::to_string(activeId) + TEX_ENABLE), 0);
			}
		};

		Kint Texture::max_num = 0;
		std::unordered_map<std::string, Kuint> Texture::texPaths = std::unordered_map<std::string, Kuint>();
		const std::string Texture::TEX_HEAD("u_textures[");
		const std::string Texture::TEX_TEXTURE("].tex");
		const std::string Texture::TEX_TYPE("].type");
		const std::string Texture::TEX_ENABLE("].enable");
	}
}

#endif //KENGINE_TEXTURE_H
