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
    float3 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    //uv.y = saturate( /*uv.y +*/abs(sin(g_time)) * (1.0f - (uv.y * uv.y)) + 0.3f);
    //uv.y = saturate( uv.y - abs(sin(g_time) )/* * (1.0f - (uv.y * uv.y)) + 0.3f*/);
    uv.y = uv.y / 2;
    //uv.x = frac(uv.x * uv.y + ((g_time * 0.1f)));
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if(color.r < 0.1f)
    {
        discard;
    }
    float4 noiseCol = Noise.Sample(diffuseMapSampler, PSInput.uv);
    color = float4(1.0f, frac(sin(g_time) + noiseCol.r/* * PSInput.worldPos.x*/), 1.0f, color.r);

    return color;
}