#version 100
/* GLES 2.0 */

/**
 * ( poly_x | poly_y )はそれぞれ0.0f〜1.0fの正規化座標で設定する。
 * poly_yについては2D座標系を基本として、画面左上が(0, 0)、画面右下が(1, 1)として扱う。
 */
// ポリゴンのXYWH
uniform mediump vec4    poly_data;
uniform mediump float   aspect;
// ポリゴンの回転角度
uniform mediump float   rotate;
// ポリゴンのUV情報
uniform mediump vec4    poly_uv;
// カラーポリゴンのみで描画する場合はtrue
uniform lowp float	colorOnly;

// アクセス用のショートカットを設定する
#define poly_x         poly_data.x
#define poly_y         poly_data.y
#define poly_width     poly_data.z
#define poly_height    poly_data.w
//
#define poly_uv_u      poly_uv.x
#define poly_uv_v      poly_uv.y
#define poly_uv_w      poly_uv.z
#define poly_uv_h      poly_uv.w

//
attribute mediump vec2 vTexCoord;
attribute mediump vec2 vPosition;

varying mediump vec2 fTexCoord;

void main() {
	mediump mat4 mat = mat4(1.0);
	mediump mat4 temp = mat4(1.0);

   // 各種行列指定
   {
       // 移動行列を作成する
       {
           mat[3][0] = poly_x;
           mat[3][1] = poly_y;
       }
       // アスペクト比補正をかける
       {
           temp[0][0] = 1.0 / aspect;
           mat = mat * temp;
       }
       // 回転行列を作成する
       {
           temp = mat4(1.0);
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
       gl_Position = mat * vec4(vPosition, 0.0, 1.0);
   }
   
   // テクスチャ操作
   if(colorOnly == 0.0) {
       mat = mat4(1.0);
       {
		// スケーリング行列を作成する
           mat[0][0] = poly_uv_w;
           mat[1][1] = poly_uv_h;
		// 移動行列を作成する
           mat[3][0] = poly_uv_u;
           mat[3][1] = poly_uv_v;
       }
       fTexCoord.xy = (mat * vec4(vTexCoord, 0.0, 1.0)).xy;
   }
}