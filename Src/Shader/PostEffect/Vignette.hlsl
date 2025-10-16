#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float g_max_vine_pow;// ビネットの強度
    float g_player_hp;	//プレイヤーの体力
    float g_max_player_hp;	//プレイヤーの最大体力
    float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

	float2 uv = PSInput.uv;
	float x = abs(uv.x - 0.5f);	// 0.0～1.0f → -0.5～0.5 → 0.0～0.5
	float y = abs(uv.y - 0.5f);	// 0.0～1.0f → -0.5～0.5 → 0.0～0.5
	float dis = (x * x + y * y);
    float hpPer = ( ((g_max_player_hp - g_player_hp) / g_max_player_hp));
		// ビネットの強度を計算
    float vignette = 1.0f - saturate(dis * g_max_vine_pow * hpPer);
	if(g_player_hp < 0.0f)
    {
        vignette = 1.0f - saturate(dis * g_max_vine_pow);
    }

    //ヴィネットの強さを0.2～1.0に補正する
    vignette = vignette * 0.8f + 0.2f;
    
	// UV座標とテクスチャを参照して、最適な色を取得する
	float4 ret = tex.Sample(texSampler, PSInput.uv);
    if(dis < 0.35f)
    {
        return ret;
    }
    if (g_player_hp < 0.0f)
    {
        ret.gb *= vignette;
    }
    else
    {
        ret.rgb *= vignette;
    }    
	return ret;

}