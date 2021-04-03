#version 330 core

out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D gridTexture;
uniform int isApple;

void main()
{
    //FragColor = texture(gridTexture, texCoord);
    if (isApple == 0){
        FragColor = vec4(0.9, 0.5, 0.3, 1.0);
    } else {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }

}