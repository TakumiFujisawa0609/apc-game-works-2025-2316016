#include "../Common/Pixel/PixelShader2DHeader.hlsli"

Texture2D ui : register(t1); //テクスチャ
SamplerState uiSampler : register(s1); //サンプラー
Texture2D mask : register(t2); //テクスチャ
SamplerState maskSampler : register(s2); //サンプラー
Texture2D normal : register(t3); //テクスチャ
SamplerState normalSampler : register(s3); //サンプラー


// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float rate; //体力割合
    float3 color; //ゲージの色
    float g_min;
    float g_max;
    float2 dummy;
    float3 g_right_dir; //ライト方向
    float dummy1;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float2 uv = PSInput.uv;
    float4 col = ui.Sample(uiSampler, uv);
    if (col.a < 0.1f)
    {
        discard;
    }
    float dif = g_max - g_min;
    float size = dif * rate;
    size += g_min;
    float4 maskCol = mask.Sample(maskSampler, uv);
    float3 normalCol = normal.Sample(normalSampler, uv).xyz;
    normalCol = normalCol * 2.0f - 1.0f;
    normalCol.z = saturate(normalCol.z);
    normalCol = normalize(normalCol);
    float3 L = normalize(g_right_dir);
    float ndotl = saturate(dot(normalCol, L));
    //float cDot = (dot(normalize(normalCol.rgb), normalize(g_right_dir)));
    //cDot = 1.0f - cDot;
    float3 lightCol = float3(0.5f, 0.5f, 0.5f);
    float3 lighting = lightCol * ndotl;
    if (maskCol.r < 0.9f || size < uv.x)
    {
        float4 ret = float4(col.rgb + lighting, col.a);
        //float cmax = max(ret.r, max(ret.g, ret.b));
        //ret.rgb += (1.0f - cmax);
        return ret;
    }
    float4 ret = float4(color.rgb + lighting, col.a);
    //float cmax = max(ret.r, max(ret.g, ret.b));
    //ret.rgb += (1.0f - cmax);
    return ret;
    //return float4(color * col.rgb, 1.0f);
}