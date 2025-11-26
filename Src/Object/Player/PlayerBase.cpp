#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Light/PointLight.h"
#include "PlayerBase.h"

PlayerBase::PlayerBase(int playerNum) :keyIns_(KeyConfig::GetInstance())
{
	rimPow_ = RIM_MIN_POW;
	playerNum_ = playerNum;
	transform_ = std::make_shared<Transform>();
	transform_->pos = MOVE_LIMIT_MIN;
	transform_->SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	transform_->scl = VGet(SIZE, SIZE, SIZE);
	transform_->Update();
	material_ = std::make_unique<ModelMaterial>(
		"PlayerVS.cso", 0,
		"PlayerPS.cso", 3
	);
	//material_->AddConstBufVS({ TEXTURE_SCALE, 0.0f, 1.0f, 1.0f });
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(DEFAULT_COLOR_TIMES)));
	auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	material_->AddConstBufPS({ cameraPos.x,cameraPos.y,cameraPos.z,rimPow_ });
	material_->AddConstBufPS(static_cast<FLOAT4>(Utility::COLOR_F2FLOAT4(RIM_COLOR)));
	//material_->SetTextureBuf(3, ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	renderer_ = std::make_shared<ModelRenderer>(
		transform_->modelId, *material_
	);
	InitAnimationController();
	pointLight_ = std::make_unique<PointLight>(*transform_);
}

PlayerBase::~PlayerBase(void)
{
}

void PlayerBase::Init(void)
{
}

void PlayerBase::Update(void)
{
}

void PlayerBase::Draw(void)
{
	int color = (state_ == STATE::DAMAGE) ? GetColor(255, 0, 0) : GetColor(0, 255, 0);
	VECTOR landPos = transform_->pos;
	landPos.y = MOVE_LIMIT_MIN.y;
	Utility::DrawCircle3DXZ(landPos, RADIUS, 16, GetColor(0, 0, 0), true);
	//MV1DrawModel(transform_->modelId);
	renderer_->Draw();
}

void PlayerBase::UIDraw(void)
{
	animCtrl_->DebugDraw();
}

void PlayerBase::OnHit(const std::weak_ptr<Collider> _hitCol, VECTOR hitPos)
{
}

void PlayerBase::SetPos(const VECTOR& pos)
{
	transform_->pos = pos;
	transform_->Update();
}

void PlayerBase::SetRot(const VECTOR& rot)
{
	transform_->rot = rot;
	transform_->quaRot = Quaternion(rot);
	transform_->Update();
}

void PlayerBase::InitAnimationController(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animCtrl_ = std::make_unique<AnimationController>(transform_->modelId);
	animCtrl_->Add((int)STATE::IDLE, path + "Idle.mv1", 60.0f);
	animCtrl_->Add((int)STATE::MOVE, path + "Move.mv1", 120.0f);
	animCtrl_->Add((int)STATE::JUMP, path + "Jump.mv1", 120.0f);
	animCtrl_->Add((int)STATE::AVOID, path + "Avoid2.mv1", 100.0f);
	animCtrl_->Add((int)STATE::ATTACK, path + "Throw.mv1", 210.0f);
	animCtrl_->Add((int)STATE::DAMAGE, path + "Damage.mv1", 120.0f);
	//animCtrl_->Add((int)STATE::DEAD, path + "Falling.mv1", 80.0f);

	animCtrl_->Play((int)STATE::IDLE);
}
