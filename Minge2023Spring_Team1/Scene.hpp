#pragma once
#include"common.hpp"
#include"StageClass.hpp"
class Title : public App::Scene
{
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
		{Rect{ 50, 50, 100, 100 }, font, U"001",   true,  normal},
		{Rect{ 800, 250, 100, 100 }, font, U"002", true,  endress},
		{Rect{ 800, 400, 100, 100 }, font, U"予備",       false, other}
	};


	size_t cursorPos = 0;
	size_t cursorMax = ButtonTable.size();



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
