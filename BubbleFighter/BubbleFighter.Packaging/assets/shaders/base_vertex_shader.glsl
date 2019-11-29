#version 100

//uniform mat3 worldMx3;
//uniform mat3 viewProjMx3;
uniform mat3 worldViewProjMx3;

attribute vec2 position;
attribute vec2 tex1;
//attribute vec2 tex2;

varying vec2 tex1Coords;

void main()
{
	tex1Coords = tex1;
	gl_Position = worldViewProjMx3 * vec3(Position, 1.0);
}