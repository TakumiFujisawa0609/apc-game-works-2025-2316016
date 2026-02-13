#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "PushKeyUI.h"

PushKeyUI::PushKeyUI(FLOAT3 color,TYPE type ,VECTOR outlineCol ,FLOAT2 size)
{
	size_ = size;
	uiHandle_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::PUSH_KEY_IMG).handleId_;
	material_ = std::make_unique<Polygon2DMaterial>("PushKeyUI.cso",3);
	material_->AddTextureBuf(uiHandle_);
	material_->AddTextureBuf(ResourceManager::GetInstance().Load(ResourceManager::SRC::NOISE).handleId_);
	material_->AddTextureBuf(SceneManager::GetInstance().GetMainScreen());
	material_->AddConstBuf({color.x,color.y,color.z, static_cast<float>(type)});
	material_->AddConstBuf({time_,TIME_POWER,0.0f, 0.0f });
	material_->AddConstBuf({outlineCol.x,outlineCol.y,outlineCol.z, 0.0f });
	renderer_ = std::make_shared<Polygon2DRenderer>(*material_,polygonInfo_);
	screenHandle_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);
}

PushKeyUI::~PushKeyUI(void)
{
}

void PushKeyUI::Init(void)
{
	
}

void PushKeyUI::Update(void)
{
	time_ += SceneManager::GetInstance().GetDeltaTime();
	material_->SetConstBuf(1, { time_,TIME_POWER,0.0f, 0.0f });
}

void PushKeyUI::Draw(void)
{
	SetDrawScreen(screenHandle_);
	material_->SetTextureBuf(2, SceneManager::GetInstance().GetMainScreen());
	renderer_->Draw();
	SetDrawScreen(SceneManager::GetInstance().GetMainScreen());
	DrawGraph(0, 0, screenHandle_, true);
}
