//
// Created by KingSun on 2018/05/11
//

#ifndef KENGINE_TEXTURE_H
#define KENGINE_TEXTURE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <GL/glew.h>
#include <stb_image.h>
#include <iostream>
#include "../../Render/Buffer/UniformBlock.h"
#include "../../KHeader.h"
#include "../../Render/Shader.h"

namespace KEngine {
	namespace KMaterial {
		enum TextureType {
			AMBIENT = 1, DIFFUSE, SPECULAR,
			COLOR, DEPTH, STENCIL, DEPTH_AND_STENCIL
		};

		struct TextureCount {
			TextureCount(Kuint id): tex_id(id), count(1) {}

			Kuint tex_id;
			Ksize count;
		};

		class Texture {
			friend class KBuffer::FrameBuffer;

		private:
			Kuint id;
			mutable Kuint activeId;
			std::string path;
			TextureType type;

			static Kint max_num; // = 0;
			static std::unordered_map<std::string, TextureCount> texPaths;
			const static KRenderer::Shader* shader;

			const static std::string TEXTURES; // = "textures";
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
			Texture(TextureType type = COLOR, Ksize width = 1024, Ksize height = 1024):
				type(type), path() {
				if (max_num == 0) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_num);
				id = getTextureId(type, width, height);
			}
			~Texture() {
				//if (glIsTexture(id)) glDeleteTextures(1, &id);
				//remember that you share your textures with other objects,
				//so some day when you get wrong with texture, maybe here.
				//Oh yeah I got no texture hours later.
				//texPaths.erase(path);
				if (!path.empty()) {
					auto fit = texPaths.find(path);
					if (fit != texPaths.end()) {
						fit->second.count -= 1;
						if (fit->second.count == 0) {
							if (glIsTexture(id)) glDeleteTextures(1, &id);
							texPaths.erase(fit);
						}
					}
					return;
				}
				if (glIsTexture(id)) glDeleteTextures(1, &id);
			}

			static Kuint getTextureId(const std::string &path) {
				auto fit = texPaths.find(path);
				if (fit != texPaths.end()) {
					fit->second.count += 1;
					return fit->second.tex_id;
				}
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
					std::cerr << "Load texture file: " << path << " failed!" << std::endl;
					glDeleteTextures(1, &tex_id);
				}
				texPaths.emplace(path, tex_id);
				return tex_id;
			}

			static Kuint getTextureId(TextureType type = COLOR, Ksize width = 1024, Ksize height = 1024) {
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

			static Kuint getCubeTextureId(TextureType type = COLOR, Ksize width = 1024, Ksize height = 1024) {
				Kuint tex_id;
				glGenTextures(1, &tex_id);
				glBindTexture(GL_TEXTURE_CUBE_MAP, tex_id);

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
				for (Kuint i = 0; i < 6; ++i) {
					//PX, NX, PY, NY, PZ, NZ
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_FLOAT, nullptr);
				}

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				return tex_id;
			}

			static void bindUniform(const KRenderer::Shader* shader) {
				Texture::shader = shader;
			}

			void bind(Kuint activeId = 0XFFFFFFFF)const {
				if (activeId < max_num) this->activeId = activeId;

				if (shader == nullptr) return;
				glActiveTexture(GL_TEXTURE0 + this->activeId);
				glBindTexture(GL_TEXTURE_2D, id);
				const std::string index = std::to_string(this->activeId);
				shader->bindUniform1i(TEX_HEAD + index + TEX_TEXTURE, this->activeId);
				shader->bindUniform1i(TEX_HEAD + index + TEX_TYPE, type);
				shader->bindUniform1i(TEX_HEAD + index + TEX_ENABLE, 1);
			}

			void active(Kboolean enable = true) {
				shader->bindUniform1i(TEX_HEAD + std::to_string(activeId) + TEX_ENABLE, enable);
			}
		};

		Kint Texture::max_num = 0;
		std::unordered_map<std::string, TextureCount> Texture::texPaths = std::unordered_map<std::string, TextureCount>();
		const KRenderer::Shader* Texture::shader(nullptr);

		const std::string Texture::TEXTURES("textures");
		const std::string Texture::TEX_HEAD("u_textures[");
		const std::string Texture::TEX_TEXTURE("].tex");
		const std::string Texture::TEX_TYPE("].type");
		const std::string Texture::TEX_ENABLE("].enable");
	}
}

#endif //KENGINE_TEXTURE_H
