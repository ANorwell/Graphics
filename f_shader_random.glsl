varying vec3 vertex;
varying vec3 normal;
varying vec4 v_colour;

float RAND_CONST = 0.15;

//random float in [-1, 1]
float random(int x) {
    return ( 1.0 - float(( (x * (x * x * 15731 + 789221) + 1376312589))) / 1073741824.0); 
}

float frandom(float x) {
    return 1.0 - 2.0*fract(sin(x * 12.9898) * 43758.5453);
}

float vec3random(vec3 v) {
    return 1.0 - 2.0*fract(sin(v.x * 12.9898 + v.y*5.84846 + v.z*3.4693)) * 43758.5453;
}

float vec4random(vec4 v) {
    return 1.0 - 2.0*fract(sin(v.x * 12.9898 + v.y*5.84846 + v.z*3.4693 + v.w*4.693219)) * 63758.5459;
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
	gl_FragColor.r  += RAND_CONST*frandom(colour.r);
	gl_FragColor.g  += RAND_CONST*frandom(colour.g);
	gl_FragColor.b  += RAND_CONST*frandom(colour.b);
}
