#pragma once
#include"common.hpp"
#include"StageClass.hpp"
class Title : public App::Scene
{
private:
	int32 buttonNum = 2; // ボタンの数
	int32 cursorPosition = 0; // ボタン選択位置
	RectF button_startGame; // スタートボタン
	RectF button_quit; // 終了ボタン

	const Texture background{ U"sprites/title.png" }; // 背景の画像
public:

	// コンストラクタ（必ず実装）
	Title(const InitData&);

	// 更新関数（オプション）
	void update() override;

	// 描画関数（オプション）
	void draw() const override;
};

class Stage : public App::Scene
{
public:

	// コンストラクタ（必ず実装）
	Stage(const InitData& init);

	// 更新関数（オプション）
	void update() override;

	// 描画関数（オプション）
	void draw() const override;
private:
	Stopwatch clear_time;

	Tiles tiles;
	Player player;
};

class EndlessStage : public App::Scene
{
public:

	// コンストラクタ（必ず実装）
	EndlessStage(const InitData& init);

	// 更新関数（オプション）
	void update() override;

	// 描画関数（オプション）
	void draw() const override;
private:
	// カウントダウン用のタイマー
	Timer timeLimit{ 0.1min };

	Tiles tiles;
	Player player;

	// ターゲット破壊時のタイマー基礎増加分
	const Duration TIMER_INCREASE_BREAK_TARGET = 5s;
	// 箱破壊時のタイマー基礎増加分
	const Duration TIMER_INCREASE_BREAK_BOX = 5s;
};

class StageSelect : public App::Scene
{
private:

	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	enum ButtonResponse
	{
		gotoGame,
		endress,
		other
	};

	struct ButtonItem
	{
		RectF rect;
		Font font;
		String text;
		bool enabled;

		ButtonResponse response;
		String StagePass;
		int StageNo;
		String StageName;
	};

	/*
	const Array<ButtonItem> ButtonTable = {
		{Rect{  50, 50, 100, 100 }, font, U"001",   true,  gotoGame},
		{Rect{ 200, 50, 100, 100 }, font, U"002", true,  gotoGame},
		{Rect{ 350, 50, 100, 100 }, font, U"003", true,  gotoGame},
		{Rect{ 500, 50, 100, 100 }, font, U"004", true,  gotoGame},
		{Rect{ 800, 400, 100, 100 }, font, U"予備",       false, other}
	};
	*/

	Array<ButtonItem> ButtonTable;

	size_t cursorPos = 0;
	size_t cursorMax;// = ButtonTable.size();
	double KeyAccumulatedTime;


	//ボタン関数
	bool ButtonMouseOver(const RectF& rect, bool enabled);
	bool ButtonClicked(const RectF& rect, bool enabled);

	void ButtonDraw(const RectF& rect, const Font& font_, const String& text, bool enabled,bool Selected) const;


public:

	// コンストラクタ（必ず実装）
	StageSelect(const InitData& init);

	// 更新関数（オプション）
	void update() override;

	// 描画関数（オプション）
	void draw() const override;

};
