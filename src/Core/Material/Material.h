 //
// Created by KingSun on 2018/5/3.
//

#ifndef KENGINE_MATERIAL_H
#define KENGINE_MATERIAL_H

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include "../../KHeader.h"
#include "./Texture.h"
#include "../../Render/Shader.h"
#include "../../Render/Buffer/UniformBlock.h"
#include "./Color.h"

namespace KEngine{
    namespace KMaterial{

        class Material{
		public:
            Color ambient;
            Color diffuse;
            Color specular;
            Kfloat shininess;

		private:
            std::vector<Texture*> *textures;

			static std::shared_ptr<KBuffer::UnifromBlock> block;

			const static std::string MATERIAL; //material block name
			const static std::string AMBIENT; //u_ambient
			const static std::string DIFFUSE; //u_diffuse
			const static std::string SPECULAR; //u_specular
			const static std::string SHININESS; //u_shininess

        public:

            Material(const Color &ka = WHITE, const Color &kd = GREY,
                     const Color &ks = GREY, Kfloat shininess = 1.0f): shininess(shininess),
                    ambient(ka), diffuse(kd), specular(ks), textures(nullptr){}
            Material(std::vector<Texture*> *tex, const Color &ka = WHITE,
                     const Color &kd = GREY, const Color &ks = GREY,
                     Kfloat shininess = 1.0f): ambient(ka), diffuse(kd), specular(ks),
                     shininess(shininess), textures(tex){}
            ~Material(){
                deleteAllTextures();
                delete textures;
            }

            void setTextures(std::vector<Texture*> *tex){
                if(tex == nullptr) return;
                deleteAllTextures();
                delete textures;
                textures = tex;
            }
            void addTexture(const std::string &path, TextureType type = TextureType::AMBIENT) {
                if(textures == nullptr) textures = new std::vector<Texture*>();
                textures->emplace_back(new Texture(path, type));
            }

			static void bindUniform(const KRenderer::Shader *shader) {
				if (block == nullptr) {
					block = std::make_shared<KBuffer::UnifromBlock>(shader, MATERIAL.c_str());
					block->prepare(std::vector<const char*>{
						AMBIENT.c_str(), DIFFUSE.c_str(), SPECULAR.c_str(), SHININESS.c_str()
					});
				}
				else block->bindShader(shader);
				Texture::bindUniform(shader);
			}

            void bind()const {
				if (block != nullptr) {
					block->allocate(std::vector<KBuffer::BlockData>{
						{ AMBIENT.c_str(), ambient.data() },
						{ DIFFUSE.c_str(), diffuse.data() },
						{ SPECULAR.c_str(), specular.data() },
						{ SHININESS.c_str(), &shininess }
					});
				}
				if (textures != nullptr) {
					int i = 0;
					for (auto &it : *textures) {
						it->bind(i++);
						//note: use ++i maybe you will got some different answer.
						//Maybe it's related to glActiveTexture();
					}
				}
            }

			void activeTextures(Kboolean enable = true)const {
				if (textures != nullptr) {
					for (auto &it : *textures) {
						it->active(enable);
					}
				}
			}

            Kboolean deleteTexture(Kuint index){
                if(textures == nullptr || index >= textures->size()) return false;
                auto it = textures->begin() + index;
                delete(*it);
                *it = nullptr;
                textures->erase(it);
            }
            void deleteAllTextures(){
				if (textures == nullptr) return;
                for(auto &tex : *textures){
                    delete tex;
                    tex = nullptr;
                }
                textures->clear();
            }
        };

		const std::string Material::MATERIAL("material");
		const std::string Material::AMBIENT("u_ambient");
		const std::string Material::DIFFUSE("u_diffuse");
		const std::string Material::SPECULAR("u_specular");
		const std::string Material::SHININESS("u_shininess");

		std::shared_ptr<KBuffer::UnifromBlock> Material::block(nullptr);
    }
}

#endif //KENGINE_MATERIAL_H
