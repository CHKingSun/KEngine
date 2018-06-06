#version 330 core

struct Texture {
    sampler2D tex;
    int type;
    bool enable;
};

const int MAX_TEXTURE_NUM = 12;

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

void calALight(ALight l, inout vec4 v_ambient) {
    v_ambient += l.ambient * l.factor;
}

void calDLight(DLight l, vec3 N, vec3 E, float u_shininess,
    inout vec4 v_ambient, inout vec4 v_diffuse, inout vec4 v_specular) { //N and E are normalized;
    vec3 L = normalize(-l.direction);
    float cosT = max(dot(L, N), 0.0);
    float cosA = 0.0;
    if(cosT != 0.0) cosA = max(dot(N, normalize(L + E)), 0.0);

    v_ambient += l.ambient * l.factor;
    v_diffuse += l.diffuse * cosT * l.factor;
    v_specular += l.specular * pow(cosA, u_shininess) * l.factor;
}

void calPLight(PLight l, vec3 N, vec3 E, vec3 m_pos, float u_shininess,
    inout vec4 v_ambient, inout vec4 v_diffuse, inout vec4 v_specular) {
    vec3 l_dir = l.position - m_pos;
    float dis = length(l_dir);
    float attenuation = 1.0 / (l.kc + dis * l.kl + dis * dis * l.kq);
    vec3 L = normalize(l_dir);
    float cosT = max(dot(L, N), 0.0);
    float cosA = max(dot(N, normalize(L + E)), 0.0);

    attenuation *= l.factor;
    v_ambient += l.ambient * attenuation;
    v_diffuse += l.diffuse * cosT * attenuation;
    v_specular += l.specular * pow(cosA, u_shininess) * attenuation;
}

void calSLight(SLight l, vec3 N, vec3 E, vec3 m_pos, float u_shininess,
    inout vec4 v_ambient, inout vec4 v_diffuse, inout vec4 v_specular) {
    vec3 l_dir = l.position - m_pos;
    float dis = length(l_dir);

    vec3 L = normalize(l_dir);
    float intensity = smoothstep(l.outerCutOff, l.innerCutOff, dot(normalize(l.direction), -L));
    if(intensity == 0) return;
    float attenuation = 1.0 / (l.kc + dis * l.kl + dis * dis * l.kq);
    float cosT = max(dot(L, N), 0.0);
    float cosA = max(dot(N, normalize(L + E)), 0.0);

    attenuation *= l.factor * intensity;
    v_ambient += l.ambient * attenuation;
    v_diffuse += l.diffuse * cosT * attenuation;
    v_specular += l.specular * pow(cosA, u_shininess) * attenuation;
}

in vec3 v_N;
in vec3 v_E;
in vec3 v_mPos;
// in vec4 v_shadowcoord;
in vec2 v_texcoord;

layout(std140) uniform material {
    float u_shininess;
    vec4 u_ambient;
    vec4 u_diffuse;
    vec4 u_specular;
};

layout(std140) uniform lights {
    ALight u_aLights[MAX_LIGHTS_NUM];
    PLight u_pLights[MAX_LIGHTS_NUM];
    DLight u_dLights[MAX_LIGHTS_NUM];
    SLight u_sLights[MAX_LIGHTS_NUM];
};

uniform Texture u_textures[MAX_TEXTURE_NUM];

// uniform samplerCubeShadow u_shadow_texture;

out vec4 fragColor;

void main() {
    vec4 ambient = vec4(0.0f);
    vec4 diffuse = vec4(0.0f);
    vec4 specular = vec4(0.0f);

    bool flag[3] = bool[3]( false, false, false );
    for(int i = 0; i < MAX_TEXTURE_NUM; ++i) {
        if(u_textures[i].enable) {
            if(u_textures[i].type == 1) {
                flag[0] = true;
                ambient += texture(u_textures[i].tex, v_texcoord);
            } else if(u_textures[i].type == 2) {
                flag[1] = true;
                diffuse += texture(u_textures[i].tex, v_texcoord);
            } else if(u_textures[i].type == 3) {
                flag[2] = true;
                specular += texture(u_textures[i].tex, v_texcoord);
            }
        }
    }

    if(flag[0]) ambient *= u_ambient;
    else ambient = u_ambient * vec4(0.3f, 0.3f, 0.3f, 1.0f);
    if(flag[1]) diffuse *= u_diffuse;
    else diffuse = u_diffuse * vec4(0.3f, 0.3f, 0.3f, 1.0f);
    if(flag[2]) specular *= u_specular;
    else specular = u_specular * vec4(0.3f, 0.3f, 0.3f, 1.0f);

    vec4 v_ambient = vec4(0.0f);
    vec4 v_diffuse = vec4(0.0f);
    vec4 v_specular = vec4(0.0f);
    flag[0] = false;
    flag[1] = false;
    if(v_N == vec3(0.0f)) { //no normals
        for(int i = 0; i < MAX_LIGHTS_NUM; ++i) {
            if(u_aLights[i].enable) {
                flag[0] = true;
                calALight(u_aLights[i], v_ambient);
            }
        }
    } else {
        for(int i = 0; i < MAX_LIGHTS_NUM; ++i) {
            if(u_aLights[i].enable) {
                flag[0] = true;
                calALight(u_aLights[i], v_ambient);
            }
            if(u_dLights[i].enable) {
                flag[1] = true;
                calDLight(u_dLights[i], v_N, v_E, u_shininess, v_ambient, v_diffuse, v_specular);
            }
            if(u_pLights[i].enable) {
                flag[1] = true;
                calPLight(u_pLights[i], v_N, v_E, v_mPos, u_shininess, v_ambient, v_diffuse, v_specular);
            }
            if(u_sLights[i].enable) {
                flag[1] = true;
                calSLight(u_sLights[i], v_N, v_E, v_mPos, u_shininess, v_ambient, v_diffuse, v_specular);
            }
        }
    }

    vec4 minVal = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    vec4 maxVal = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    v_ambient = clamp(v_ambient, minVal, maxVal);
    v_diffuse = clamp(v_diffuse, minVal, maxVal);
    v_specular = clamp(v_specular, minVal, maxVal);

    if(flag[1]) {
        // float shadow = texture(u_shadow_texture, v_shadowcoord);
        fragColor = v_ambient * ambient + v_diffuse * diffuse + v_specular * specular;
        // fragColor = vec4(vec3(shadow), 1.0f);
        // fragColor = texture(u_shadow_texture, v_mPos);
    } else if(flag[0]) {
        fragColor = v_ambient * ambient;
    } else {
        fragColor = ambient;
    }
}
