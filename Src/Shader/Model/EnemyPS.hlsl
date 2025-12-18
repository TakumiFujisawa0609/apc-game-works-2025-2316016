// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D Noise : register(t3);
SamplerState noiseSampler : register(s3); //サンプラー

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float4 color_times; //色の倍率
    float4 damage_add_color;    //ダメージ時の加算値
    float damage_time;  //ダメージエフェクト時間
    float3 hitPos;  //ダメージを受けた場所
    float damage_radius;    //ダメージの半径
    float3 dummy;
    float disolve_rate;
    float3 dummy2;
    //float g_time;
    //float3 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float4 noiseCol = Noise.Sample(noiseSampler, uv);
    if(disolve_rate > noiseCol.r)
    {
        discard;
    }
    if (noiseCol.r - disolve_rate < 0.01f)
    {
        return float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    color *= color_times;
    if(damage_time < 0.0f)
    {
        return color;
    }
    float3 worldPos = PSInput.worldPos;
    float dis = distance(worldPos, hitPos);
    if(dis > damage_radius)
    {
        return color;
    }
    return color + damage_add_color;
}