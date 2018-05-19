//
// Created by KingSun on 2018/05/12
//

#ifndef KENGINE_LOG_H
#define KENGINE_LOG_H

#include <iostream>
#include <iomanip>
#include <GL/glew.h>

#define glCall(x)	x; \
					glCheckError(#x, __FILE__, __LINE__);
void glClearError() { while (glGetError() != GL_NO_ERROR); } //抛弃当前所有错误;
bool glCheckError(const char* fun, const char* file, int line) {
	GLenum error;
	if ((error = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error at: " << fun << " in file: " << file << " at line: " << std::setbase(10) <<
			line << " with error code: 0x0" << std::setbase(16) << error << std::endl;
		return false;
	}
	return true;
}

void getMapBuffer(GLenum target, GLuint buffer) {
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
	char* mapData = (char*)glMapBuffer(GL_UNIFORM_BUFFER, GL_READ_ONLY);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
	//do not delete the memory for it not your business
}

#endif //KENGINE_LOG_H
