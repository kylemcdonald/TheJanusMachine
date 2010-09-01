varying vec3 normal;

void main() {
	gl_FragColor =
		vec4(
			vec3(
				(1. - sin((normal.z - (3.14159/2.)) * 3.141)) * .1) +
				sin(normal.y * 100.) * .01 +
				sin((normal.x + normal.y) * 10.) * .004,
			1.);
}
