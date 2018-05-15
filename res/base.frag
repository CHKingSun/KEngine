#version 330 core

struct Texture
{
    sampler2D tex;
    int type;
    bool enable;
};

const int MAX_TEXTURE_NUM = 12;

in color{
    vec4 v_ambient;
    vec4 v_diffuse;
    vec4 v_specular;
};
in vec2 v_texcoord;

uniform Texture u_textures[MAX_TEXTURE_NUM];

out vec4 fragColor;

void main(){
    vec4 ambient = vec4(0.0f);
    vec4 diffuse = vec4(0.0f);
    vec4 specular = vec4(0.0f);

    bool flag[3] = bool[3]( false, false, false ); //no textures
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

    if(flag[0]) ambient *= v_ambient;
    else ambient = v_ambient;

    if(flag[1]) diffuse *= v_diffuse;
    else diffuse = v_diffuse;

    if(flag[2]) specular *= v_specular;
    else specular = v_specular;

    fragColor = (ambient + diffuse + specular) * vec4(v_texcoord, 0.0f, 1.0f);
    // fragColor = vec4(v_texcoord, 0.0f, 1.0f);
}
