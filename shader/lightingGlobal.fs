#version 330 core
precision highp float;

uniform sampler2D textureUnit;
in  vec2 texcoord;			// interpolated texture coordinates
out vec4 fragmentColor;		// output that goes to the raster memory as told by glBindFragDataLocation

void main() {
    fragmentColor = texture(textureUnit, texcoord);
}