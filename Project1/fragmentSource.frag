#version 330 core
in vec4 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D ourFace;
uniform vec3 objColor;
uniform vec3 ambientColor;

void main(){
	//FragColor = vertexColor;
	//FragColor = mix(texture(ourTexture,TexCoord),texture(ourFace,TexCoord),0.2);
	FragColor = vec4(objColor*ambientColor,1.0f)*mix(texture(ourTexture,TexCoord),texture(ourFace,TexCoord),0.2);
}