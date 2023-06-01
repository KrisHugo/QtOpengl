#version 330 core
out vec4 fragColor;

uniform sampler2D sTexture;
in vec2 vTextureCood;

in vec4 vAmbient;
in vec4 vDiffuse;
in vec4 vSpecular;

void main(void)
{
    vec4 finalColor = texture2D(sTexture, vTextureCood);
    finalColor = finalColor * (vAmbient + vDiffuse + vSpecular);
    fragColor = finalColor;
}
