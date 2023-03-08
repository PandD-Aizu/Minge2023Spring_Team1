#include"common.hpp"
#include"Scene.hpp"

void Main()
{
	// シーンマネージャーを作成
	App manager;

	// タイトルシーンを登録
	manager.add<Title>(SceneList::Title);
	manager.add<Stage>(SceneList::Stage);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
