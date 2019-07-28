#version 150

uniform sampler2D tex0;
uniform float amount;

in vec2 uv;

out vec4 colorOut;

void main()
{  
    vec4 color;
    color.r = texture(tex0, vec2(uv.x + amount, uv.y)).r;
    color.g = texture(tex0, uv).g;
    color.b = texture(tex0, vec2(uv.x - amount, uv.y)).b;
    color.a = texture(tex0, uv).a;

    // Output to screen
    colorOut = color;
}