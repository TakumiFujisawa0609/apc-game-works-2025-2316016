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
    float g_time;
    float time_scale;
    float2 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //uv座標取得
    float2 uv = PSInput.uv;
    //uvをもとにテクスチャカラー取得
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if(color.r < 0.1f)
    {
        //黒に近い色を消す
        discard;
    }
    //ノイズテクスチャのuv座標を時間で動かす
    uv.y = frac(uv.y + g_time * time_scale);
    //ノイズテクスチャの色取得
    float4 noiseCol = Noise.Sample(noiseSampler, uv);
    //紫にしたいからRとBを1.0fに固定、Gをノイズと時間で変化させる
    color = float4(
        1.0f,  
        frac(sin(g_time) + color.r + noiseCol.r),
        1.0f,
        color.r * noiseCol.r );
    return color;
}