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

uniform Texture textures[MAX_TEXTURE_NUM];

out vec4 fragColor;

void main(){
    vec4 ambient = vec4(0.0f);
    vec4 diffuse = vec4(0.0f);
    vec4 specular = vec4(0.0f);

    for(int i = 0; i < MAX_TEXTURE_NUM; ++i){
        if(textures[i].enable){
            if(textures[i].type == 1) ambient = texture(textures[i].tex, v_texcoord);
            else if(textures[i].type == 2) diffuse = texture(textures[i].tex, v_texcoord);
            else if(textures[i].type == 3) specular = texture(textures[i].tex, v_texcoord);
        }
    }

    fragColor = ambient * v_ambient + diffuse * v_diffuse + specular * v_specular;
}
