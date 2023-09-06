#version 330
precision highp float;

layout(location = 0) in vec2 cVertexPosition;	// Attrib Array 0
out vec2 texcoord;

void main() {
    texcoord = (cVertexPosition + vec2(1, 1))/2;							// -1,1 to 0,1
    gl_Position = vec4(cVertexPosition.x, cVertexPosition.y, 0, 1); 		// transform to clipping space
}