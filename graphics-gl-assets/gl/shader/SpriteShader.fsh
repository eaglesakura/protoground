#version 100 /* GLES 2.0 */

// UV setting
varying mediump vec2 fTexCoord;
// texture
uniform sampler2D tex;
// color
uniform lowp vec4    blendColor;
uniform lowp float	fillMode;

void main() {
    if(int(fillMode) == 1) {
    	gl_FragColor = blendColor;
    } else if(int(fillMode) == 2) {
        gl_FragColor = texture2D(tex, fTexCoord).rrrr * blendColor;
    } else {
        gl_FragColor = texture2D(tex, fTexCoord) * blendColor;
    }
}