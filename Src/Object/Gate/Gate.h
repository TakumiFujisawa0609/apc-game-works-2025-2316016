#pragma once
#include "../../Manager/SceneManager.h"
#include "../ObjectModelBase.h"

class GateMist;

class Gate : public ObjectModelBase
{
public:
	static constexpr int PILLER_FRAME_NUM = 0;	//柱のフレーム番号
	static constexpr int TOP_FRAME_NUM = 1;		//天井のフレーム番号

	static constexpr VECTOR SCALE = { 0.3f,0.3f,0.3f };
	static constexpr float TEXTURE_SCALE = 3.0f;
	Gate(SceneManager::SCENE_ID nextSceneID);
	~Gate(void) override;
	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void UIDraw(void) override;
	void OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos) override;
	void SetPos(VECTOR pos);
	void SetDegY(float degY);

protected:

private:

	std::unique_ptr<GateMist> mist_;

	SceneManager::SCENE_ID nextSceneID_;

	//当たり判定用頂点
	VECTOR pos1_;
	VECTOR pos2_;
	VECTOR pos3_;
	VECTOR pos4_;
	void InitCollider(void);
	void UpdatePos(void);
	void SetMistPos(void);
};

