#pragma once
#include  "../ObjectPolygonBase.h"
#include "../../Manager/SceneManager.h"

class Gate;
class GateMist;

class GateManager : public ObjectPolygonBase
{
public:

	static constexpr float GATE_RADIUS = 1000.0f;
	static constexpr int GATE_NUM = 4;

	GateManager(std::vector<SceneManager::SCENE_ID> scenes);
	~GateManager(void)override;
	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void AddVertexes(Polygon3DRenderer::PolygonInfo info);
private:
	float time_;
	std::vector<std::unique_ptr<Gate>> gates_;
};

