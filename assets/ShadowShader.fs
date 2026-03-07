#version 330

// Input texture coordinates from the vertex shader (0→1 range)
in vec2 fragTexCoord;

// Final color output to the screen
out vec4 finalColor;

// Uniforms sent from C++
uniform vec2 size;      	// width and height of the rectangle
uniform float softness;    // blur radius
uniform vec4 color;   		// RGBA shadow color

// Signed distance to rectangle
float rectSDF(vec2 p, vec2 halfSize) {
	vec2 d = abs(p) - halfSize;
	return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

void main() {
	// Calculate the TOTAL size of the drawing area (Button + Padding for blur)
	vec2 canvasSize = size + (softness * 2.0);

	// Convert fragTexCoord (0->1) into actual pixel coordinates (0 -> canvasSize)
	vec2 pixelPos = fragTexCoord * canvasSize;

	// Center the coordinates so (0,0) is the middle of the canvas
	vec2 p = pixelPos - (canvasSize * 0.5);
	
	// Compute distance from rectangle edge	
	float dist = rectSDF(p, size * 0.5);
	
	// Smooth blur transition	
	float alpha = 1.0 - smoothstep(-softness, softness, dist);
	
	// Final shadow color
	finalColor = vec4(color.rgb, color.a * alpha);
}