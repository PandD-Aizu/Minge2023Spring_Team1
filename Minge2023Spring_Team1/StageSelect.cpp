#include"common.hpp"
#include"Scene.hpp"

StageSelect::StageSelect(const InitData& init)
	: IScene{ init }
{

}

// 更新関数
void StageSelect::update()
{

}

// 描画関数
void StageSelect::draw() const
{
	static const Font font(60);
	font(U"This is StageSelect").drawAt(Scene::Center());
}
