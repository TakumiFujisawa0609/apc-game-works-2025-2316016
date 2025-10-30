#pragma once
#include <map>
#include <memory>
#include <vector>

class Transform;
class ModelRenderer;

class DrawTranslucentManager	//”¼“§–¾‚Ì•`‰æ‡ŠÇ—
{
public:

	static void CreateInstance(void);
	static DrawTranslucentManager& GetInstance(void);

	void Init(void);
	void Draw(void);
	void Destroy(void);

	void Add(std::shared_ptr<Transform> transform, std::shared_ptr<ModelRenderer> renderer = nullptr);


private:

	struct TranslucentObject
	{
		std::shared_ptr<Transform> transform;		
		std::shared_ptr<ModelRenderer> renderer;
		float distance = 0.0f;
	};

	struct TranslucentObjectLess
	{
		bool operator()(const TranslucentObject& a, const TranslucentObject& b) const
		{
			return a.distance > b.distance;
		}
	};

	static DrawTranslucentManager* instance_;
	std::vector<TranslucentObject> translucentObjects_;
	DrawTranslucentManager(void);
	~DrawTranslucentManager(void);
};

