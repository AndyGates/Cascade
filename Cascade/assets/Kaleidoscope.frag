#version 150

uniform sampler2D tex0;
uniform int _amount;

in vec2 uv;

out vec4 colorOut;

const float TWO_PI = 6.283185307179586476925286766559;

vec2 calcUV(vec2 uv)
{
	uv -= vec2(0.5);

	float sampleAngle = TWO_PI / _amount;

	//Get angle and distance from position
	float t = atan(uv.y, uv.x);
	float d = length(uv);

	//mod the angle to within the sample range
	t = abs(mod(t, sampleAngle) - (sampleAngle/2.0));

	//Map back to position
	return (vec2(cos(t), sin(t)) * d) + vec2(0.5);
}

void main()
{  
    vec4 color;
	vec2 nuv = calcUV(uv);

    color = texture(tex0, nuv);

    // Output to screen
    colorOut = color;
}