#version 410 core

in vec3 vertex_position;
out vec3 pos;

void main() {
  pos = vertex_position;

  gl_Position = vec4(pos, 1.0);
}