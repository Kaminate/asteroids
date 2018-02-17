#version 430

layout(location = 6) uniform mat4 lightView;
layout(location = 7) uniform mat4 lightProj;
layout(location = 8) uniform vec4 color;
layout(location = 9) uniform vec3 wsLightDir;
layout(location = 10) uniform vec4 lightCol;
layout(location = 11) uniform sampler2D shadowMap;

in vec3 wsNormal;
in vec4 wsPos;

out vec4 outColor;

void main()
{
	// take our position into lightspace
	vec4 posLightNDC = lightProj * lightView * wsPos;
	posLightNDC /= posLightNDC.w;

	// [-1, 1] --> [0, 1]
	vec2 lightTexCoord = vec2(posLightNDC.x, posLightNDC.y);
	lightTexCoord += 1.0;
	lightTexCoord /= 2.0;

	// [-1, 1] --> [0, 1]
	float lsDepth = (posLightNDC.z + 1.0) / 2.0;

  float bufferDist = 0.001;

	// [0, 1]

	float shadowDepth = texture(shadowMap, lightTexCoord);
  float shadow = 1.0;
	if (lsDepth - shadowDepth > bufferDist)
		shadow = 0.5; // occluded

	// float nDotl = max(0, dot(wsNormal, -wsLightDir));

	//temp, replace with above
	float nDotl = max(0, dot(normalize(wsNormal + vec3(0,0,0.5)), -wsLightDir));

	outColor = shadow * color * lightCol * nDotl;
	//outColor = vec4(vec3(shadowDepth), 1.0);
	// outColor.r += wsNormal.r / 1000.0;
	// outColor.r += color.r / 1000.0;
	// outColor.r += wsLightDir.r / 1000.0;
	// outColor.r += lightCol.r / 1000.0;
}
