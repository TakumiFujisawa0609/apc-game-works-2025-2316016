// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D Noise : register(t1);
SamplerState noiseSampler : register(s1); //サンプラー

// 定数バッファ：スロット4番目(b4と書く)
cbuffer Params : register(b4)
{
    float time; // 時間
    float3 camPos; // カメラ位置
};

// 疑似乱数
float rand(float2 p)
{
    return frac(sin(dot(p, float2(127.1, 311.7))) * 43758.5453);
}

// セルラーノイズ
float cellular(float2 uv)
{
    uv *= 8.0; // セルの細かさ（重要）

    float2 cell = floor(uv);
    float2 local = frac(uv);

    float minDist = 1.0;

    for (int y = -1; y <= 1; y++)
    {
        for (int x = -1; x <= 1; x++)
        {
            float2 neighbor = float2(x, y);
            float2 nc = cell + neighbor;

            float2 pointf =
                neighbor + float2(
                    rand(nc),
                    rand(nc + 10.0)
                );

            float d = length(local - pointf);
            minDist = min(minDist, d);
        }
    }

    return minDist;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.uv;
    uv = frac(uv * 4.0f);
    uv.x = frac(uv.x + time * 0.2f);
    float4 col = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    float3 toCamera = normalize(camPos - input.worldPos);
    float cDot = abs(dot(input.normal, toCamera));
    cDot = 1.0f - cDot;
    if(cDot < 0.5f)
    {
        if(col.r <0.3f)
        {
            //discard;
            return float4(0.5f, 0.5f, 0.5f, 0.5f);
        }
        return float4(0.7f,0.7f, 0.2f, 0.7f);
    }
    if(col.r < 0.5f)
    {
        discard;
    }

    // セルラーノイズ（少し動かす）
    //float n = cellular(uv + time * 0.1);
    // ディゾルブ（マスク）
    //clip(n - threshold);
    //if (frac(time / 2) < 0.2f)
    //{
    //    col.rgb = float3(1.0f, 1.0f, 0.3f);
    //    return col;
    //}
    //col = float4(1.0f, 1.0f, 0.3f, frac(time / 2));
    col.b *= 0.3f;
    col.rg = 1.0f;
    col.a = abs(sin(time / 2));
    //float4 col = float4(1.0, 1.0,n, 0.5);
    // 境界を少し光らせる（任意）
    //float edge = abs(n - threshold);
    //float linef = smoothstep(0.03, 0.0, edge);
    //col.rgb += linef;

    return col;
}
