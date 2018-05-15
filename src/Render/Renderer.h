//
// Created by KingSun on 2018/5/3.
//

#ifndef KENGINE_CONTROLLER_H
#define KENGINE_CONTROLLER_H

#include <vector>
#include "../Controller/LightController.h"
#include "../Controller/CameraController.h"
#include "../KHeader.h"
#include "./Shader.h"
#include "../Object/Object3D.h"

namespace KEngine{
	namespace KRenderer {
		using namespace KController;
		using tcolor = KMaterial::Color;

        class Renderer{
        private:
            LightController *lightController;
            CameraController *cameraController;
            Shader *shader;
			std::vector<KObject::Object3D*>* objects;

        public:
            Renderer(){
                lightController = new LightController();
                cameraController = new CameraController();
                shader = new Shader();
				objects = nullptr;
            }
            Renderer(const std::string &vs_filename, const std::string &fs_filename){
                lightController = new LightController();
                cameraController = new CameraController();
                shader = new Shader(vs_filename, fs_filename);
				shader->bind();
				//std::cout << shader->getAttribLocation("a_position") << std::endl;
				//std::cout << shader->getAttribLocation("a_normal") << std::endl;
				//std::cout << shader->getAttribLocation("a_texcoord") << std::endl;
				objects = nullptr;
            }
            ~Renderer(){
				removeAllObjects();
                delete lightController;
                delete cameraController;
                delete shader;
            }

			const Shader* getShader()const {
				return shader;
			}

			void removeAllObjects() {
				if (objects == nullptr) return;
				for (auto &it : *objects) {
					delete it;
					it = nullptr;
				}
				delete objects;
			}

			void addObject(KObject::Object3D* object) {
				if (object == nullptr) return;
				if (objects == nullptr) objects = new std::vector<KObject::Object3D*>();
				objects->emplace_back(object);
				object->bindUniform(shader);
			}

            void render(){
				shader->bind();
				if (objects == nullptr) return;
				for (auto &it : *objects) {
					it->bind();

					glDrawElements(GL_TRIANGLES, it->getCount(), it->getComponent(), nullptr);

					it->unBind();
				}
            }
        };
    }
}

#endif //KENGINE_CONTROLLER_H
