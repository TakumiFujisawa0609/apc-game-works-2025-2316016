// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D Noise : register(t1);
SamplerState noiseSampler : register(s1); //サンプラー

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 g_color;
    float time;
    float time_scale;
    float2 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //uv座標取得
    float2 uv = PSInput.uv;
    //uvをもとにテクスチャカラー取得
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if (color.r < 0.1f)
    {
        //黒に近い色を消す
        discard;
    }
    //ノイズテクスチャのuv座標を時間で動かす
    uv.y = frac(uv.y + time * time_scale);
    //ノイズテクスチャの色取得
    float4 noiseCol = Noise.Sample(noiseSampler, uv);
    color = float4(
        g_color.r,
        saturate(frac(sin(time) + color.g + noiseCol.g) * g_color.g),
        g_color.b,
        g_color.a * color.r);
    //color *= g_color;
    return color;
}