#version 330 core

//in blocks is not allowed in vertex shader
layout(location = 1) in vec3 a_position;
layout(location = 3) in vec2 a_texcoord;
layout(location = 4) in mat4 a_matrix; //multiple render, it will use 4 locations to save, must be column major

//uniform blocks has no location but has binding
//but binding requires #version 420
layout(std140, row_major) uniform model {
    vec3 u_mPos;
    vec3 u_mScale;
    mat3 u_mRotate;
    bool u_is_multiple;
    //model_view * vec4(position, 1.0) = u_mPos + (u_mRotate * (u_mScale * position));
    //u_NMatrix * normal = u_mRotate * (u_mScale * normal);
};

layout(std140, row_major) uniform projection {
    vec3 p_eye;
    mat4 u_view;
    mat4 u_proj;
};

out vec2 v_texcoord;

void main() {

    vec3 m_pos;
    if(!u_is_multiple) m_pos = u_mPos + (u_mRotate * (u_mScale * a_position));
    else m_pos = u_mPos + (u_mRotate * (u_mScale * (a_matrix * vec4(a_position, 1.0)).xyz));
    gl_Position = u_proj * (u_view * vec4(m_pos, 1.0));

    v_texcoord = a_texcoord;
}
