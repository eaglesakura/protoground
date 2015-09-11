#version 100 /* GLES 2.0 */
#extension GL_OES_EGL_image_external : require

precision mediump float;
// UV setting
varying vec2 fTexCoord;
// texture
uniform samplerExternalOES tex;
// color
uniform mediump vec4    blendColor;
void main() {
    gl_FragColor = texture2D(tex, fTexCoord) * blendColor;
}