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
    float2 uv = PSInput.uv;
    //uv.x = frac(uv.x + (sin(g_time)) * (1.0f - (uv.y * uv.y)));
    //uv.x = frac(uv.x + ((g_time * 0.1f)));
    
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if(color.r < 0.1f)
    {
        discard;
    }
    color = float4(1.0f, uv.y, 1.0f, color.r);

    return color;
}