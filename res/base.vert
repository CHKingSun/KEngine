#version 330 core

struct ALight{ //Ambient light
    int type;
    bool enable;

    float factor;

    vec4 ambient;
};

struct DLight{ //Direction light
    int type;
    bool enable;
    vec3 direction;

    float factor;
    float shadowFactor;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

struct PLight{ //Point Light
    int type;
    bool enable;
    vec3 position;

    float factor;
    float shadowFactor;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float kc, kl, kq;
};

struct SLight{ //Spot Light
    int type;
    bool enable;
    vec3 position;
    vec3 direction;

    float factor;
    float shadowFactor;

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

//uniform blocks has no location but has binding
//but binding requires #version 420
layout(std140, row_major) uniform model{
    vec3 u_mPos;
    vec3 u_mScale;
    mat3 u_mRotate;
    //model_view * vec4(position, 1.0) = u_mPos + (u_mRotate * (u_mScale * position));
    //u_NMatrix * normal = u_mRotate * (u_mScale * normal)；
};

layout(std140, row_major) uniform material{
    float u_shininess;
    vec4 u_ambient;
    vec4 u_diffuse;
    vec4 u_specular;
};

layout(std140, row_major) uniform projection{
    vec3 p_eye;
    mat4 u_view;
    mat4 u_proj;
};

uniform ALight u_aLights[MAX_LIGHTS_NUM];
uniform PLight u_pLights[MAX_LIGHTS_NUM];
uniform DLight u_dLights[MAX_LIGHTS_NUM];
uniform SLight u_sLights[MAX_LIGHTS_NUM];

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
    v_ambient += l.ambient;
}

void calDLight(DLight l, vec3 N, vec3 E) { //N and E are normalized;
    vec3 L = normalize(-l.direction);
    float cosT = max(dot(L, N), 0.0);
    float cosA = max(dot(E, reflect(-L, N)), 0.0);

    v_ambient += l.ambient;
    v_diffuse += l.diffuse * cosT;
    v_specular += l.specular * pow(cosA, u_shininess);
}

void calPLight(PLight l, vec3 N, vec3 E, vec3 m_pos) {
    vec3 l_dir = l.position - m_pos;
    float dis = length(l_dir);
    float attenuation = 1.0 / (l.kc + dis * l.kl + dis * dis * l.kq);
    vec3 L = normalize(l_dir);
    float cosT = max(dot(L, N), 0.0);
    float cosA = max(dot(E, reflect(-L, N)), 0.0);

    v_ambient += l.ambient * attenuation;
    v_diffuse += l.diffuse * cosT * attenuation;
    v_specular += l.specular * pow(cosA, u_shininess) * attenuation;
}

void calSLight(SLight l, vec3 N, vec3 E, vec3 m_pos) {
    vec3 l_dir = l.position - m_pos;
    float intensity = smoothstep(dot(l.direction, l_dir), l.outerCutOff, l.innerCutOff);
    if(intensity == 0.0) return;

    float dis = length(l_dir);
    float attenuation = 1.0 / (l.kc + dis * l.kl + dis * dis * l.kq);
    vec3 L = normalize(l_dir);
    float cosT = max(dot(L, N), 0.0);
    float cosA = max(dot(E, reflect(-L, N)), 0.0);

    v_ambient += l.ambient * attenuation;
    v_diffuse += l.diffuse * cosT * attenuation * intensity;
    v_specular += l.specular * pow(cosA, u_shininess) * attenuation * intensity;
}

void main() {
    v_ambient = vec4(0.0f);
    v_diffuse = vec4(0.0f);
    v_specular = vec4(0.0f);

    vec3 m_pos = u_mPos + (u_mRotate * (u_mScale * a_position));
    gl_Position = vec4(m_pos, 1.0);
    // gl_Position = u_proj * (u_view * vec4(m_pos, 1.0));

    bool flag[2] = bool[2]( false, false ); //if no lights
    if(a_normal == vec3(0.0f)) { //no normals
        for(int i = 0; i < MAX_LIGHTS_NUM; ++i) {
            if(u_aLights[i].enable) {
                flag[0] = true;
                calALight(u_aLights[i]);
            }
        }
    } else {
        vec3 N = normalize(u_mRotate * (u_mScale * a_normal));
        vec3 E = normalize(p_eye - m_pos);

        for(int i = 0; i < MAX_LIGHTS_NUM; ++i) {
            if(u_aLights[i].enable) {
                flag[0] = true;
                calALight(u_aLights[i]);
            }
            if(u_dLights[i].enable) {
                flag[1] = true;
                flag[0] = true;
                calDLight(u_dLights[i], N, E);
            }
            if(u_pLights[i].enable) {
                flag[1] = true;
                flag[0] = true;
                calPLight(u_pLights[i], N, E, m_pos);
            }
            if(u_sLights[i].enable) {
                flag[1] = true;
                flag[0] = true;
                calSLight(u_sLights[i], N, E, m_pos);
            }
        }
    }

    if(flag[1]) {
        v_ambient *= u_ambient;
        v_diffuse *= u_diffuse;
        v_specular *= u_specular;
    } else if(flag[0]) {
        v_ambient *= u_ambient;
        v_diffuse = u_diffuse;
        v_specular = u_specular;
    } else {
        // v_ambient = u_ambient;
        v_diffuse = u_diffuse;
        // v_specular = u_specular;
    }

    b_pos = a_position;
    b_scale = u_mScale;
    b_rotate = u_mRotate;
    b_out = mat4(vec4(m_pos, 1.0), v_ambient, v_diffuse, v_specular);

    v_texcoord = a_texcoord;
}
