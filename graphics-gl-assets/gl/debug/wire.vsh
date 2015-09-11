attribute highp vec3 position;
attribute lowp vec4 color;

// uniforms
uniform highp mat4 lookProjection;

// varyings
varying mediump vec4 genColor;
// main
void main() {
   gl_Position = lookProjection * vec4(position.x, position.y, position.z, 1.0);
   gl_PointSize = 16.0;
   genColor = color;
}
