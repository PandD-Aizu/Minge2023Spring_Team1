#include"common.hpp"
#include"Scene.hpp"

StageSelect::StageSelect(const InitData & init)
	: IScene{ init }
{

}


// 更新関数
void StageSelect::update()
{
	static const Font font{ FontMethod::MSDF, 48, Typeface::Bold };


	if (Button(Rect{ 800, 100, 300, 100 }, font, U"ノーマル", true))
	{
		Print << U"A";
	}

	if (Button(Rect{ 800, 250, 300, 100 }, font, U"エンドレス", true))
	{
		Print << U"B";
	}

	if (Button(Rect{ 800, 400, 300, 100 }, font, U"予備", false))
	{
		Print << U"C";
	}

}

// 描画関数
void StageSelect::draw() const
{
	static const Font font(60);
	//font(U"This is StageSelect").drawAt(Scene::Center());

}

//ボタン関数
bool StageSelect::Button(const Rect& rect, const Font& font, const String& text, bool enabled)
{
	if (enabled && rect.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (enabled)
	{
		rect.draw(ColorF{ 0.3, 0.7, 1.0 });
		rect.drawFrame(2, 2, ColorF{ 0.5, 0.7, 1.0 });

		font(text).drawAt(40, (rect.x + rect.w / 2), (rect.y + rect.h / 2));
	}
	else
	{
		rect.draw(ColorF{ 0.5 });
		rect.drawFrame(2, 2, ColorF{ 0.5 });

		font(text).drawAt(40, (rect.x + rect.w / 2), (rect.y + rect.h / 2), ColorF{ 0.7 });
	}

	return (enabled && rect.leftClicked());
}
