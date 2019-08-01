#version 150

uniform sampler2D tex0;

uniform float _amount;
uniform vec2 _resolution;

in vec2 uv;

out vec4 colorOut;

const float OuterRadius = 1.2;

void main()
{  
	vec4 texColor = texture2D(tex0, uv);
    vec2 centered = (uv - vec2(0.5)) * (_resolution.x / _resolution.y);
	
	colorOut = vec4(1.0);
	colorOut.rgb *=  1.0 - smoothstep(OuterRadius-_amount, OuterRadius, length(centered));
	colorOut *= texColor;

    colorOut = mix(texColor, colorOut, 0.7);
}