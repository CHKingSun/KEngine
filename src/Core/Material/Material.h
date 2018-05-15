 //
// Created by KingSun on 2018/5/3.
//

#ifndef KENGINE_MATERIAL_H
#define KENGINE_MATERIAL_H

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <vector>
#include <GL/glew.h>
#include "../../KHeader.h"
#include "./Texture.h"
#include "../../Render/Shader.h"
#include "../../Render/Buffer/UniformBlock.h"
#include "./Color.h"

namespace KEngine{
    namespace KMaterial{

        class Material{
        private:
            Color ambient;
            Color diffuse;
            Color specular;
            Kfloat shininess;
			mutable KBuffer::UnifromBlock* block;
            std::vector<Texture*> *textures;

			const static std::string MATERIAL;
			const static std::string AMBIENT;
			const static std::string DIFFUSE;
			const static std::string SPECULAR;
			const static std::string SHININESS;

        public:

            Material(const Color &ka = WHITE, const Color &kd = GREY,
                     const Color &ks = GREY, Kfloat shininess = 1.0f): shininess(shininess),
                    ambient(ka), diffuse(kd), specular(ks), textures(nullptr), block(nullptr){}
            Material(std::vector<Texture*> *tex, const Color &ka = WHITE,
                     const Color &kd = GREY, const Color &ks = GREY,
                     Kfloat shininess = 1.0f): ambient(ka), diffuse(kd), specular(ks),
                     shininess(shininess), textures(tex), block(nullptr){}
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
            void addTexture(Texture *tex) {
                if(tex == nullptr) return;
                if(textures == nullptr) textures = new std::vector<Texture*>();
                textures->emplace_back(tex);
            }

            void bind(const KRenderer::Shader *shader)const {
				if (block == nullptr) {
					using namespace KBuffer;
					block = new UnifromBlock(shader, MATERIAL.c_str());
					std::vector<BlockData> data;
					data.reserve(4);
					data.emplace_back(AMBIENT.c_str(), ambient.data());
					data.emplace_back(DIFFUSE.c_str(), diffuse.data());
					data.emplace_back(SPECULAR.c_str(), specular.data());
					data.emplace_back(SHININESS.c_str(), &shininess);
					block->allocate(data);
				}
				if (textures != nullptr) {
					int i = 0;
					for (auto &it : *textures) {
						it->bind(shader, ++i);
					}
				}
            }

			void activeTextures(const KRenderer::Shader *shader)const {
				if (textures != nullptr) {
					for (auto &it : *textures) {
						it->active(shader);
					}
				}
			}

			void unactiveTextures(const KRenderer::Shader *shader)const {
				if (textures != nullptr) {
					for (auto &it : *textures) {
						it->unActive(shader);
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
    }
}

#endif //KENGINE_MATERIAL_H
