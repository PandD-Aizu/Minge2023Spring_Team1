#include "StageClass.hpp"

constexpr Point deltaPosList[] = {
	{ 0, -1 },
	{ 0, 1 },
	{ -1, 0 },
	{ 1, 0 }
};

Player::Player(Tiles *tiles, Point position)
	: tiles{ tiles }, position{ position } {}

void Player::update() {
	if (inputUp.down()) {
		move(0, KeyShift.pressed());
	}
	else if (inputDown.down()) {
		move(1, KeyShift.pressed());
	}
	else if (inputLeft.down()) {
		move(2, KeyShift.pressed());
	}
	else if (inputRight.down()) {
		move(3, KeyShift.pressed());
	}
}

void Player::draw(Point left_upper, Point right_bottom) const {
	//ブロックのサイズ算出 (tiles.cppから引用)
	double block_size = Min((double)(right_bottom.y - left_upper.y) / tiles->size(), (double)(right_bottom.x - left_upper.x) / tiles->width_size());

	Vec2 drawPos = left_upper + (position + Vec2{ 0.5, 0.5 }) * block_size;
	Circle{ drawPos, block_size * 0.25 }.draw(Palette::Blue);
}

void Player::draw(int x1, int y1, int x2, int y2) const {
	draw(Point{ x1, y1 }, Point{ x2, y2 });
}

void Player::move(int direction, bool isDash) {
	Point deltaPos = deltaPosList[direction];

	while (true) {
		Point nextPos = position + deltaPos;

		// 盤上範囲内チェック
		if (nextPos.x < 0 || nextPos.y < 0 || nextPos.y >= tiles->size()) break;
		if (nextPos.x >= (*tiles)[nextPos.y].size()) break;

		// タイルチェック
		if ((*tiles)[nextPos.y][nextPos.x] == Tiles::Kind::Wall) {
			// 壁だった場合
			// 移動せず終了
			break;
		}
		else if ((*tiles)[nextPos.y][nextPos.x] == Tiles::Kind::Target) {
			// ターゲットだった場合
			// 移動せず終了
			break;
		}

		// 移動確定
		position = nextPos;

		if (!isDash) break; // ダッシュが有効な場合、移動し続ける
	}
}
