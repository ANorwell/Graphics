varying vec3 vertex;
varying vec3 normal;
varying vec4 v_colour;

float RAND_CONST = 0.0;

//random float in [-1, 1]
float random(int x) {
    //	x =  (x<<13) ^ x;
    return ( 1.0 - float(( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff)) / 1073741824.0); 
}

void main() {


	vec4 v = gl_Vertex;
	v.x += RAND_CONST*random(int(gl_Vertex.x*0x45654565));
	v.y += RAND_CONST*random(int(gl_Vertex.y*0x45654565));
	v.z += RAND_CONST*random(int((gl_Vertex.z + 2.0*gl_Vertex.y)*0x45654565));
	//v.w += RAND_CONST*random(int(gl_Vertex.z*0x45654556));
	
	
     gl_Position = gl_ModelViewProjectionMatrix * v;
	 vertex = vec3(gl_ModelViewMatrix * v);
     normal = normalize(gl_NormalMatrix * gl_Normal);
     v_colour = gl_Color;
}
