#version 440

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;

out vec4 frag_colour;

uniform sampler2D ourTexture;

void main () 
{
	
        float r1= 0.8f;
        float r=(vs_position.x*vs_position.x) + (vs_position.y*vs_position.y);
        if (r1*r1>=r)
        {
            frag_colour = texture(ourTexture, vs_texcoord);
        }
        else 
	{
	    frag_colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}
