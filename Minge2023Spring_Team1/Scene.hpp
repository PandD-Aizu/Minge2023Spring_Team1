#pragma once
#include"common.hpp"
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
};

class StageSelect : public App::Scene
{
public:

	// コンストラクタ（必ず実装）
	StageSelect(const InitData& init);

	// 更新関数（オプション）
	void update() override;

	// 描画関数（オプション）
	void draw() const override;

	

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
		Rect rect;
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


	//ボタン関数
	bool Button(const Rect& rect, bool enabled);
	void ButtonDraw(const Rect& rect, const Font& font_, const String& text, bool enabled) const;

};
