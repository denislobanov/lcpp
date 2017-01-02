#version 120
varying vec3 frag_colour;

void main(void)
{
  gl_FragColor = vec4(frag_colour.r, frag_colour.g, frag_colour.b, 1.0);
}
