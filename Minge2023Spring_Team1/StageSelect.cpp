#include"common.hpp"
#include"Scene.hpp"




StageSelect::StageSelect(const InitData & init)
	: IScene{ init }
{

}


// 更新関数
void StageSelect::update()
{
	
	for (size_t i = 0; i < ButtonTable.size(); ++i) {

		if (Button(ButtonTable[i].rect, ButtonTable[i].enabled))
		{
			switch (ButtonTable[i].response)
			{
			case normal :
				Print << U"A";
				break;

			case endress :
				Print << U"B";
				break;

			default:
				Print << U"C";
				break;
			}
		}
	}


}

// 描画関数
void StageSelect::draw() const
{
	for (size_t i = 0; i < ButtonTable.size(); ++i) {

		ButtonDraw(ButtonTable[i].rect, ButtonTable[i].font, ButtonTable[i].text, ButtonTable[i].enabled, (cursorPos == i) );

	}
}

//ボタン関数
bool StageSelect::Button(const RectF& rect, bool enabled)
{
	if (enabled && rect.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	return (enabled && rect.leftClicked());
}

//ボタン関数
void StageSelect::ButtonDraw(const RectF& rect, const Font& font_, const String& text, bool enabled, bool Selected) const
{
	
	if (enabled)
	{
		rect.draw(ColorF{ 0.2, 0.5, 1.0 });
		rect.drawFrame(2, 2, ColorF{ 0.4, 0.6, 1.0 });

		font_(text).drawAt(40, (rect.x + rect.w / 2), (rect.y + rect.h / 2), ColorF{ 0.8 });
	}
	else
	{
		rect.draw(ColorF{ 0.5 });
		rect.drawFrame(2, 2, ColorF{ 0.5 });

		font_(text).drawAt(40, (rect.x + rect.w / 2), (rect.y + rect.h / 2), ColorF{ 0.7 });
	}
}


