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
    namespace KRenderer{
        class Renderer{
            using namespace KController;

        private:
            LightController *lightController;
            CameraController *cameraController;
            Shader *shader;

        public:
            Renderer(){
                lightController = new LightController();
                cameraController = new CameraController();
                shader = new Shader();
            }
            Renderer(const std::string &vs_filename, const std::string &fs_filename){
                lightController = new LightController();
                cameraController = new CameraController();
                shader = new Shader(vs_filename, fs_filename);
            }
            ~Renderer(){
                delete lightController;
                delete cameraController;
                delete shader;
            }

            void render(){

            }
        };
    }
}

#endif //KENGINE_CONTROLLER_H
