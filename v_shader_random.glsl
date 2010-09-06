varying vec3 vertex;
varying vec3 normal;
varying vec4 v_colour;

float RAND_CONST = 0.0;

//random float in [-1, 1]
float random(int x) {
    return ( 1.0 - float(( (x * (x * x * 15731 + 789221) + 1376312589))) / 1073741824.0); 
}

float frandom(float x) {
    return fract(sin(x * 12.9898)) * 43758.5453;
}

float vec3random(vec3 v) {
    return fract(sin(v.x * 12.9898 + v.y*5.84846 + v.z*3.4693)) * 43758.5453;
}

float vec4random(vec4 v) {
    return fract(sin(v.x * 12.9898 + v.y*5.84846 + v.z*3.4693 + v.w*4.693219)) * 63758.5459;
}


void main() {

	vec4 v = gl_Vertex;
    v.x += RAND_CONST*frandom(gl_Vertex.x);
	v.y += RAND_CONST*frandom(gl_Vertex.y*0x45654565);
	v.z += RAND_CONST*vec4random(gl_Vertex*0x45654565);
	
     gl_Position = gl_ModelViewProjectionMatrix * v;
	 vertex = vec3(gl_ModelViewMatrix * v);
     normal = normalize(gl_NormalMatrix * gl_Normal);
     v_colour = gl_Color;
}
