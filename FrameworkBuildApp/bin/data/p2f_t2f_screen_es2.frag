precision highp float;

uniform sampler2D u_texture;

uniform float u_alpha;

varying vec2 v_texcoord;

void main() { 

	gl_FragColor = texture2D(u_texture, v_texcoord);
	gl_FragColor.a *= u_alpha;
}