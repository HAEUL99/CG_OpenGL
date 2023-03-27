#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//projection -> view -> model 
uniform mat4 transform;
//local -> world 
uniform mat4 modelTransform;

out vec3 normal;
out vec2 texCoord;
out vec3 position;

struct Obj {
    vec3 position;
    vec3 direction;
};
uniform Obj obj;

void main() {

    gl_Position = transform * vec4(aPos, 1.0);
    //normal = aNormal;
    //normal = (modelTransform * vec4(aNormal, 0.0)).xyz;
    normal = (transpose(inverse(modelTransform)) * vec4(aNormal, 0.0)).xyz;
    texCoord = aTexCoord;
    //world coordinate 
    position = vec3(modelTransform * vec4(aPos, 1.0));
    
}   