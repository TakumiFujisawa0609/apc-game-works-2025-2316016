#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "GameLog.h"

GameLog::GameLog()
{
}

GameLog::~GameLog()
{
}


// ======================================================
// 日付 + 時刻 を "YYYY/MM/DD HH:MM:SS" 形式で返す
// ======================================================
std::string GameLog::GetDateTimeString()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t t = system_clock::to_time_t(now);
    std::tm localTime;

#if defined(_WIN32)
    localtime_s(&localTime, &t);  // Windows安全版
#else
    localtime_r(&t, &localTime);  // Linux / Mac
#endif

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y/%m/%d %H:%M:%S");
    return oss.str();
}


// ======================================================
// ログ出力（形式①：項目名=値）
// ======================================================
void GameLog::OutPut(Propaty propaty)
{
    //txtに追加
    std::ofstream txt("Data/Log/GameStats.txt", std::ios::app);
    if (!txt.is_open()) return;

    // 現在時刻付き
    txt << "[" << GetDateTimeString() << "] ";

    // 各種データ
    txt << "Time=" << propaty.time_ << ", "
        << "RollNum=" << propaty.rollAvoidNum_ << ", "
        << "RollSuccess=" << propaty.rollAvoidSaccessNum_ << ", "
        << "JumpNum=" << propaty.jumpAvoidNum_ << ", "
        << "JumpSuccess=" << propaty.jumpAvoidSaccessNum_ << ", "
        << "Damage=" << propaty.damage_ << ", "
        << "DamageNum=" << propaty.damageNum_ << ", "
        << "LastHP=" << propaty.lastPlayerHP_ << ", "
        << "EnemyHP=" << propaty.lastEnemyHP_
        << "\n";

    txt.close();

    //csvに追加
    std::ofstream csv("Data/Log/GameStats.csv", std::ios::app);
    if (!csv.is_open()) return;

    int win = propaty.lastEnemyHP_ <= 0.0f ? 1 : propaty.lastPlayerHP_ < 0.0f ? 0 : -1;

    // 各種データ
    csv << win << ","
        << propaty.time_ << ", "
        << propaty.rollAvoidNum_ << ", "
        << propaty.rollAvoidSaccessNum_ << ", "
        << propaty.jumpAvoidNum_ << ", "
        << propaty.jumpAvoidSaccessNum_ << ", "
        << propaty.damage_ << ", "
        << propaty.damageNum_ << ", "
        << propaty.lastPlayerHP_ << ", "
        << propaty.lastEnemyHP_
        << "\n";

    csv.close();
}

