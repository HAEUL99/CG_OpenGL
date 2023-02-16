#version 330 core
in vec4 vertexColor;
in vec2 texCoord;
out vec4 fragColor;

//텍스처
uniform sampler2D tex;

void main() {
    fragColor = texture(tex, texCoord);
} 