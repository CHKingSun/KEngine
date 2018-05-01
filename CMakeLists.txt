cmake_minimum_required(VERSION 3.10)
project(KEngine)

set(CMAKE_CXX_STANDARD 11)

set(VECTOR_DIR ./Core/Vector)
set(VECTOR_FILE ${VECTOR_DIR}/Vec2.h ${VECTOR_DIR}/Vec3.h ${VECTOR_DIR}/Vec4.h ${VECTOR_DIR}/Color.h)

set(MATRIX_DIR ./Core/Matrix)
set(MATRIX_FILE ${MATRIX_DIR}/Mat3.h ${MATRIX_DIR}/Mat4.h)

set(LIGHT_DIR ./Core/Light)
set(LIGHT_FILE ${LIGHT_DIR}/Light.h ${LIGHT_DIR}/PointLight.h
        ${LIGHT_DIR}/DirectionLight.h ${LIGHT_DIR}/SpotLight.h)

set(CONTROLLER_DIR ./Core/Controller)
set(CONTROLLER_FILE ${CONTROLLER_DIR}/LightController.h)

set(CORE_FILE ./Core/KHeader.h ./Core/function.h ./Core/transform.h
              ${VECTOR_FILE} ${MATRIX_FILE} ${LIGHT_FILE} ${CONTROLLER_FILE})

add_executable(${PROJECT_NAME} ${CORE_FILE} main.cpp)