#version 120
attribute vec2 coord_2d;
attribute vec3 coord_colour;

varying vec3 frag_colour;

void main(void) 
{
  gl_Position = vec4(coord_2d, 0.0, 1.0);
  frag_colour = coord_colour;
}
