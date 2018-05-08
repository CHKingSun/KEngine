cmake_minimum_required(VERSION 3.10)
project(KEngine)

set(CMAKE_CXX_STANDARD 11)

set(OPENGL_DIR D:/GL)
include_directories(${OPENGL_DIR}/include)
link_directories(${OPENGL_DIR}/lib/x64)

set(GLFW_DIR D:/glfw)
include_directories(${GLFW_DIR}/include)
link_directories(${GLFW_DIR}/lib-mingw-w64)

include_directories(D:/stb)

set(VECTOR_DIR ./Core/Vector)
set(VECTOR_FILE ${VECTOR_DIR}/Vec2.h ${VECTOR_DIR}/Vec3.h ${VECTOR_DIR}/Vec4.h ${VECTOR_DIR}/Color.h)

set(MATRIX_DIR ./Core/Matrix)
set(MATRIX_FILE ${MATRIX_DIR}/Mat3.h ${MATRIX_DIR}/Mat4.h ${MATRIX_DIR}/Quaternion.h)

set(MATERIAL_DIR ./Core/Material)
set(MATERIAL_FILE ${MATERIAL_DIR}/Material.h)

set(LIGHT_DIR ./Core/Light)
set(LIGHT_FILE ${LIGHT_DIR}/Light.h ${LIGHT_DIR}/PointLight.h
        ${LIGHT_DIR}/DirectionLight.h ${LIGHT_DIR}/SpotLight.h)

set(CAMERA_DIR ./Core/Camera)
set(CAMERA_FILE ${CAMERA_DIR}/Camera.h)

set(CONTROLLER_DIR ./Controller)
set(CONTROLLER_FILE ${CONTROLLER_DIR}/LightController.h ${CONTROLLER_DIR}/CameraController.h)

set(RENDER_DIR ./Render)
set(RENDER_FILE ${RENDER_DIR}/Renderer.h ${RENDER_DIR}/Shader.h)

set(CORE_FILE ./Core/function.h ./Core/transform.h
        ${VECTOR_FILE} ${MATRIX_FILE} ${LIGHT_FILE} ${CAMERA_FILE} ${MATERIAL_FILE})

set(MAIN_FILE ./KHeader.h ./Window.h ./main.cpp)

add_executable(${PROJECT_NAME} ${CORE_FILE} ${CONTROLLER_FILE} ${RENDER_FILE} ${MAIN_FILE})
target_link_libraries(${PROJECT_NAME} freeglut glew32 libglfw3)