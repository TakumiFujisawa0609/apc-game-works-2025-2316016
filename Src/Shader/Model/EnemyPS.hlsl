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
    float4 damage_add_color;    //ダメージ時の加算値
    float damage_time;  //ダメージエフェクト時間
    float3 hitPos;  //ダメージを受けた場所
    float damage_radius;    //ダメージの半径
    float3 dummy;
    //float g_time;
    //float3 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
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