#include <algorithm>
#include <DxLib.h>
#include "../Object/Common/Transform.h"
#include "../Renderer/ModelRenderer.h"
#include "DrawTranslucentManager.h"

DrawTranslucentManager* DrawTranslucentManager::instance_ = nullptr;

void DrawTranslucentManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new DrawTranslucentManager();
	}
	instance_->Init();
}

DrawTranslucentManager& DrawTranslucentManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		DrawTranslucentManager::CreateInstance();
	}
	return *instance_;
}

void DrawTranslucentManager::Init(void)
{
}

void DrawTranslucentManager::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	SetWriteZBufferFlag(FALSE);
	for (auto& obj : translucentObjects_)
	{
		VECTOR camPos = GetCameraPosition();
		obj.distance = VSize(VSub(obj.transform->pos, camPos));
	}
	std::sort(translucentObjects_.begin(), translucentObjects_.end(), TranslucentObjectLess());
	for (auto& obj : translucentObjects_)
	{
		if (obj.renderer != nullptr)
		{
			obj.renderer->Draw();
		}
		else
		{
			MV1DrawModel(obj.transform->modelId);
		}
	}
	translucentObjects_.clear();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	SetWriteZBufferFlag(TRUE);
}

void DrawTranslucentManager::Destroy(void)
{
	delete instance_;
}

void DrawTranslucentManager::Add(std::shared_ptr<Transform> transform, std::shared_ptr<BaseRenderer> renderer)
{
	TranslucentObject obj;
	obj.transform = transform;
	obj.renderer = renderer;
	translucentObjects_.push_back(obj);
}

DrawTranslucentManager::DrawTranslucentManager(void)
{
}

DrawTranslucentManager::~DrawTranslucentManager(void)
{
}
