#version 410 core

in vec3 pos;
out vec4 frag_colour;

void main() {
     frag_colour = vec4(pos, 1.0);
}