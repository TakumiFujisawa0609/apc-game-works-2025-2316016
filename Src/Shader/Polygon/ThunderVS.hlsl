// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#include "../Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_VERTEX3DSHADER
// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Common/Vertex/VertexShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b7)
{
    float2 g_uv_scale;
    float radius;
    float dummy;
    //float3 center_pos;
    //float g_random;
}

VS_OUTPUT main(VS_INPUT VSInput)
{
VS_OUTPUT ret;
// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 開始 )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;
// float3 → float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;
// ローカル座標をワールド座標に変換(剛体)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    //float3 dir = float3(sin(random), 0.0f, cos(random));
    //float randomScale = lWorldPosition.y;
    //if(randomScale < 0.0f)
    //{
    //    randomScale = 0.0f;
    //}
    //float random = g_random * lWorldPosition.y;
    //float3 dir = center_pos - lWorldPosition.xyz;
    //dir.y = 0.0f;
    //dir = normalize(dir);
    //lWorldPosition.xyz = lWorldPosition.xyz + dir * (random % radius);
    //lWorldPosition.y += sin(lWorldPosition.x + time) * waveScale;
    ret.worldPos = lWorldPosition.xyz; // ワールド座標をピクセルシェーダへ引き継ぐ
// ワールド座標をビュー座標に変換
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;
// ビュー座標を射影座標に変換
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);
// 頂点座標変換 +++++++++++++++++++++++++++++++++++++( 終了 )
    
    // その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 開始 )
// UV座標
    ret.uv.x = VSInput.TexCoords0.x;
    ret.uv.y = VSInput.TexCoords0.y;
    ret.uv.x = VSInput.TexCoords0.x * g_uv_scale.x;
    ret.uv.y = VSInput.TexCoords0.y * g_uv_scale.y;
    //ret.uv.y += sin(ret.uv.x + time) * waveScale;
// 法線
    ret.normal = VSInput.Normal; // 法線をローカル空間からワールド空間へ変換
    ret.normal = normalize(mul(VSInput.Normal, (float3x3) g_base.localWorldMatrix));

// ディフューズカラー
    ret.diffuse = VSInput.diffuse;
// ライト方向(ローカル)
    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
// ライトから見た座標
    ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
// その他、ピクセルシェーダへ引継&初期化 ++++++++++++( 終了 )
// 出力パラメータを返す
    return ret;
}