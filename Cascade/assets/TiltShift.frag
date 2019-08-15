#version 150

uniform sampler2D _tex0;
uniform float _distance;
uniform float _blurRadius;
uniform vec2 _direction;

in vec2 uv;

out vec4 colorOut;

void main()
{  
    vec4 color;

	float amount = clamp( abs((uv.y * 2.0) - 1.0) - _distance, 0, 1);

	float blurX = _direction.x * _blurRadius * amount;
	float blurY = _direction.y * _blurRadius * amount;
	
	//sample texture with precomputed gaussian weightings
	color += texture2D(_tex0, vec2(uv.x - 4.0*blurX, uv.y - 4.0*blurY)) * 0.0162162162;
	color += texture2D(_tex0, vec2(uv.x - 3.0*blurX, uv.y - 3.0*blurY)) * 0.0540540541;
	color += texture2D(_tex0, vec2(uv.x - 2.0*blurX, uv.y - 2.0*blurY)) * 0.1216216216;
	color += texture2D(_tex0, vec2(uv.x - 1.0*blurX, uv.y - 1.0*blurY)) * 0.1945945946;
	color += texture2D(_tex0, vec2(uv.x,			 uv.y))				* 0.2270270270;
	color += texture2D(_tex0, vec2(uv.x + 1.0*blurX, uv.y + 1.0*blurY)) * 0.1945945946;
	color += texture2D(_tex0, vec2(uv.x + 2.0*blurX, uv.y + 2.0*blurY)) * 0.1216216216;
	color += texture2D(_tex0, vec2(uv.x + 3.0*blurX, uv.y + 3.0*blurY)) * 0.0540540541;
	color += texture2D(_tex0, vec2(uv.x + 4.0*blurX, uv.y + 4.0*blurY)) * 0.0162162162;

    // Output to screen
    colorOut = color;
}