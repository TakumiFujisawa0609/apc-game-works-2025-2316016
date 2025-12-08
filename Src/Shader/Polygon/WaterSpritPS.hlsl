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
    float2 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    uv.y -= g_time * time_scale;
    uv.x -= g_time;
    uv = frac(uv);
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if(color.r < 0.1f)
    {
        discard;
    }
    float4 ret = float4(0.0f,0.5f,1.0f,color.r *color.r);
    ret *= color;
    return ret;
}