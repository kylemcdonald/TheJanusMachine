varying vec3 normal;
uniform float alpha;
uniform float redScale;
uniform float greenScale;
uniform float blueScale;

void main() {

	vec3 vec = vec3(
				(1. - sin((normal.z - (3.14159/2.)) * 3.141)) * .1);
				
				vec.x *= redScale;
				vec.y *= greenScale;
				vec.z *= blueScale;
				
				
	gl_FragColor =
		vec4(vec +
				sin(normal.y * 100.) * .01 +
				sin((normal.x + normal.y) * 10.) * .004,
			alpha);
}
