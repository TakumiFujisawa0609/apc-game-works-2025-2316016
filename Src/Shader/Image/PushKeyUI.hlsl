#include "../Common/Pixel/PixelShader2DHeader.hlsli"

Texture2D Noise : register(t1);
SamplerState noiseSampler : register(s1); //サンプラー

Texture2D OriginTex : register(t2); //元のテクスチャ
SamplerState OriginSampler : register(s2); //サンプラー

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float3 color; //色
    float effectType; //エフェクトタイプ
    float time; //時間
    float power;
    float2 size;
    float3 outlineColor;
    float dummy;
}

//bool IsOutLine(float2 uv)
//{
//    bool ret = false;
//    float2 pixUV = float2(1.0f / size.x, 1.0f / size.y);
//    for (int y = -1; y <= 1; y += 1)
//    {
//        for (int x = -1; x <= 1; x+=1)
//        {
//            float2 offsetUV = uv + float2(pixUV.x * x, pixUV.y * y);
//            float4 sampleCol = tex.Sample(texSampler, offsetUV);
//            if (sampleCol.a > 0.1f)
//            {
//                ret = true;
//            }
//        }
//    }
//    return ret;
//}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //return float4(outlineColor,1.0f);
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
    float2 uv = PSInput.uv;
    float2 suv = PSInput.suv;
    float4 col =tex.Sample(texSampler, uv);
    float4 retCol = float4(0.0f,0.0f,0.0f,0.0f);
    if(col.a < 0.1f)
    {
        //if (IsOutLine(uv))
        //{
        //    retCol = float4(outlineColor, 1.0f);
        //}
        //else
        {
            discard;
        }
    }
    if(effectType == 0.0f)
    {
        //uv.x += (time * power);
        uv = frac(uv);
        retCol = float4(0.5f +uv.x / 2,0.5f + uv.y / 2, 0.0f, 1.0f);
    }
    else if(effectType == 1.0f)
    {
        float4 noiseCol = Noise.Sample(noiseSampler, uv);
        retCol = col * noiseCol;
    }
    //retCol.a = abs(sin(time * power));
    float4 originCol = OriginTex.Sample(OriginSampler, suv);
    float percent = abs(sin(time * power));
    retCol.rgb = retCol.rgb * percent + originCol.rgb * (1.0f - percent);
    return retCol;
    //return float4(color, 1.0f);
}