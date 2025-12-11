// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

//Texture2D Noise : register(t3);

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
    //uv = frac(uv);
    //return float4(uv, 0.0f, 1.0f);
    uv.y -= g_time * time_scale;
    //uv.x -= g_time;
    uv = frac(uv);
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    //return color;
    float4 ret = float4(1.0f, 1.0f, 0.1f, color.a);
    if (abs(PSInput.worldPos.y) < threshold)
    {
        return float4(1.0f, 1.0f, 0.1f, 1.0f);
    }
    if(color.a < 0.1f)
    {
        discard;
    }
    if(color.a < 0.3f)
    {
        ret = float4(0.0f, 0.1f, 0.0f, color.a);
    }
    return ret;
}