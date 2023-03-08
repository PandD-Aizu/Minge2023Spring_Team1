#include"common.hpp"
#include"Scene.hpp"

Stage::Stage(const InitData& init)
	: IScene{ init }
{

}

// 更新関数
void Stage::update()
{

}

// 描画関数
void Stage::draw() const
{
	static const Font font(60);
	font(U"This is Stage").drawAt(Scene::Center());
}
