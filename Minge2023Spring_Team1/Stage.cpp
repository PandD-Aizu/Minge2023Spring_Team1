#include"common.hpp"
#include"Scene.hpp"
#include"StageClass.hpp"

Stage::Stage(const InitData& init)
	: IScene{ init }
{
	// ブロックの追加プログラム(仮なので削除・変更大歓迎)
	tiles << Array<Tiles::Kind>{Tiles::Kind::Target, Tiles::Kind::None, Tiles::Kind::Wall, Tiles::Kind::None};
	tiles << Array<Tiles::Kind>{Tiles::Kind::Wall, Tiles::Kind::None, Tiles::Kind::None, Tiles::Kind::None};
	tiles << Array<Tiles::Kind>{Tiles::Kind::Wall, Tiles::Kind::Wall, Tiles::Kind::None, Tiles::Kind::Wall};
	tiles << Array<Tiles::Kind>{Tiles::Kind::None, Tiles::Kind::None, Tiles::Kind::None, Tiles::Kind::None};

}

// 更新関数
void Stage::update()
{
}

// 描画関数
void Stage::draw() const
{
	static int margin = 30, tiles_size = Scene::Height() - margin * 2;
	tiles.draw(Scene::Center().x - tiles_size / 2, margin, Scene::Center().x + tiles_size / 2, Scene::Height() - margin);
}
