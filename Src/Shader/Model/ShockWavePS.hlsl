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
    //uv.y = saturate(uv.y + abs(sin(g_time) / 2.0f + uv.x) * ( /*1.0f - */(uv.y * uv.y)) /* + 0.3f*/);
    //uv.y = saturate( uv.y - abs(sin(g_time) )/* * (1.0f - (uv.y * uv.y)) + 0.3f*/);
    //uv.y = 1.0 - uv.y;
    //uv.y = 1.0 - (uv.y / 2);
    //uv.x = frac(uv.x * uv.y + ((g_time)));
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if(color.r < 0.1f)
    {
        discard;
    }
    //if (PSInput.worldPos.y < 0.001f)
    //{
    //    return float4(1.0f, 0, 1.0f, 1.0f);
    //}
    uv.y = frac(uv.y + g_time) * 4;
    uv.x *= 5.0f;
    float4 noiseCol = Noise.Sample(diffuseMapSampler, uv);
    color = float4(1.0f, frac(sin(g_time) + color.r + noiseCol.r/* * PSInput.worldPos.x*/), 1.0f, color.r * noiseCol.r );

    return color;
}