//
// Created by KingSun on 2018/5/4.
//

#ifndef KENGINE_WINDOW_H
#define KENGINE_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "./KHeader.h"
#include "Render/Renderer.h"

namespace KEngine{
    namespace KWindow{
        using namespace KRenderer;

        class Window{
        private:
            Kdouble start_time, run_time;
            Kuint width, height;
            Kboolean key[512]; //-1, 32-162, 256-248
            Renderer *renderer;
            GLFWwindow *window;

            Kboolean initial(const std::string &name){
                if(!glfwInit()){
                    window = nullptr;
                    return false;
                }

                window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);
                if(!window) return false;
                glfwSetWindowPos(window, 60, 60);
                glfwMakeContextCurrent(window);
                return true;
            }

        public:
            Window(const std::string &name){
                if(!initial(name)){
                    std::cerr << "Create window failed!" << std::endl;
                    renderer = nullptr;
                    return;
                }

                renderer = new Renderer();
            }
            Window(const std::string &name, const std::string &vs_filename, const std::string &fs_filename){
                if(!initial(name)){
                    std::cerr << "Create window failed!" << std::endl;
                    renderer = nullptr;
                    return;
                }

                renderer = new Renderer(vs_filename, fs_filename);
            }
            ~Window(){
                delete renderer;
                glfwDestroyWindow(window);
                glfwTerminate();
            }

            void startup(){
                start_time = glfwGetTime();
                if(!window){
                    while(!glfwWindowShouldClose(window)){
                        renderer->render();
                        glfwSwapBuffers(window);
                        run_time = glfwGetTime() - start_time;
                        glfwWaitEvents(); //glfwPollEvents() for animation
                    }
                }
            }
        };
    }
}

#endif //KENGINE_WINDOW_H
