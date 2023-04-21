﻿#include"common.hpp"
#include"Scene.hpp"
#include"StageClass.hpp"

EndlessStage::EndlessStage(const InitData& init)
	: IScene{ init }, player(tiles, { 0, 0 })
{
	// CSVからtilesにデータを追加する
	const CSV csv{ getData().StagePass };

	if (not csv) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load CSV of Stage" };
	}


	tiles.setStageNumber(getData().StageNo);

	for (size_t row = 0; row < csv.rows(); row++)
	{
		Array<Tiles::Kind> x;
		for (auto& v : csv[row]) {
			if (v.isEmpty() or v == U"0" or v == U"N" or v == U"None") {
				x << Tiles::Kind::None;
			}
			else if (v == U"1" or v == U"W" or v == U"Wall") {
				x << Tiles::Kind::Wall;
			}
			else if (v == U"2" or v == U"T" or v == U"Target") {
				x << Tiles::Kind::Target;
			}
			else if (v == U"3" or v == U"B" or v == U"Box") {
				x << Tiles::Kind::Box;
			}
			else if (v == U"5" or v == U"RL" or v == U"RefWallL") {
				x << Tiles::Kind::ReflectiveWallL;
			}
			else if (v == U"6" or v == U"RR" or v == U"RefWallR") {
				x << Tiles::Kind::ReflectiveWallR;
			}
			else if (v == U"7" or v == U"H" or v == U"WarpHole") {
				x << Tiles::Kind::WarpHole;
			}
			else {
				throw Error{ U"csvに変なモノ({})が混じっています"_fmt(v) };
			}
		}
		tiles << x;
	}
	timeLimit.restart();
}

// 更新関数
void EndlessStage::update()
{
	// ゲームオーバー時
	if (timeLimit.reachedZero()) {
		// シフトを押したらシーンをステージセレクトに戻す
		if (KeyShift.down()) {
			changeScene(SceneList::StageSelect);
		}
		return;
	}

	// ゲームプレイ時
	player.update();

	// イベント処理
	for (GameEvent e = tiles.popEventQueue(); e != GameEvent::None; e = tiles.popEventQueue()) {
		switch (e) {
		case GameEvent::BreakTarget:
			timeLimit.setRemaining(timeLimit.remaining() + TIMER_INCREASE_BREAK_TARGET);
			break;
		case GameEvent::BreakBox:
			timeLimit.setRemaining(timeLimit.remaining() + TIMER_INCREASE_BREAK_BOX);
			break;
		}
	}
}

// 描画関数   
void EndlessStage::draw() const
{
	static Font font(40);
	static int timeBarHeight = 20;
	static int margin = 30, tiles_size = Scene::Height() - margin * 2 - timeBarHeight;
	static Font font_gameClear{ 85, Typeface::Bold };
	static Font font_score{ 50 };

	tiles.draw(Scene::Center().x - tiles_size / 2, margin, Scene::Center().x + tiles_size / 2, Scene::Height() - margin - timeBarHeight);

	player.draw(Scene::Center().x - tiles_size / 2, margin, Scene::Center().x + tiles_size / 2, Scene::Height() - margin - timeBarHeight);

	const int score_x_pos = Scene::Center().x + tiles_size / 2 + 120;
	font(timeLimit.format(U"MM:ss.xx")).drawAt(score_x_pos, 50);
	font(U"歩行:{}回"_fmt(player.get_walk_count())).drawAt(score_x_pos, 100);

	// 画面下カウントダウンバーの描画
	double timeRatio = timeLimit.remaining() / 1min;
	RectF timeBar{
		0, Scene::Height() - timeBarHeight,
		Scene::Width() * timeRatio, Scene::Height()
	};
	timeBar.draw(HSV{ timeRatio < 1 ? 120 * timeRatio : 120, 1, 1});

	if (timeLimit.reachedZero()) {
		// ゲームオーバー時の表示

		RectF transparent{ 0, 0, Scene::Width(), Scene::Height() }; // ゲーム画面を透過するRectF
		RectF score_board{ Arg::center(transparent.center()),Scene::Width() * 0.6, Scene::Height() * 0.6 };

		transparent.draw(ColorF{ Palette::Gray, 0.8 });
		score_board.draw(Palette::White);

		auto pos = font_gameClear(U"GAME OVER").draw(Arg::topCenter = score_board.topCenter(), Palette::Yellow);
		pos = font_score(U"歩行回数: {}回"_fmt(player.get_walk_count())).draw(Arg::topCenter = pos.bottomCenter(), Palette::Black); // 歩行回数の表示
		pos = font_score(U"スコア: {}"_fmt(U"hoge")).draw(Arg::topCenter = pos.bottomCenter(), Palette::Black); // スコアの表示
		font_score(U"SHIFTキーでステージセレクトへ").draw(Arg::bottomCenter = score_board.bottomCenter(), Palette::Black);
	}
}
