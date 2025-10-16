#include "../Common/Pixel/PixelShader2DHeader.hlsli"

// 定数バッファ：スロット4番目(b4と書く)
cbuffer cbParam : register(b4)
{
	float g_max_vine_pow;// ビネットの強度
    float g_player_hp;	//プレイヤーの体力
    float g_max_player_hp;	//プレイヤーの最大体力
    float g_time;   //時間
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // UV座標とテクスチャを参照して、最適な色を取得する
    float4 ret = tex.Sample(texSampler, PSInput.uv);
	float2 uv = PSInput.uv;
    //中心に近ければ0に近い
	float x = abs(uv.x - 0.5f);	// 0.0～1.0f → -0.5～0.5 → 0.0～0.5
	float y = abs(uv.y - 0.5f);	// 0.0～1.0f → -0.5～0.5 → 0.0～0.5
    //中心からの距離
    float dis = (x * x + y * y);
    //hpが高ければ０に近い
    float hpPer = (((g_max_player_hp - (g_player_hp < 0.0f?0.0f:g_player_hp)) / g_max_player_hp));
    
    float retValue = 0.25;
    //中心から近いところはそのまま
    if (dis < retValue)
    {
        return ret;
    }
    //0.2～0.5 ->　0.0～0.5
    dis -= retValue;
    dis /= (retValue / 0.5f);
    //時間をsinで波上にする
    float speed;
    if (g_player_hp < 0.0f)
    {
        speed = 2.0f;
    }
    else
    {
        speed = 1.0f;
    }
    float time = abs(sin(g_time * speed)); //0-1
    time /= 2.0f;   //0 - 0.5
    time += 0.5f;   //0.5 - 1.0f
    
		// ビネットの強度を計算
    float vignette = 1.0f - saturate(dis * g_max_vine_pow * hpPer * time);
	if(g_player_hp < 0.0f)
    {
        vignette = 1.0f - saturate(dis * g_max_vine_pow * time);
    }
    //if (vignette > 0.7f)
    //{
    //    return ret;
    //}
    //ヴィネットの強さを0.2～1.0に補正する
    vignette = vignette * 0.8f + 0.2f;



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