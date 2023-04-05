#include"common.hpp"
#include"Scene.hpp"
#include"StageClass.hpp"

Stage::Stage(const InitData& init)
	: IScene{ init }, player(tiles, { 0, 0 })
{
	// CSVからtilesにデータを追加する
	const CSV csv{ U"tiles.csv" };

	if (not csv) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load `tiles.csv`" };
	}

	tiles.setStageNumber(1);
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
			else if (v == U"5" or v == U"RL" or v == U"RefWallL") {
				x << Tiles::Kind::ReflectiveWallL;
			}
			else if (v == U"6" or v == U"RR" or v == U"RefWallR") {
				x << Tiles::Kind::ReflectiveWallR;
			}
			else {
				throw Error{ U"csvに変なモノ({})が混じっています"_fmt(v) };
			}
		}
		tiles << x;
	}
	clear_time.restart();
}

// 更新関数
void Stage::update()
{
	// ゲームクリア時
	if (player.isGameCleared()) {
		// クリアタイムが動いていたら止める
		if (clear_time.isRunning()) {
			clear_time.pause();
		}

		// シフトを押したらシーンをステージセレクトに戻す
		if (KeyShift.down()) {
			changeScene(SceneList::StageSelect);
		}
		return;
	}

	// ゲームプレイ時
	player.update();
}

// 描画関数   
void Stage::draw() const
{
	static Font font(40);
	static int margin = 30, tiles_size = Scene::Height() - margin * 2;
	static Font font_gameClear{ 85, Typeface::Bold };
	static Font font_score{ 50 };

	tiles.draw(Scene::Center().x - tiles_size / 2, margin, Scene::Center().x + tiles_size / 2, Scene::Height() - margin);

	player.draw(Scene::Center().x - tiles_size / 2, margin, Scene::Center().x + tiles_size / 2, Scene::Height() - margin);

	const int score_x_pos = Scene::Center().x + tiles_size / 2 + 120;
	font(clear_time.format(U"MM:ss.xx")).drawAt(score_x_pos, 50);
	font(U"歩行:{}回"_fmt(player.get_walk_count())).drawAt(score_x_pos, 100);

	if (player.isGameCleared()) {
		// ゲームクリア時の表示

		RectF transparent{ 0, 0, Scene::Width(), Scene::Height() }; // ゲーム画面を透過するRectF
		RectF score_board{ Arg::center(transparent.center()),Scene::Width() * 0.6, Scene::Height() * 0.6 };

		transparent.draw(ColorF{ Palette::Gray, 0.8 });
		score_board.draw(Palette::White);

		auto pos = font_gameClear(U"GAME CLEAR!!").draw(Arg::topCenter = score_board.topCenter(), Palette::Yellow);
		pos = font_score(U"クリアタイム: {}"_fmt(clear_time.format(U"MM:ss.xx"))).draw(Arg::topCenter = pos.bottomCenter(), Palette::Black); // クリアタイムの表示
		pos = font_score(U"歩行回数: {}回"_fmt(player.get_walk_count())).draw(Arg::topCenter = pos.bottomCenter(), Palette::Black); // 歩行回数の表示
		pos = font_score(U"スコア: {}"_fmt(U"hoge")).draw(Arg::topCenter = pos.bottomCenter(), Palette::Black); // スコアの表示
		font_score(U"SHIFTキーでステージセレクトへ").draw(Arg::bottomCenter = score_board.bottomCenter(), Palette::Black);
	}
}
