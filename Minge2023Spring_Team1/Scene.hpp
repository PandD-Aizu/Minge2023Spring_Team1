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

	//ボタン関数
	bool Button(const Rect& rect, const Font& font, const String& text, bool enabled);
};
