//
// Created by KingSun on 2018/5/3.
//

#ifndef KENGINE_CAMERACONTROLLER_H
#define KENGINE_CAMERACONTROLLER_H

#include <vector>
#include "../KHeader.h"
#include "../Core/Camera/Camera.h"

namespace KEngine{
    namespace KController{
        using namespace KCamera;

        class CameraController{
        private:
            std::vector<Camera*> *cameras;
            Camera *defaultCamera;
            Kuint active; //which camera to use

        public:
            CameraController(){
                cameras = nullptr;
                defaultCamera = new Camera();
                active = 0;
            }
            ~CameraController(){
                deleteAllCamera();
                delete cameras;
                delete defaultCamera;
            }

            Kboolean setDefaultCamera(Camera *camera){
                if(camera == nullptr) return false;
                delete defaultCamera;
                defaultCamera = camera;
            }
            Kboolean setActiveCamera(Kuint index){
                if(cameras == nullptr || index >= cameras->size()) return false;
                active = index + 1; //zero is the default camera
            }
            Kboolean deleteCamera(Kuint index){
                if(cameras == nullptr || index >= cameras->size()) return false;
                auto it = cameras->begin() + index;
                delete(*it);
                *it = nullptr;
                cameras->erase(it);
                if(index+1 == active) active = 0;
                if(index+1 > active) --active;
                return true;
            }
            void deleteAllCamera(){
                if(cameras == nullptr) return;
                for(auto &camera : *cameras){
                    delete camera;
                    camera = nullptr;
                }
                cameras->clear();
                active = 0;
            }

            Camera* getActiveCamera(){
                if(active == 0) return defaultCamera;
                return cameras->at(active-1);
            }

            Kboolean addCamera(Camera *camera){
                if(camera == nullptr) return false;
				if (cameras == nullptr) {
					cameras = new std::vector<Camera*>();
					cameras->reserve(6);
				}
                cameras->emplace_back(camera);
            }
        };
    }
}

#endif //KENGINE_CAMERACONTROLLER_H
