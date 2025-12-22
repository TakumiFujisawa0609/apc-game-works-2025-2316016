// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_time;
    float3 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //uv値を取得する
    float2 uv = PSInput.uv;
    //y方向を時間でスクロールさせる
    uv.y = frac(uv.y + g_time);
    //ノイズテクスチャからノイズの色を取得
    float4 col = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    //赤色の半透明にする
    col.r = 1.0f;
    col.a = 0.5f;
    return col;
}