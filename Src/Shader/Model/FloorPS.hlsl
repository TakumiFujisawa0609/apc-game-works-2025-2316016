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
    float4 light_color;
    float3 light_pos;
    float light_radius;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);

    float3 worldPos = PSInput.worldPos;
    float dis = distance(worldPos, light_pos);
    if(dis > light_radius)
    {
        return color;
    }
    float3 lightcol = light_color.rgb * (1.0f - (dis / light_radius));
    float4 ret = color + float4(lightcol, 1.0f);
    return ret;
}