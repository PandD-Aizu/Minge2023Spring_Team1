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

class StageSelect : public App::Scene
{
private:

	const Font font{ FontMethod::MSDF, 48, Typeface::Bold };

	enum ButtonResponse
	{
		normal,
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
	};

	const Array<ButtonItem> ButtonTable = {
		{Rect{ 800, 100, 300, 100 }, font, U"ノーマル",   true,  normal},
		{Rect{ 800, 250, 300, 100 }, font, U"エンドレス", true,  endress},
		{Rect{ 800, 400, 300, 100 }, font, U"予備",       false, other}
	};


	int32 cursorPos = 0;
	int32 cursorMax;


	//ボタン関数
	bool Button(const RectF& rect, bool enabled);
	void ButtonDraw(const RectF& rect, const Font& font_, const String& text, bool enabled,bool Selected) const;


public:

	// コンストラクタ（必ず実装）
	StageSelect(const InitData& init);

	// 更新関数（オプション）
	void update() override;

	// 描画関数（オプション）
	void draw() const override;

};
