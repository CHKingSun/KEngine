#version 330 core

struct ALight { //Ambient light
    bool enable;

    float factor;

    vec4 ambient;
};

struct DLight { //Direction light
    bool enable;

    float factor;
    float shadowFactor;

    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PLight { //Point Light
    bool enable;

    float factor;
    float shadowFactor;

    vec3 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float kc, kl, kq;
};

struct SLight { //Spot Light
    bool enable;

    float factor;
    float shadowFactor;

    vec3 position;
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float kc, kl, kq;
    float innerCutOff, outerCutOff;
};

const int MAX_LIGHTS_NUM = 4;

//in blocks is not allowed in vertex shader
layout(location = 1) in vec3 a_position;
layout(location = 2) in vec3 a_normal;
layout(location = 3) in vec2 a_texcoord;
layout(location = 4) in mat4 a_matrix; //multiple render, it will use 4 locations to save, must be column major.

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

layout(std140) uniform material {
    float u_shininess;
    vec4 u_ambient;
    vec4 u_diffuse;
    vec4 u_specular;
};

layout(std140, row_major) uniform projection {
    vec3 p_eye;
    mat4 u_view;
    mat4 u_proj;
};

layout(std140) uniform lights {
    ALight u_aLights[MAX_LIGHTS_NUM];
    PLight u_pLights[MAX_LIGHTS_NUM];
    DLight u_dLights[MAX_LIGHTS_NUM];
    SLight u_sLights[MAX_LIGHTS_NUM];
}

out feedback{
    mat4 b_out;
    vec3 b_pos;
    vec3 b_scale;
    mat3 b_rotate;
};

out color{
    vec4 v_ambient;
    vec4 v_diffuse;
    vec4 v_specular;
};
out vec2 v_texcoord;

void calALight(ALight l) {
    v_ambient += l.ambient * l.factor;
}

void calDLight(DLight l, vec3 N, vec3 E) { //N and E are normalized;
    vec3 L = normalize(-l.direction);
    float cosT = max(dot(L, N), 0.0);
    float cosA = 0.0;
    if(cosT != 0.0) cosA = max(dot(N, normalize(L + E)), 0.0);

    v_ambient += l.ambient * l.factor;
    v_diffuse += l.diffuse * cosT * l.factor;
    v_specular += l.specular * pow(cosA, u_shininess) * l.factor;
}

void calPLight(PLight l, vec3 N, vec3 E, vec3 m_pos) {
    vec3 l_dir = l.position - m_pos;
    float dis = length(l_dir);
    float attenuation = 1.0 / (l.kc + dis * l.kl + dis * dis * l.kq);
    vec3 L = normalize(l_dir);
    float cosT = max(dot(L, N), 0.0);
    float cosA = 0.0;
    if(cosT != 0.0) cosA = max(dot(N, normalize(L + E)), 0.0);

    attenuation *= l.factor;
    v_ambient += l.ambient * attenuation;
    v_diffuse += l.diffuse * cosT * attenuation;
    v_specular += l.specular * pow(cosA, u_shininess) * attenuation;
}

void calSLight(SLight l, vec3 N, vec3 E, vec3 m_pos) {
    vec3 l_dir = l.position - m_pos;
    float dis = length(l_dir);

    vec3 L = normalize(l_dir);
    float intensity = smoothstep(l.outerCutOff, l.innerCutOff, dot(l.direction, -L));
    if(intensity == 0) return;
    float attenuation = 1.0 / (l.kc + dis * l.kl + dis * dis * l.kq);
    float cosT = max(dot(L, N), 0.0);
    float cosA = 0.0;
    if(cosT != 0.0) cosA = max(dot(N, normalize(L + E)), 0.0);

    attenuation *= l.factor * intensity;
    v_ambient += l.ambient * attenuation;
    v_diffuse += l.diffuse * cosT * attenuation;
    v_specular += l.specular * pow(cosA, u_shininess) * attenuation;
}

void main() {
    v_ambient = vec4(0.0f);
    v_diffuse = vec4(0.0f);
    v_specular = vec4(0.0f);

    vec3 m_pos;
    if(!u_is_multiple) m_pos = u_mPos + (u_mRotate * (u_mScale * a_position));
    else m_pos = u_mPos + (u_mRotate * (u_mScale * (a_matrix * vec4(a_position, 1.0)).xyz));
    gl_Position = u_proj * (u_view * vec4(m_pos, 1.0));

    bool flag[2] = bool[2]( false, false ); //if no lights
    if(a_normal == vec3(0.0f)) { //no normals
        for(int i = 0; i < MAX_LIGHTS_NUM; ++i) {
            if(u_aLights[i].enable) {
                flag[0] = true;
                calALight(u_aLights[i]);
            }
        }
    } else {
        vec3 N;
        if(!u_is_multiple) N = normalize(u_mRotate * (u_mScale * a_normal));
        else N = normalize(u_mRotate * (u_mScale * (a_matrix * vec4(a_normal, 0.0)).xyz));
        vec3 E = normalize(p_eye - m_pos);

        for(int i = 0; i < MAX_LIGHTS_NUM; ++i) {
            if(u_aLights[i].enable) {
                flag[0] = true;
                calALight(u_aLights[i]);
            }
            if(u_dLights[i].enable) {
                flag[1] = true;
                calDLight(u_dLights[i], N, E);
            }
            if(u_pLights[i].enable) {
                flag[1] = true;
                calPLight(u_pLights[i], N, E, m_pos);
            }
            if(u_sLights[i].enable) {
                flag[1] = true;
                calSLight(u_sLights[i], N, E, m_pos);
            }
        }
    }

    vec4 minVal = vec4(0.0, 0.0, 0.0, 0.0);
    vec4 maxVal = vec4(1.0, 1.0, 1.0, 1.0);
    v_ambient = clamp(v_ambient, minVal, maxVal);
    v_diffuse = clamp(v_diffuse, minVal, maxVal);
    v_specular = clamp(v_specular, minVal, maxVal);

    if(flag[1]) {
        v_ambient *= u_ambient;
        v_diffuse *= u_diffuse;
        v_specular *= u_specular;
    } else if(flag[0]) {
        v_ambient *= u_ambient;
    } else {
        v_ambient = u_ambient;
    }

    b_pos = a_normal;
    b_scale = u_mScale;
    b_rotate = u_mRotate;
    b_out = mat4(vec4(u_sLights[0].direction, u_sLights[0].outerCutOff),
                vec4(u_sLights[0].position, u_sLights[0].innerCutOff),
                vec4(m_pos, 1.0), vec4(a_normal, 0.0));

    v_texcoord = a_texcoord;
}
