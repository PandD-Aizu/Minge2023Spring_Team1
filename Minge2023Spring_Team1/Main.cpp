#include"common.hpp"
#include"Scene.hpp"

void Main()
{
	// シーンマネージャーを作成
	App manager;

	// タイトルシーンを登録
	manager.add<Title>(SceneList::Title);
	manager.add<Stage>(SceneList::Stage);
	manager.add<StageSelect>(SceneList::StageSelect);

	manager.init(SceneList::StageSelect);

	manager.init(SceneList::Stage);

	// 画面サイズを変更
	Window::Resize(1280, 720);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
