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
    float4 color_times; //色の倍率
    float3 camera_pos;
    float rim_power;
    float4 rimlight_color;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    color *= color_times;
     
    float3 toCamera = normalize(camera_pos - PSInput.worldPos);
    float cDot = abs(dot(PSInput.normal, toCamera));
    cDot = 1.0f - cDot;
    float rimDot = pow(cDot, rim_power); // 強弱を強め
    float4 rimColor = rimlight_color * rimDot; // 緑リムライト
    // 色の合成
    float3 rgb = (color.rgb * color_times.rgb) + rimColor.rgb;
    return float4(rgb, color.a);
}