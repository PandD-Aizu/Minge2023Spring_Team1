#include"common.hpp"
#include"Scene.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{

}

// 更新関数
void Title::update()
{

}

// 描画関数
void Title::draw() const
{
	static const Font font(60);
	font(U"This is Title").drawAt(Scene::Center());
}
