//
// Created by KingSun on 2018/5/3.
//

#ifndef KENGINE_SHADER_H
#define KENGINE_SHADER_H

#include <GL/glew.h>
#include <string>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include "../KHeader.h"

namespace KEngine{
    namespace KRenderer{
        class Shader{
        private:
            Kuint program;
            std::unordered_map<std::string, int> *uniforms;

            bool readFile(const std::string &filename, std::string content)const {
                std::ifstream file(filename, std::ios::in);
                if(!file.is_open())
                {
                    std::cerr<<"File "<<filename<<" read failed!"<<std::endl;
                    return false;
                }

                content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
            }

            Kint getLocation(const std::string &name)const {
                auto it = uniforms->find(name);
                if(it == uniforms->end()){
					Kint loc = glGetUniformLocation(program, name.c_str());
					uniforms->emplace(std::make_pair(name, loc));
					return loc;
                }
				return it->second;
            }

        public:
            Shader(): program(0), uniforms(new std::unordered_map<std::string, int>()){}
            Shader(const std::string &vs_filename, const std::string &fs_filename):
            uniforms(new std::unordered_map<std::string, int>()){
                createProgram(vs_filename, fs_filename);
            }
            ~Shader(){
                delete uniforms;
                if(glIsProgram(program)) glDeleteProgram(program);
            }

            bool createProgram(const std::string &vs_filename, const std::string &fs_filename){
                program = glCreateProgram();
                if(program == 0){
                    std::cerr << "Create program failed!" << std::endl;
                    return false;
                }

                Kuint vertShader = createShader(GL_VERTEX_SHADER, vs_filename);
                Kuint fragShader = createShader(GL_FRAGMENT_SHADER, fs_filename);
                glAttachShader(program, vertShader);
                glAttachShader(program, fragShader);
                glLinkProgram(program);
                glDeleteShader(vertShader);
                glDeleteShader(fragShader);

                GLint flag;
                auto *error_log = new GLchar[1024];
                glGetProgramiv(program, GL_LINK_STATUS, &flag);
                if(GL_FALSE == flag){
                    glGetProgramInfoLog(program, 1024, nullptr, error_log);
                    std::cerr << "Error linking program: " << error_log << std::endl;
                    glDeleteProgram(program);
                    return false;
                }

                glValidateProgram(program);
                glGetProgramiv(program, GL_VALIDATE_STATUS, &flag);
                if(GL_FALSE == flag){
                    glGetProgramInfoLog(program, 1024, nullptr, error_log);
                    std::cerr << "Program valid: " << error_log << std::endl;
                    glDeleteProgram(program);
                    return false;
                }
                delete error_log;
                return true;
            }

            Kuint createShader(GLenum type, const std::string &filename)const {
                Kuint shader = glCreateShader(type);
                if(shader == 0){
                    std::cerr << "Create shader failed with type: " << type << std::endl;
                    return shader;
                }

                std::string content;
                if(!readFile(filename, content)){
                    std::cerr << "Read file: " << filename << " failed!" << std::endl;
                    glDeleteShader(shader);
                    return 0;
                }

                const char *sources = content.c_str();
                glShaderSource(shader, 1, &sources, nullptr);
                glCompileShader(shader);
                delete sources;
                sources = nullptr;

                GLint flag;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &flag);
                if(flag == GL_FALSE){
                    auto *error_log = new GLchar[1024];
                    glGetShaderInfoLog(shader, 1024, nullptr, error_log);
                    std::cerr << "Error compiling shader: " << error_log << std::endl;
                    glDeleteShader(shader);
                    delete error_log;
                    return 0;
                }

                return shader;
            }

            bool addShader(Kuint shader)const {
                if(!glIsShader(shader) || !glIsProgram(program)) return false;
                glAttachShader(program, shader);
                glLinkProgram(program);

                GLint flag;
                glGetProgramiv(program, GL_LINK_STATUS, &flag);
                if(GL_FALSE == flag){
                    auto *error_log = new GLchar[1024];
                    glGetProgramInfoLog(program, 1024, nullptr, error_log);
                    std::cerr<<"Error linking program: "<<error_log<<std::endl;
                    delete error_log;
                    return false;
                }
                return true;
            }

            bool isValid()const {
                return glIsProgram(program);
            }

            void bind()const {
                if(glIsProgram(program)) glUseProgram(program);
            }

            void unBind()const {
                glUseProgram(0);
            }

            void bindUniform1f(const std::string &name, Kfloat value)const {
                if(!isValid()) return;
                glUniform1f(getLocation(name), value);
            }

            void bindUniform1i(const std::string &name, Kint value)const {
                if(!isValid()) return;
                glUniform1i(getLocation(name), value);
            }
        };
    }
}

#endif //KENGINE_SHADER_H
