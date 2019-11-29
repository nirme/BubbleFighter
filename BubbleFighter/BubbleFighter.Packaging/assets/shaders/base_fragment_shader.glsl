#version 100

uniform sampler2D tex1;

varying vec2 tex1Coords;

void main()
{
    gl_FragColor = texture2D(tex1, vTexCoord);
}
