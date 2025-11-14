#include "../Common/Pixel/PixelShader2DHeader.hlsli"

Texture2D ui : register(t1); //テクスチャ
SamplerState uiSampler : register(s1); //サンプラー
Texture2D mask : register(t2); //テクスチャ
SamplerState maskSampler : register(s2); //サンプラー


// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float rate;     //体力割合
    float3 color;   //ゲージの色
    float min;
    float max;
    float2 dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float4 col = ui.Sample(uiSampler, uv);
    if(col.a < 0.1f)
    {
        discard;
    }
    float dif = max - min;
    float size = dif * rate;
    size += min;
    float4 maskCol = mask.Sample(maskSampler, uv);
    if(maskCol.r < 0.9f || size < uv.x)
    {
        return col;
    }
    return float4(color, 1.0f);
}