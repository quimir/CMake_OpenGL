#version 330 core
out vec4 frag_color;

uniform vec4 frag_color_value;

void main() {
  frag_color = frag_color_value;
}