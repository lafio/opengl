#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

struct Material{
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct LightPoint{
	float constant;
	float linear;
	float quadratic;
};
struct LightSpot{
	float cosPhyInner;
	float cosPhyOutter;
};

uniform Material material;
uniform LightPoint lightp;
uniform LightSpot lights;
//uniform sampler2D ourTexture;
//uniform sampler2D ourFace;
uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightPos;
uniform vec3 lightDirUniform;
uniform vec3 lightColor;
uniform vec3 cameraPos;

out vec4 FragColor;
void main(){
	float dist = length(lightPos - FragPos);
	float attenuation = 1.0/(lightp.constant + lightp.linear*dist+lightp.quadratic*(dist*dist));

	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 reflectVec = reflect(-lightDir,Normal);
	vec3 cameraVec = normalize(cameraPos - FragPos);

	//specular
	float specularAmount = pow(max(dot(reflectVec,cameraVec),0),material.shininess);
	vec3 specular = texture(material.specular,TexCoord).rgb * specularAmount * lightColor;

	//FragColor = vec4(objColor * ambientColor,1.0);

	//diffuse
	vec3 diffuse = texture(material.diffuse,TexCoord).rgb * max(dot(lightDir,Normal),0) * lightColor;
	//vec3 diffuse = texture(material.diffuse,TexCoord).rgb;

	//ambient
	vec3 ambient = texture(material.diffuse,TexCoord).rgb * ambientColor;

	float cosTheta = dot(normalize(FragPos - lightPos), -1 * lightDirUniform);

	float spotRatio;
	if(cosTheta > lights.cosPhyInner){
		//inside
		spotRatio = 1.0f;
	}
	else if(cosTheta > lights.cosPhyOutter){
		//middle
		spotRatio = 1.0f - pow((cosTheta - lights.cosPhyInner) / (lights.cosPhyOutter - lights.cosPhyInner),2);
	}
	else{
		//outside
		spotRatio = 0;
	}
	FragColor = vec4((ambient + (diffuse + specular) * spotRatio) * objColor,1.0);
}