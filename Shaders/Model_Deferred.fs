#version 430

uniform sampler2D DiffuseMap ;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap ;
uniform sampler2D RoughnessMap ;
uniform sampler2D EmissiveMap ;
uniform float IOR ;
uniform float Opacity ;
uniform int NotAffectedByLight;

in vec4 PositionColor ;
in  mat3 TBN ;
in vec2 texcoord0;
in vec3 normal ;
void main(){
	
	
	vec3 normalcolor0 ;

	normalcolor0 = texture2D(NormalMap, texcoord0).bgr;
	normalcolor0 = normalize(normalcolor0 * 2.0 - 1.0);   
	normalcolor0 = normalize(TBN * normalcolor0);
	
	gl_FragData[0]=	texture2D(DiffuseMap,texcoord0) ;// Diffuse 1 (Solid Colors)	
	gl_FragData[1]= vec4(texture2D(SpecularMap,texcoord0).bgr,1.0);	// Specular 
	gl_FragData[2]= vec4(normal,1.0);	// world normal
	if(NotAffectedByLight)
		gl_FragData[3]= vec4(0.0,texture2D(RoughnessMap,texcoord0).r,0.0,1.0); // Material ID
	else
		gl_FragData[3]= vec4(1.0,0.0,0.0,1.0); // Material ID
		
	gl_FragData[4]= vec4(PositionColor.rgb,1.0);// World Postion
	gl_FragData[5]= vec4(texture2D(DiffuseMap,texcoord0).a+Opacity ,0.0,0.0,1.0); // Transprency
	gl_FragFata[6]= vec4(texture2D(EmissiveMap,texcoord0).rgb,1.0);

}