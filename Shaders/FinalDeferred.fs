#version 410 

uniform sampler2D Specular ;
uniform sampler2D Diffuse1 ;
uniform sampler2D Diffuse2;
uniform sampler2D Shadow ;
uniform sampler2D LightDiffuse ;
uniform sampler2D LightSpecular ;
uniform sampler2D MaterialID ;
uniform sampler2D Transparency ;
uniform int noLight ;

in vec2 texcoord0 ;


void main(){
	
	vec3 FinalColor=vec3(0.0);
	vec3 DiffuseColor1=texture2D(Diffuse1,texcoord0).rgb;
	vec3 DiffuseColor2=texture2D(Diffuse2,texcoord0).rgb;
	vec3 DiffuseColor=mix(DiffuseColor1,DiffuseColor2,texture2D(Transparency,texcoord0).rgb);
	vec3 SpecularColor=vec3(0.0) ;
	vec3 shadowcolor=vec3(1.0);
	if(noLight==0){
		DiffuseColor*=texture2D(LightDiffuse,texcoord0).rgb ;
		SpecularColor=texture2D(Specular,texcoord0).rgb*texture2D(LightSpecular,texcoord0).rgb ;
		shadowcolor=texture2D(Shadow,texcoord0).rgb;
	}
	gl_FragColor=vec4((DiffuseColor+SpecularColor)*shadowcolor ,1.0);
}
