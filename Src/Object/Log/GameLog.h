#pragma once
#include <string>

class GameLog
{
public:

    struct Propaty
    {
        float time_ = 0.0f;            // 開始から終了までの時間
        int   rollAvoidNum_ = 0;        // 回転実行回数
        int   rollAvoidSaccessNum_ = 0; // 回転回避成功回数
        int   jumpAvoidNum_ = 0;        // ジャンプ実行回数
        int   jumpAvoidSaccessNum_ = 0; // ジャンプ回避成功回数
        float damage_ = 0.0f;          // 総ダメージ量
        int   damageNum_ = 0;       // ダメージを受けた回数
        float lastPlayerHP_ = 0.0f;    // 終了時プレイヤーHP
        float lastEnemyHP_ = 0.0f;     // 終了時敵HP
    };

    GameLog();
    ~GameLog();
    static void OutPut(Propaty propaty);   // テキストファイルに出力

protected:

    // 内部用：時刻取得
    static std::string GetDateTimeString();

private:

};
