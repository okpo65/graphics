#version 120

attribute vec3 vertexPosition_modelspace;

// Output data ; will be interpolated for each fragment.
varying vec2 UV;

uniform mat4 ModelTransform;
uniform mat4 Eye;
uniform mat4 Projection;

out vec3 fragmentPosition;
out vec3 fragmentNormal;
out vec3 fragmentColor;

mat4 NormalMatrix(mat4 MVM)
{
	mat4 invm = inverse(MVM);
	invm[0][3] = 0;
	invm[1][3] = 0;
	invm[2][3] = 0;

	return transpose(invm);
}

void main()
{

    mat4 MVM = inverse(Eye) * ModelTransform;
    mat4 NVM = NormalMatrix(MVM);
    vec4 wPosition = MVM * vec4(vertexPosition_modelspace,1);

    fragmentPosition = wPosition.xyz;
    gl_Position = Projection * wPosition;
	//gl_Position = Projection * Eye * ModelTransform * vec4(vertexPosition_modelspace, 1);

	fragmentNormal = (NVM * vec4(vertexNormal_modelspace,1.0)).xyz;

    	// The color of each vertex will be interpolated
    	// to produce the color of each fragment

    	fragmentColor = vertexColor;

}