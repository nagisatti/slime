/*==============================================================================

   2D描画用頂点シェーダー [shader_vertex_2d.hlsl]
--------------------------------------------------------------------------------

==============================================================================*/

// 定数バッファ
cbuffer Buffer1 : register(b0)
{
    float4x4 mtx; //C言語から渡されたデータが入っている
}
cbuffer Buffer2 : register(b1)
{
    float4x4 World; //C言語から渡されたデータが入っている
}
struct LIGHT
{
    bool Enable;
    bool3 dummy;
    float4 Direction;
    float4 Diffuse;
    float4 Ambient;
};
cbuffer Buffer3 : register(b2)
{
    LIGHT Light;
}


//入力用頂点構造体
struct VS_INPUT
{ //              V コロン！
    float4 posL : POSITION0; //頂点座標 オーでなくゼロ！
    float4 normal : NORMAL0; //法線
    float4 color : COLOR0; //頂点カラー（R,G,B,A）
    float2 texcoord : TEXCOORD0;
};

//出力用頂点構造体
struct VS_OUTPUT
{
    float4 posH : SV_POSITION; //変換済頂点座標
    float4 color : COLOR0; //頂点カラー
    float2 texcoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT vs_in)
{
    VS_OUTPUT vs_out; //出力用構造体変数
    
    //頂点を行列で変換
    vs_out.posH = mul(vs_in.posL, mtx);
    //頂点カラーはそのまま出力
    vs_out.color = vs_in.color;
    //テクスチャ座標を出力
    vs_out.texcoord = vs_in.texcoord;
    //------------------------------

    //ライティング
    if (Light.Enable == true)
    {
        float4 normal = vs_in.normal;
        normal.w = 0.0f; //法線を初期化
        normal = mul(normal, World); //法線を回転
        normal = normalize(normal);
        float light = -dot(normal, Light.Direction); //明るさ計算
        light = saturate(light); //明るさ補正

        vs_out.color.rgb *= Light.Diffuse.rgb; //光の色を乗算
        vs_out.color.rgb *= light; //明るさを乗算
        vs_out.color.rgb += Light.Ambient.rgb; //アンビエント色加算
    }
 
    //結果を出力する
    return vs_out;
}



////=============================================================================
//// 頂点シェーダ
////=============================================================================
//float4 main(in float4 posL : POSITION0 ) : SV_POSITION
//{
//	return mul(posL, mtx);//頂点座標＊mtx（変換行列）
//}
