#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/IntVector3.h"
#include "../Common/Quaternion.h"
#include "../Manager/KeyConfig.h"
#include "../Object/Common/Transform.h"

class Utility
{

public:

	enum class STRING_PLACE	//文字列を揃える位置
	{
		LEFT,		//左揃え
		CENTER,		//中央揃え
		RIGHT,		//右揃え
	};

	// ラジアン(rad)・度(deg)変換用
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	static constexpr float ONE_TRACK_DEG = 360.0f; //1周分の角度

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XZ = { 1.0f, 0.0f, 1.0f };
	static constexpr VECTOR AXIS_XY = { 1.0f, 1.0f, 1.0f };
	static constexpr VECTOR AXIS_YZ = { 0.0f, 1.0f, 1.0f };
	static constexpr VECTOR AXIS_XYZ = { 1.0f, 1.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	//カラーコード
	static constexpr int BLACK = 0x000000;	//黒
	static constexpr int RED = 0xFF0000;	//赤
	static constexpr int BLUE = 0x0000FF;	//青
	static constexpr int YELLOW = 0xFFFF00;	//黄色
	static constexpr int GREEN = 0x008000;	//緑
	static constexpr int CYAN = 0x00FFFF;	//水色
	static constexpr int PINK = 0xFFC0CB;	//桃色
	static constexpr int ORANGE = 0xFFA500;	//オレンジ
	static constexpr int LIME = 0xADFF2F;	//黄緑
	static constexpr int PURPLE = 0x800080;	//紫
	static constexpr int WHITE = 0xFFFFFF;	//白
	static constexpr int GRAY = 0xBBBBBB;	//灰色
	static constexpr int BROWN = 0x8B4513;	//茶色

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	//値を反転させる溜めのスケール
	static constexpr float REVERSE_SCALE = -1.0f;

	//アルファ最大値
	static constexpr int ALPHA_MAX = 255;

	static constexpr float EPSILON = 1e-6f;    //浮動小数誤差対策の極小値

	// 四捨五入
	static int Round(float v);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ラジアン(rad)から度(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);

	// 度(deg)からラジアン(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);

	// 0～360度の範囲に収める
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	
	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// ベジェ曲線
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);
	
	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const Vector2& v1, const Vector2& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	// 球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius, 
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	// 正規化
	static VECTOR Normalize(const Vector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	// 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);

	//待機時間
	static bool IsTimeOver(float& totalTime, const float& waitTime);

	/// <summary>
	/// 文字列を指定位置に揃えて描画
	/// </summary>
	/// <param name="_str">描画する文字列</param>
	/// <param name="_line">揃えるX座標　左揃え：左の座標　中央揃え：中央の座標　右揃え：右の座標</param>
	/// <param name="_posY">描画するY座標</param>
	/// <param name="_color">描画する色コード</param>
	/// <param name="_place">揃える位置</param>
	static void DrawStringPlace(std::string _str, int _line, int _posY, int _color, STRING_PLACE _place);

	/// <summary>
	/// とある点からとある点までの移動ベクトルを返す
	/// </summary>
	/// <param name="_start">狙う側</param>
	/// <param name="_goal">向かう先</param>
	/// <param name="_speed">設定速度(未設定だと、方向ベクトルのみを返す)</param>
	/// <returns>向かう先までの移動ベクトル</returns>
	static const VECTOR GetMoveVec(const VECTOR _start, const VECTOR _goal, const float _speed = 1.0f);

	/// <summary>
	/// 対象に向かう回転軸
	/// </summary>
	/// <param name="_pos">自身の座標</param>
	/// <param name="_targetPos">対象の座標</param>
	/// <param name="_needAxis">必要な回転軸</param>
	/// <returns>回転軸</returns>
	static const VECTOR GetRotAxisToTarget(const VECTOR _pos, const VECTOR _targetPos, const VECTOR _needAxis = AXIS_XYZ);

	/// <summary>
	/// 対象に向く
	/// </summary>
	/// <param name="_trans">回転させるモデルの情報</param>
	/// <param name="_toGoalAxis">対象までの回転軸</param>
	/// <param name="_time">補正完了までの全体時間(指定がないなら補正なし)</param>
	/// <param name="_relativePos">モデル自体の相対座標</param>
	static void LookAtTarget(Transform& _trans, const VECTOR _toTargetAxis, const float _time = 0.0f, const VECTOR _relativePos = VECTOR_ZERO);

	/// <summary>
	/// 特定の範囲内に座標が含まれてるか調べる
	/// </summary>
	/// <param name="pos">調べたい座標</param>
	/// <param name="leftTop">範囲左上</param>
	/// <param name="rightBotm">範囲右下</param>
	/// <returns>判定結果を返す</returns>
	static bool IsPointInRect(const Vector2 _pos, const Vector2 _leftTop, const Vector2 _rightBotm);

	/// <summary>
	/// 特定の円の範囲内に座標が含まれてるか調べる
	/// </summary>
	/// <param name="_pos"></param>
	/// <param name="_circlePos"></param>
	/// <param name="_radius"></param>
	/// <returns></returns>
	static bool IsPointInRectCircle(const Vector2 _pos, const Vector2 _circlePos, const float _radius);

	static VECTOR GetWorldPosAtScreen(const Vector2 screenPos, const float distance, const VECTOR cameraPos, const VECTOR cameraDir);

	//符号を返す
	static int GetSign(float f);
	static int GetSign(int f);

	//エクスプローラーからファイルを開く
	static std::string OpenFileDialog();

	//数値の反転
	static VECTOR ReverseValue(const VECTOR _vec);
	static  int ReverseValue(const int _i);
	static float ReverseValue(const float _f);
	static IntVector3 ReverseValue(const IntVector3 _iv);

	/// <summary>
	/// 範囲外の値を指定した範囲に収める
	/// </summary>
	/// <param name="index"></param>収めたい値
	/// <param name="max"></param>範囲の最大値
	/// <returns></returns>0からN-1の範囲に補正した値
	static int WrapIndex(int index, int max);

	/// <summary>
	/// 点線を描画する
	/// </summary>
	/// <param name="sPos">始点</param>
	/// <param name="ePos">終点</param>
	/// <param name="color">色</param>
	/// <param name="len">間隔</param>
	static void DrawPointLine3D(const VECTOR sPos, const VECTOR ePos, int color, float len = 50.0f);

	/// <summary>
	/// イーズイン
	/// </summary>
	/// <param name="_time">経過時間</param>
	/// <param name="_totalTime">アニメーション総時間</param>
	/// <param name="_start">開始位置</param>
	/// <param name="_end">終了位置</param>
	/// <returns>変化量</returns>
	static float EaseInQuad(float _time,const float _totalTime, const float _start, const float _end);


	/// <summary>
	/// イーズアウト
	/// </summary>
	/// <param name="_time">経過時間</param>
	/// <param name="_totalTime">アニメーション総時間</param>
	/// <param name="_start">開始位置</param>
	/// <param name="_end">終了位置</param>
	/// <returns>変化量</returns>
	static float EaseOutQuad(float _time, const float _totalTime, const float _start, const float _end);

	/// <summary>
	/// イーズインアウトバック
	/// </summary>
	/// <param name="_time">経過時間</param>
	/// <param name="_totalTime">アニメーション総時間</param>
	/// <param name="_start">開始位置</param>
	/// <param name="_end">終了位置</param>
	/// <returns>変化量</returns>
	static float EaseInOutBack(float _time, const float _totalTime, const float _start, const float _end);

	static constexpr float DEFAULT_SHAKE_SPEED = 5.0f;		//揺れの振幅
	static constexpr float DEFAULT_SHAKE_AMPLITUDE = 10.0f;	//揺れの速さ
	/// <summary>
	///	揺らす計算した値を返す
	/// </summary>
	/// <param name="_value">揺らしたい値</param>
	/// <param name="_time">時間</param>
	/// <param name="_speed">速度</param>
	/// <param name="_amplitude">振幅</param>
	/// <returns></returns>揺らす計算した値
	static float GetShake(const float _value, float _time, const float _speed = DEFAULT_SHAKE_SPEED, const float _amplitude = DEFAULT_SHAKE_AMPLITUDE);

	/// <summary>
	/// 指定した桁数の数字を返す
	/// </summary>
	/// <param name="_value">桁値を求める値</param>
	/// <param name="_digit">桁数</param>
	/// <returns>桁値</returns>
	static int GetDigit(const int _value, const int _digit);

	/// <summary>
	/// 桁数を返す
	/// </summary>
	/// <param name="_value">桁数を調べる値</param>
	/// <returns>桁数</returns>
	static int GetDigitCount(const int _value);

	/// <summary>
	/// JSON保存ダイアログを表示し、パスをUTF-8文字列で返す
	/// </summary>
	/// <returns>文字列を返す</returns>
	static std::string ShowSaveJsonDialog();

	/// <summary>
	/// ワイド文字列をUTF-8に変換する
	/// </summary>
	static std::string WideToUtf8(const std::wstring& wstr);

	/// <summary>
	/// 値を上下に動かす処理
	/// </summary>
	/// <param name="_value">現在の値</param>
	/// <param name="_step">増減量</param>
	/// <param name="_max">最大値</param>
	/// <param name="_min">最小値</param>
	/// <param name="_dir">増加方向（参照）1なら増加中、-1なら減少中</param>
	/// <returns>処理後の値</returns>
	static float PingPongUpdate(const float _value, const float _step, const float _max, const float _min, int& _dir);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="pos1">1つ目の球の中心座標</param>
	/// <param name="radius1">１つ目の球の半径</param>
	/// <param name="pos2">2つ目の球の中心座標</param>
	/// <param name="radius2">２つ目の球の半径</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Sphere(VECTOR pos1, float radius1, VECTOR pos2, float radius2);

	/// <summary>
	/// 球とモデルの当たり判定
	/// </summary>
	/// <param name="pos">球の中心座標</param>
	/// <param name="radius">球の半径</param>
	/// <param name="modelId">モデルのハンドル</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Model(VECTOR pos, float radius, int modelId);

	/// <summary>
	/// 円周と円の当たり判定
	/// </summary>
	/// <param name="pos1">円周の中心座標</param>
	/// <param name="radius1">円周の半径</param>
	/// <param name="pos2">円の中心座標</param>
	/// <param name="radius2">円の半径</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCircumference2Circle(VECTOR pos1, float radius1, VECTOR pos2, float radius2);

	/// <summary>
	/// 線と三角形の当たり判定(AI産のため理論は不明)
	/// </summary>
	/// <param name="tPos1">三角形の座標1</param>
	/// <param name="tPos2">三角形の座標2</param>
	/// <param name="tPos3">三角形の座標3</param>
	/// <param name="lPos1">線の座標1</param>
	/// <param name="lPos2">線の座標2</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColTriangle2Line(VECTOR tPos1, VECTOR tPos2, VECTOR tPos3, VECTOR lPos1, VECTOR lPos2,VECTOR& hitPos);

	/// <summary>
	/// 線とカプセルの当たり判定(AI産のため理論は不明)
	/// </summary>
	/// <param name="cPos1">カプセルの座標１</param>
	/// <param name="cPos2">カプセルの座標２</param>
	/// <param name="cRadius">カプセルの半径</param>
	/// <param name="lPos1">線の座標１</param>
	/// <param name="lPos2">線の座標２</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCapsule2Line(VECTOR cPos1, VECTOR cPos2, float cRadius, VECTOR lPos1, VECTOR lPos2, VECTOR& hitPos);

	/// <summary>
	/// カプセルと球の当たり判定(Ai)
	/// </summary>
	/// <param name="cPos1">カプセル座標１</param>
	/// <param name="cPos2">カプセル座標２</param>
	/// <param name="cRadius">カプセルの半径</param>
	/// <param name="sPos">球の座標</param>
	/// <param name="sRadius">球の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCapsule2Sphere(VECTOR cPos1, VECTOR cPos2, float cRadius, VECTOR sPos, float sRadius, VECTOR& hitPos);

	/// <summary>
	/// 球と三角形の当たり判定
	/// </summary>
	/// <param name="sPos">球の座標</param>
	/// <param name="radius">球の半径</param>
	/// <param name="tPos1">三角形の座標１</param>
	/// <param name="tPos2">三角形の座標２</param>
	/// <param name="tPos3">三角形の座標３</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Triangle(VECTOR sPos, float radius, VECTOR tPos1, VECTOR tPos2, VECTOR tPos3, VECTOR& hitPos);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="s1Pos">球１の座標</param>
	/// <param name="radius1">球１の半径</param>
	/// <param name="s2Pos">球２の座標</param>
	/// <param name="radius2">球２の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColSphere2Sphere(VECTOR s1Pos, float radius1, VECTOR s2Pos, float radius2, VECTOR& hitPos);

	/// <summary>
	/// 円柱と円柱の当たり判定
	/// </summary>
	/// <param name="c1Pos">円柱１の座標</param>
	/// <param name="radius1">円柱１の半径</param>
	/// <param name="c2Pos">円柱２の座標</param>
	/// <param name="radius2">円柱２の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCylinder2Cylinder(VECTOR c1Pos, float radius1, VECTOR c2Pos, float radius2, VECTOR& hitPos);

	/// <summary>
	/// 円周と円の当たり判定
	/// </summary>
	/// <param name="pos1">円周の中心座標</param>
	/// <param name="radius1">円周の半径</param>
	/// <param name="pos2">円の中心座標</param>
	/// <param name="radius2">円の半径</param>
	/// <param name="hitPos">当たった座標(参照型)</param>
	/// <returns>当たったらtrue</returns>
	static bool IsColCircumference2Circle(VECTOR pos1, float radius1, VECTOR pos2, float radius2,VECTOR& hitPos);

	/// <summary>
	/// 点pから線分abへの最近点を求める
	/// </summary>
	/// <param name="p"></param>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns>最近点の座標</returns>
	static VECTOR ClosestPointOnSegment(VECTOR p, VECTOR a, VECTOR b);

	/// <summary>
	/// XZ平面に円を描画する
	/// </summary>
	/// <param name="center">円の中心座標</param>
	/// <param name="radius">円の半径</param>
	/// <param name="vertexNum">円の頂点数</param>
	/// <param name="fillFlag">塗りつぶすかどうか</param>
	static void DrawCircle3DXZ(VECTOR center, float radius, int vertexNum,int color, bool fillFlag);

	/// <summary>
	/// ボタンの名前の文字列を取得
	/// </summary>
	/// <param name="btn">ボタンの種類</param>
	/// <returns>ボタンの文字列</returns>
	static std::string GetBtnName(KeyConfig::JOYPAD_BTN btn);

	/// <summary>
	/// ボタンの画像を取得
	/// </summary>
	/// <param name="btn">ボタンの種類</param>
	/// <returns>画像のハンドルID</returns>
	static int GetBtnImage(KeyConfig::JOYPAD_BTN btn);

	/// <summary>
	/// COLOR_FからFLOAT4に変換
	/// </summary>
	/// <param name="color"></param>
	/// <returns></returns>
	static FLOAT4 COLOR_F2FLOAT4(const COLOR_F& color);

	/// <summary>
	/// モデルのフレームの最小最大座標を取得
	/// </summary>
	/// <param name="modelId">求めるモデルのハンドルID</param>
	/// <param name="minPos">最小座標(参照型)</param>
	/// <param name="maxPos">最大座標(参照型)</param>
	/// <param name="outFlameNum">計算に含めないフレーム番号</param>
	static void GetModelFlameBox(int modelId, VECTOR& minPos, VECTOR& maxPos,std::vector<int>outFlameNum ={});

	/// <summary>
	/// モデルのメッシュの最小最大座標を取得
	/// </summary>
	/// <param name="modelId">モデルのハンドルID</param>
	/// <param name="minPos">最小座標(参照型)</param>
	/// <param name="maxPos">最大座標(参照型)</param>
	static void GetModelMeshLocalBox(int modelId, VECTOR& minPos, VECTOR& maxPos);

	/// <summary>
	/// 3点から法線方向を求める
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <returns></returns>
	static VECTOR CalcNormal(const VECTOR& a, const VECTOR& b, const VECTOR& c);

	/// <summary>
	/// 4天の中心を求める
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="c"></param>
	/// <param name="d"></param>
	/// <returns></returns>
	static VECTOR CalcCenter(const VECTOR& a, const VECTOR& b, const VECTOR& c ,const VECTOR& d);

	/// <summary>
	/// vector配列の中のnullptrを削除する
	/// </summary>
	/// <typeparam name="T">型名</typeparam>
	/// <param name="allay">配列の参照型</param>
	/// <returns>削除した要素数</returns>
	template<typename T>
	static int EraseVectorAllay(std::vector<T>& allay);
};
template<typename T>
inline int Utility::EraseVectorAllay(std::vector<T>& allay)
{
	int ret = static_cast<int>(std::erase_if(allay, [](auto& a) {return a == nullptr;}));
	return ret;
}