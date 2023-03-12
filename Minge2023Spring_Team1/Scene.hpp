#pragma once
#include"common.hpp"
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
};
