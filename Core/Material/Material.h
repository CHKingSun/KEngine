//
// Created by KingSun on 2018/5/3.
//

#ifndef KENGINE_MATERIAL_H
#define KENGINE_MATERIAL_H

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <vector>
#include <GL/glew.h>
#include <stb_image.h>
#include <iostream>
#include "../../KHeader.h"
#include "../../Render/Shader.h"
#include "../Vector/Color.h"

namespace KEngine{
    namespace KMaterial{
        enum TextureType{
            AMBIENT = 1, DIFFUSE, SPECULAR
        };

        class Material{
            typedef KColor::Color tcolor;

        private:
            tcolor ambient;
            tcolor diffuse;
            tcolor specular;
            Kfloat shininess;
            std::vector<Texture*> *textures;

            const static std::string AMBIENT

        public:

            Material(const tcolor &ka = KColor::WHITE, const tcolor &kd = KColor::GREY,
                     const tcolor &ks = KColor::GREY, Kfloat shininess): shininess(shininess),
                    ambient(ka), diffuse(kd), specular(ks), textures(nullptr){}
            Material(std::vector<Texture*> *tex, const tcolor &ka = KColor::WHITE,
                     const tcolor &kd = KColor::GREY, const tcolor &ks = KColor::GREY,
                     Kfloat shininess): ambient(ka), diffuse(kd), specular(ks),
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
            void addTexture(Texture *tex){
                if(tex == nullptr) return;
                if(textures == nullptr) textures = new std::vector<Texture*>();
                textures->push_back(tex);
            }

            void bind(const KRenderer::Shader *shader)const {

            }

            Kboolean deleteTexture(Kuint index){
                if(textures == nullptr || index >= textures->size()) return false;
                auto it = textures->begin() + index;
                delete(*it);
                *it = nullptr;
                textures->erase(it);
            }
            void deleteAllTextures(){
                for(auto &tex : *textures){
                    delete tex;
                    tex = nullptr;
                }
                textures->clear();
            }
        };

        class Texture{
        private:
            Kuint id;
            mutable Kint activeId;
            std::string path;
            TextureType type;

            static Kint max_num = 0;
            const static std::string TEX_HEAD = "u_textures[";
            const static std::string TEX_TEXTURE = "].texture";
            const static std::string TEX_TYPE = "].type";
            const static std::string TEX_ENABLE = "].enable";

        public:

            Texture(const std::string &path, TextureType type = DIFFUSE): path(path),
                  type(type){
                if(max_num == 0) glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &max_num);
                id = getTextureId(path);
            }
            ~Texture(){
                if(glIsTexture(id)) glDeleteTextures(1, &id);
            }

            Kuint getTextureId(const std::string &path)const {
                GLuint tex_id;
                glGenTextures(1, &tex_id);
                //OpenGL 4.5 glCreateTextures(target, size, *textures);

                int width, height, component;
                GLubyte *data = stbi_load(path.data(), &width, &height, &component, 0);
                if(data != nullptr){
                    GLenum format;
                    switch (component){
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
                    //OpenGL 4.5 glGenerateTextureMipmap(texture);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 3);

                    stbi_image_free(data);
                } else {
                    std::cerr << "Load texture file: " << path << "failed!" << std::endl;
                    glDeleteTextures(1, &tex_id);
                }
                return tex_id;
            }

            void bind(const KRenderer::Shader *shader, Kint activeId)const {
                if(activeId < 0 || activeId >= max_num) return;
                this->activeId = activeId;

                glBindTexture(GL_TEXTURE_2D, id);
                glActiveTexture(GL_TEXTURE0 + activeId);
                const std::string index = std::to_string(activeId);
                shader->bindUniform1i(std::string(TEX_HEAD + index + TEX_TEXTURE), GL_TEXTURE0 + activeId);
                shader->bindUniform1i(std::string(TEX_HEAD + index + TEX_TYPE), type);
                shader->bindUniform1i(std::string(TEX_HEAD + index + TEX_ENABLE), 1);
            }

            void unBind(const KRenderer::Shader *shader)const {
                shader->bindUniform1i(std::string(TEX_HEAD + std::to_string(activeId) + TEX_ENABLE), 0);
            }
        };
    }
}

#endif //KENGINE_MATERIAL_H
