#version 410 core
out vec4 frag_colour;

in vec2 TexCoord;

uniform sampler2D tex0; // brick
uniform sampler2D tex1; // smiley

void main() {
    vec4 base = texture(tex0, TexCoord);
    vec4 overlay = texture(tex1, TexCoord);
    frag_colour = mix(base, overlay, overlay.a);
}
