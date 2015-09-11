#version 100 /* GLES 2.0 */

/**
 * ( poly_x | poly_y )はそれぞれ0.0f〜1.0fの正規化座標で設定する。
 */
// ポリゴンのXYWH       
uniform mediump vec4    poly_data;
// ポリゴンの回転角度
uniform mediump float   aspect;
uniform mediump float   rotate;
// ポリゴンのUV情報
uniform mediump mat4    unif_texm;

// アクセス用のショートカットを設定する
#define poly_x         poly_data.x
#define poly_y         poly_data.y
#define poly_width     poly_data.z
#define poly_height    poly_data.w

//
attribute mediump vec4 vTexCoord;
attribute mediump vec4 vPosition;
varying mediump vec2 fTexCoord;

void main() {
   // 位置操作
   mediump mat4 mat = mat4(1.0);
   {
       mediump mat4 temp = mat4(1.0);

       // 移動行列を作成する
       {
           mat[3][0] = poly_x;
           mat[3][1] = poly_y;
       }
       // 回転行列を作成する
       {
           temp[0][0] = cos(rotate);
           temp[1][0] = sin(rotate);
           temp[0][1] = -temp[1][0];
           temp[1][1] = temp[0][0];
           mat = mat * temp;
       }
       // スケーリング行列を作成する
       {
           temp = mat4(1.0);
           temp[0][0] = poly_width * aspect;
           temp[1][1] = poly_height;
           mat = mat * temp;
       }
       {
           temp = mat4(1.0);
           temp[0][0] = 1.0 / aspect;
           mat = mat * temp;
       }
       gl_Position = mat * vPosition;
   }

   // テクスチャ操作
   {
       fTexCoord.xy = (unif_texm * vec4(vTexCoord.x, 1.0 - vTexCoord.y, 0.0, 1.0)).xy;
   }
}