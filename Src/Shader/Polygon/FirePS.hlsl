// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"
// IN
#define PS_INPUT VertexToPixelLit
//PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

Texture2D Noise : register(t1);
SamplerState noiseSampler : register(s1); //サンプラー

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
    float g_time;
    float noise_pow;
    float2 dummy;
}


float4 main(PS_INPUT PSInput) : SV_TARGET
{
    //uv値を取得する
    float2 uv = PSInput.uv;
    //uv値をもとにディフーズテクスチャから色を取得する
    float4 color = diffuseMapTexture.Sample(diffuseMapSampler, uv);
    if (color.r < 0.1f)
    {
        //グレースケール画像で黒に近いところは描画しない
        discard;
    }
    //ノイズ用uv値を設定(y値を動かすことによって炎が動いているように見える)
    uv.y = frac(uv.y + g_time) * noise_pow;
    //ノイズテクスチャからノイズの色を取得
    float4 noiseCol = Noise.Sample(noiseSampler, uv);
    //炎の色だから赤・橙・黄色になるようにする
    color = float4(
        1.0f,
        (frac(sin(g_time) + color.r + noiseCol.r)/* * uv.y*/),
        0.0f,
        color.r * noiseCol.r);

    return color;
}