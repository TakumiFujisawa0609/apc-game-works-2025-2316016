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
    float3 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    //uv.y += g_time;
    uv.x = uv.x + (sin(g_time));
    uv.y = uv.y + (cos(g_time));
    uv.y += g_time * 2;
    uv = frac(uv);
    //return float4(uv.x, uv.y, 1.0f, 1.0f);
    float4 noiseCol = Noise.Sample(noiseSampler, uv);
    //if(noiseCol.r < 0.1f)
    //{
    //    discard;
    //}
    noiseCol.a = noiseCol.r;
    noiseCol.rgb = 1.0f;
    //noiseCol.gb = 0.0f;
    //noiseCol.a = 0.0f;
    //return float4(noiseCol.xyz, 0.0f);
    return noiseCol;
}