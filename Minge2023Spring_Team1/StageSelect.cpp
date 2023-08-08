#include"common.hpp"
#include"Scene.hpp"




StageSelect::StageSelect(const InitData& init)
	: IScene{ init }
{

	ButtonItem ArrItem;
	String StagePass_;

	size_t tentative_stageMax = 3;


	for (size_t i = 0; i < 10; i++)
	{
		int y = i / 7;
		int x = i % 7;
		ArrItem.rect = Rect{ 100 + 150 * x, 50 + 150 * y, 100, 100 };
		ArrItem.font = font;
		ArrItem.text = U"{:0>3}"_fmt(i + 1);
		ArrItem.response = gotoGame;

		ArrItem.enabled = true;
		//ArrItem.enabled = (i < tentative_stageMax);

		ArrItem.StageNo = i + 1;

		StagePass_ = U"stage/n{:0>3}.csv"_fmt(i + 1);
		ArrItem.StagePass = StagePass_;

		ButtonTable << ArrItem;
	}


	//ArrItem = { Rect{ 900, 550, 300, 100 }, font, U"予備", false, other };
	//ArrItem = { Rect{ 700, 550, 200, 100 }, font, U"エンドレステスト", true, endress };
	//ArrItem.StagePass = U"tiles.csv";
	//ButtonTable << ArrItem;

	//ArrItem = { Rect{ 1000, 550, 200, 100 }, font, U"テスト", true, gotoGame };
	//ArrItem.StagePass = U"tiles.csv";
	//ButtonTable << ArrItem;

	cursorMax = ButtonTable.size();
}


// 更新関数
void StageSelect::update()
{



	if (inputRight.pressed())
	{
		KeyAccumulatedTime += Scene::DeltaTime();

		if (inputRight.down() || KeyAccumulatedTime >= 0.3) {

			cursorPos++;
			cursorPos = cursorPos % cursorMax;

			KeyAccumulatedTime = 0;
		}
	}
	else if (inputLeft.pressed())
	{
		KeyAccumulatedTime += Scene::DeltaTime();

		if (inputLeft.down() || KeyAccumulatedTime >= 0.3) {

			cursorPos--;
			cursorPos += cursorMax;
			cursorPos = cursorPos % cursorMax;

			KeyAccumulatedTime = 0;
		}
	}
	else KeyAccumulatedTime = 0;



	for (size_t i = 0; i < ButtonTable.size(); ++i) {


		if (ButtonMouseOver(ButtonTable[i].rect, ButtonTable[i].enabled)) cursorPos = i;

		if (ButtonClicked(ButtonTable[i].rect, ButtonTable[i].enabled)
			|| (cursorPos == i && decisionKey.down() && ButtonTable[i].enabled) )
		{
			switch (ButtonTable[i].response)
			{
			case gotoGame:
				//Print << ButtonTable[i].StagePass;

				getData().StagePass = ButtonTable[i].StagePass;
				getData().StageNo = ButtonTable[i].StageNo;

				changeScene(SceneList::Stage);
				break;

			case endress :
				//Print << ButtonTable[i].StagePass;

				getData().StagePass = ButtonTable[i].StagePass;
				getData().StageNo = ButtonTable[i].StageNo;

				changeScene(SceneList::EndlessStage);
				break;

			default:
				//Print << U"C";
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
bool StageSelect::ButtonClicked(const RectF& rect, bool enabled)
{
	return (enabled && rect.leftClicked());
}

bool StageSelect::ButtonMouseOver(const RectF& rect, bool enabled) {

	if (enabled && rect.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	return (enabled && rect.mouseOver());
}



//ボタン関数
void StageSelect::ButtonDraw(const RectF& rect, const Font& font_, const String& text, bool enabled, bool Selected) const
{

	if (Selected)
	{
		rect.stretched(20).drawFrame(2, 2);
	}


	if (enabled)
	{

		if (Selected)
		{
			rect.draw(ColorF{ 0.3, 0.7, 1.0 });
			rect.drawFrame(2, 2, ColorF{ 0.6, 0.85, 1.0 });

			font_(text).drawAt(30, (rect.x + rect.w / 2), (rect.y + rect.h / 2), ColorF{ 1 });
		}
		else {
			rect.draw(ColorF{ 0.2, 0.5, 1.0 });
			rect.drawFrame(2, 2, ColorF{ 0.4, 0.6, 1.0 });

			font_(text).drawAt(30, (rect.x + rect.w / 2), (rect.y + rect.h / 2), ColorF{ 0.8 });
		}
	}
	else
	{
		rect.draw(ColorF{ 0.5 });
		rect.drawFrame(2, 2, ColorF{ 0.5 });

		font_(text).drawAt(30, (rect.x + rect.w / 2), (rect.y + rect.h / 2), ColorF{ 0.7 });
	}
}
