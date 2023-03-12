#include"common.hpp"
#include"Scene.hpp"
#include"StageClass.hpp"

Stage::Stage(const InitData& init)
	: IScene{ init }
{
	// ブロックの追加プログラム(仮なので削除・変更大歓迎)
	blocks << Array<Blocks::Kind>{Blocks::Kind::Target, Blocks::Kind::None, Blocks::Kind::Wall, Blocks::Kind::None};
	blocks << Array<Blocks::Kind>{Blocks::Kind::Wall, Blocks::Kind::None, Blocks::Kind::None, Blocks::Kind::None};
	blocks << Array<Blocks::Kind>{Blocks::Kind::Wall, Blocks::Kind::Wall, Blocks::Kind::None, Blocks::Kind::Wall};
	blocks << Array<Blocks::Kind>{Blocks::Kind::None, Blocks::Kind::None, Blocks::Kind::None, Blocks::Kind::None};

}

// 更新関数
void Stage::update()
{
}

// 描画関数
void Stage::draw() const
{
	static int margin = 30, blocks_size = Scene::Height() - margin * 2;
	blocks.draw(Scene::Center().x - blocks_size / 2, margin, Scene::Center().x + blocks_size / 2, Scene::Height() - margin);
}
