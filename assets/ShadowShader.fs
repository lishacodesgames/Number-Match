#version 330

// Input texture coordinates from the vertex shader (0→1 range)
in vec2 fragTexCoord;

// Final color output to the screen
out vec4 finalColor;

// Uniforms sent from C++
uniform vec2 size;      	// width and height of the rectangle
uniform vec2 offset;  		// x/y offset of the shadow
uniform float softness;    // blur radius
uniform vec4 color;   		// RGBA shadow color

// Signed distance to rectangle
float rectSDF(vec2 p, vec2 halfSize) {
	vec2 d = abs(p) - halfSize;
	return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

void main() {
	// Convert texture coordinates (0→1) to centered coordinates (-0.5→0.5)
	vec2 p = fragTexCoord - 0.5;
	
	// Convert to pixel space using rectangle size	
	p *= size + softness * 2.0;
	
	// Apply shadow offset	
	p -= offset;
	
	// Compute distance from rectangle edge	
	float dist = rectSDF(p, size * 0.5);
	
	// Smooth blur transition	
	float alpha = 1.0 - smoothstep(0.0, softness, dist);
	
	// Final shadow color
	finalColor = vec4(color.rgb, color.a * alpha);
}