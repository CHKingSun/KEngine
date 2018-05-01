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

struct PLight{
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

struct SLight{
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

const int MAX_LIGHTS = 4;

in point{
    layout(location=1) vec3 a_position;
    layout(location=2) vec3 a_normal;
    layout(location=3) vec2 a_texcoord;
};

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat3 u_NMatrix;
uniform vec3 p_eye;

uniform material{
    vec4 u_ambient;
    vec4 u_diffuse;
    vec4 u_specular;
    float u_shininess;
};

uniform ALight u_aLights[MAX_LIGHTS];
uniform PLight u_pLights[MAX_LIGHTS];
uniform DLight u_dLights[MAX_LIGHTS];
uniform SLight u_sLights[MAX_LIGHTS];

out Color{
    vec4 v_ambient;
    vec4 v_diffuse;
    vec4 v_specular;
};
out vec2 v_texcoord;

void calALight(ALight l){
    v_ambient += l.ambient;
}

void calDLight(DLight l, vec3 N, vec3 E){ //N and E are normalized;
    vec3 L = normalize(-l.direction);
    float cosT = max(dot(L, N), 0.0);
    float cosA = max(dot(E, reflect(-L, N)), 0.0);

    v_ambient += l.ambient;
    v_diffuse += l.diffuse * cosT;
    v_specular += l.specular * pow(cosA, u_shininess);
}

void calPLight(PLight l, vec3 N, vec3 E, vec3 m_pos){
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

void calSLight(SLight l, vec3 N, vec3 E, vec3 m_pos){
    vec3 l_dir = l.position - m_pos;
    float intensity = smoothstep(dot(l.direction, l_dir), outerCutOff, innerCutOff);
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

    vec3 N = normalize(NMatrix * normal);
    vec3 m_pos = vec3(u_model * vec4(a_position, 1,0));
    vec3 E = normalize(p_eye - m_pos);

    for(int i = 0; i < MAX_LIGHTS; ++i){
        if(u_aLights[i].enable) calALight(u_aLights[i]);
        if(u_dLights[i].enable) calDLight(u_dLights[i], N, E);
        if(u_pLights[i].enable) calPLight(u_pLights[i], N, E, m_pos);
        if(u_sLights[i].enable) calSLight(u_sLights[i], N, E, m_pos);
    }

    v_ambient *= u_ambient;
    v_diffuse *= u_diffuse;
    v_specular *= u_specular;
}
