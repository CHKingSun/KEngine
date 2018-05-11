//
// Created by KingSun on 2018/5/3.
//

#ifndef KENGINE_CONTROLLER_H
#define KENGINE_CONTROLLER_H

#include "../Controller/LightController.h"
#include "../Controller/CameraController.h"
#include "../KHeader.h"
#include "./Shader.h"

namespace KEngine{
	namespace KRenderer {
		using namespace KController;
		using tcolor = KColor::Color;

        class Renderer{
        private:
            LightController *lightController;
            CameraController *cameraController;
            Shader *shader;

        public:
            Renderer(){
                lightController = new LightController();
                cameraController = new CameraController();
                shader = new Shader();
				setClearColor();
            }
            Renderer(const std::string &vs_filename, const std::string &fs_filename){
                lightController = new LightController();
                cameraController = new CameraController();
                shader = new Shader(vs_filename, fs_filename);
				setClearColor();
            }
            ~Renderer(){
                delete lightController;
                delete cameraController;
                delete shader;
            }

            void render(){
				glBegin(GL_TRIANGLES);
				glVertex2f(0.0f, 0.0f);
				glVertex2f(0.0f, 1.0f);
				glVertex2f(1.0f, 1.0f);
				glEnd();
            }

			void clear() {
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}

			void setClearColor(const tcolor& color = tcolor(0.17f, 0.17f, 0.17f, 1.0f)) {
				glClearColor(color.r, color.g, color.b, color.a);
			}
        };
    }
}

#endif //KENGINE_CONTROLLER_H
