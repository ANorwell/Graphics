varying vec3 vertex;
varying vec3 normal;
varying vec4 v_colour;

float RAND_CONST = 0.15;

float random(int x) {
	x =  (x<<13) ^ x;
    return ( 1.0 - float(( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff)) / 1073741824.0); 
} 

//phong lighting (per pixel)
void main() {
	
	vec4 cr = gl_FrontMaterial.diffuse;
	vec4 cs = gl_FrontMaterial.specular;
	float p = gl_FrontMaterial.shininess;

	//ambient contribution
	vec4 colour = gl_LightModel.ambient * v_colour;

	for (int i = 0; i < gl_MaxLights; i++) {
		vec3 l = normalize(gl_LightSource[i].position.xyz - vertex);   //light vector
		vec3 hh = normalize(l) - normalize(vertex);
		vec3 h =  normalize(hh);             //half vector
		
		//diffuse
		colour = colour + cr * gl_LightSource[i].diffuse * max(0, dot(normal,l));		
		
		//specular
		if (dot(h,normal) > 0.0f)
			colour = colour + cs * gl_LightSource[i].specular * pow(dot(h,normal), p);
	}
	
	gl_FragColor = colour;
	gl_FragColor.r  += RAND_CONST*random(int(colour.r*0x3425556));
	gl_FragColor.g  += RAND_CONST*random(int(colour.g*0x3425556));
	gl_FragColor.b  += RAND_CONST*random(int(colour.b*0x3425556));
}
