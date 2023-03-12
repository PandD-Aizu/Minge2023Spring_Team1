﻿#include"common.hpp"
#include"Scene.hpp"
#include"StageClass.hpp"

Stage::Stage(const InitData& init)
	: IScene{ init }
{
	// CSVからtilesにデータを追加する
	const CSV csv{ U"tiles.csv" };

	if (not csv) // もし読み込みに失敗したら
	{
		throw Error{ U"Failed to load `tiles.csv`" };
	}

	for (size_t row = 0; row < csv.rows(); row++)
	{
		Array<Tiles::Kind> x;
		for (auto& v : csv[row]) {
			if (v.isEmpty() or v == U"0" or v == U"N" or v == U"None") {
				x << Tiles::Kind::None;
			}
			else if (v==U"1" or v==U"W" or v==U"Wall") {
				x << Tiles::Kind::Wall;
			}
			else if (v == U"2" or v == U"T" or v == U"Target") {
				x << Tiles::Kind::Target;
			}
			else {
				throw Error{U"csvに変なモノ({})が混じっています"_fmt(v)};
			}
		}
		tiles << x;
	}

}

// 更新関数
void Stage::update()
{
}

// 描画関数
void Stage::draw() const
{
	static int margin = 30, tiles_size = Scene::Height() - margin * 2;
	tiles.draw(Scene::Center().x - tiles_size / 2, margin, Scene::Center().x + tiles_size / 2, Scene::Height() - margin);
}
