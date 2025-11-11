// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D Noise : register(t3);

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_time;
    float time_scale;
    float threshold;
    float dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    uv.y -= g_time * time_scale;
    uv.x -= g_time;
    uv.y = frac(uv.y);
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float4 ret = float4(1.0f, 1.0f, 0.1f, color.a);
    if (abs(PSInput.worldPos.y)  < threshold)
    {
        ret = float4(1.0f, 1.0f, 0.1f, 1.0f);
    }
    return ret;
}