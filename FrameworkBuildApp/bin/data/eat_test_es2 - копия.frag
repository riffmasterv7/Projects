precision highp float;

uniform highp vec3 u_LPR_from;
uniform highp vec3 u_LPR_to;

varying highp vec3 v_LPR;

void main() { 

	vec4 color = vec4(0.0, 1.0, 0.0, 1.0);
	
	if (v_LPR.x < u_LPR_from.x) {
		discard;
	} else if (v_LPR.x == u_LPR_from.x) {
	
		if (v_LPR.y < u_LPR_from.y) {
			discard;
		} else if (v_LPR.y == u_LPR_from.y 
				&& v_LPR.z < u_LPR_from.z) {
			discard;
		}
	}
	
	if (v_LPR.x > u_LPR_to.x) {
		discard;
	} else if (v_LPR.x == u_LPR_to.x) {
	
		if (v_LPR.y > u_LPR_to.y) {
			discard;
		} else if (v_LPR.y == u_LPR_to.y 
			    && v_LPR.z > u_LPR_to.z) {
			discard;
		}
	}
	
	/*
	if (v_LPR.z <= u_LPR_from.z) {
	
		if (v_LPR.y <= u_LPR_from.y) {
		
			if (v_LPR.x <= u_LPR_from.x) {
				color = vec4(1.0, 0.0, 0.0, 1.0);
			}
		}
	}
	
	if (v_LPR.z >= u_LPR_to.z) {
	
		if (v_LPR.y >= u_LPR_to.y) {
		
			if (v_LPR.x >= u_LPR_to.x) {
				color = vec4(0.0, 0.0, 1.0, 1.0);
			}
		}
	}
	*/
	
	/*
	if (v_LPR.z <= u_LPR_from.z 
	 && v_LPR.y <= u_LPR_from.y 
	 && v_LPR.x <= u_LPR_from.x) {
		color = vec4(1.0, 1.0, 0.0, 1.0);
	}
	
	if (v_LPR.z >= u_LPR_to.z 
	 && v_LPR.y >= u_LPR_to.y 
	 && v_LPR.x >= u_LPR_to.x) {
		color = vec4(0.0, 1.0, 0.0, 1.0);
	}
	*/
	
	//color.a = length(u_LPR_from) + length(u_LPR_to) + 1.0;

	gl_FragColor = color;
}