#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 frag_colour;

uniform sampler2D ourTexture;

void main () 
{
    frag_colour = texture(ourTexture, vs_texcoord);	
}
