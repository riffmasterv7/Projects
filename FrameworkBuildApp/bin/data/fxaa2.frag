
precision mediump float;

uniform sampler2D u_texture;

varying vec2 v_texcoord00;

void main()
{
	vec3 color = texture2D(u_texture, v_texcoord00).xyz;
	
	gl_FragColor = vec4(color, 1.0);
}
