#version 120
attribute vec3 coord_3d;
attribute vec3 coord_colour;

uniform mat4 mvp;
varying vec3 frag_colour;

void main(void)
{
  gl_Position = mvp*vec4(coord_3d, 1.0);
  frag_colour = coord_colour;
}
