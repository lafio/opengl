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
struct LightDirectional{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
};
struct LightPoint{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
};
struct LightSpot{
	vec3 pos;
	vec3 color;
	vec3 dirToLight;
	float constant;
	float linear;
	float quadratic;
	float cosPhyInner;
	float cosPhyOutter;
};

uniform Material material;
uniform LightDirectional lightd;
uniform LightPoint lightp0;
uniform LightPoint lightp1;
uniform LightPoint lightp2;
uniform LightPoint lightp3;
uniform LightSpot lights;

uniform vec3 objColor;
uniform vec3 ambientColor;
uniform vec3 lightDirUniform;
uniform vec3 cameraPos;

out vec4 FragColor;

vec3 CalcLightDirectional(LightDirectional light,vec3 uNormal,vec3 dirToCamera){
	
	//diffuse max(dot(L,N),0)
	float diffIntensity = max(dot(light.dirToLight,uNormal),0);
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse,TexCoord).rgb;

	//specular pow(max(dot(R,Cam),0),shininess)
	vec3 R = normalize(reflect(-light.dirToLight,uNormal));
	float specIntensity = pow(max(dot(R,dirToCamera),0),material.shininess);
	vec3 specColor = specIntensity * light.color * texture(material.specular,TexCoord).rgb;

	vec3 result = diffColor + specColor;
	return result;
}

vec3 CalcLightPoint(LightPoint light,vec3 uNormal,vec3 dirToCamera){
	//attenuation
	float dist = length( light.pos - FragPos);
	float attenuation = 1/(light.constant+light.linear*dist+light.quadratic*(dist*dist));
	//diffuse
	float diffIntensity = max(dot(normalize(light.pos-FragPos),uNormal),0) * attenuation;
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse,TexCoord).rgb;
	//specular
	vec3 R = normalize(reflect(-normalize(light.pos-FragPos),uNormal));
	float specIntensity = pow(max(dot(R,dirToCamera),0),material.shininess) * attenuation;
	vec3 specColor = specIntensity * light.color * texture(material.specular,TexCoord).rgb;

	vec3 result = diffColor + specColor;
	return result;
}
vec3 CalcLightSpot(LightSpot light,vec3 uNormal,vec3 dirToCamera){
	//attenuation
	float dist = length( light.pos - FragPos);
	float attenuation = 1/(light.constant+light.linear*dist+light.quadratic*(dist*dist));
	float spotRatio;
	float CosTheta = dot(normalize(FragPos - light.pos),-light.dirToLight);

	if(CosTheta > light.cosPhyInner){
		spotRatio = 1.0;
	}
	else if(CosTheta > light.cosPhyOutter){
		spotRatio = (CosTheta-light.cosPhyOutter) / (light.cosPhyInner - light.cosPhyOutter);
	}
	else{
		spotRatio = 0;
	}
	//diffuse
	float diffIntensity = max(dot(normalize(light.pos-FragPos),uNormal),0) * attenuation * spotRatio;
	vec3 diffColor = diffIntensity * light.color * texture(material.diffuse,TexCoord).rgb;
	//specular
	vec3 R = normalize(reflect(-normalize(light.pos-FragPos),uNormal));
	float specIntensity = pow(max(dot(R,dirToCamera),0),material.shininess) * attenuation * spotRatio;
	vec3 specColor = specIntensity * light.color * texture(material.specular,TexCoord).rgb;

	vec3 result = diffColor + specColor;
	return result;
}

void main(){
	
	vec3 finalResult = vec3(0,0,0);
	vec3 uNormal = normalize(Normal);
	vec3 dirToCamera = normalize(cameraPos - FragPos);
	finalResult += CalcLightDirectional(lightd,uNormal,dirToCamera);
	finalResult += CalcLightPoint(lightp0,uNormal,dirToCamera);
	finalResult += CalcLightPoint(lightp1,uNormal,dirToCamera);
	finalResult += CalcLightPoint(lightp2,uNormal,dirToCamera);
	finalResult += CalcLightPoint(lightp3,uNormal,dirToCamera);
	finalResult += CalcLightSpot(lights,uNormal,dirToCamera);

	FragColor = vec4(finalResult,1.0);
}