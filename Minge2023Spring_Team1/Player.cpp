#include "StageClass.hpp"

Player::Player(Tiles& tiles, Point position)
	: tiles{ tiles }, position{ position } {
	delayTimer.setRemaining(0s);
	delayTimer.start();
}

void Player::update() {
	if (delayTimer.reachedZero()) {
		if (!dashFlag) {
			// ダッシュ移動中でない場合
			// 方向入力受付
			if (inputUp.pressed()) direction = Direction::Up;
			else if (inputDown.pressed()) direction = Direction::Down;
			else if (inputLeft.pressed()) direction = Direction::Left;
			else if (inputRight.pressed()) direction = Direction::Right;
			else return; // 移動キーを押さなかった場合はここで処理終了

			// ↓↓↓方向キーを押した場合の処理↓↓↓
			if (KeyShift.pressed()) {
				// ダッシュ移動開始
				dashFlag = true;
			}
			else {
				// 通常歩行処理
				if (move(direction) != Direction::None) {
					// 進行方向に壁がなく移動に成功した場合
					walk_count++;
					// 行動遅延設定
					delayTimer.set(WALKING_DELAY);
					delayTimer.start();
				}
			}
			// ↑↑↑方向キーを押した場合の処理↑↑↑　ここまで
		}

		if (dashFlag) {
			// ダッシュ移動中の場合

			// そのまま移動する
			Direction nextDirection = move(direction);

			if (nextDirection != Direction::None) {
				// 移動できた場合
				direction = nextDirection;
				// 行動遅延設定
				delayTimer.set(DASHING_DELAY);
				delayTimer.start();
			}
			else {
				// 移動できなかった場合、ダッシュ移動を終了する
				dashFlag = false;
			}
		}
	}
}

void Player::draw(Point left_upper, Point right_bottom) const {
	//ブロックのサイズ算出 (tiles.cppから引用)
	double block_size = Min((double)(right_bottom.y - left_upper.y) / tiles.size(), (double)(right_bottom.x - left_upper.x) / tiles.width_size());

	Vec2 originPos = left_upper + (lastPosition + Vec2{ 0.5, 0.5 }) * block_size;
	Vec2 destinationPos = left_upper + (position + Vec2{ 0.5, 0.5 }) * block_size;
	Vec2 pathVector = destinationPos - originPos;
	Vec2 drawPos = originPos + pathVector * (1 - delayTimer.remaining() / delayTimer.duration());

	Circle{ drawPos, block_size * 0.25 }.draw(Palette::Blue);
}

void Player::draw(int x1, int y1, int x2, int y2) const {
	draw(Point{ x1, y1 }, Point{ x2, y2 });
}

Direction Player::move(Direction movingDirection) {
	// 移動前位置を記録
	lastPosition = position;

	// 進行方向
	Point deltaPos;
	if (movingDirection == Direction::Up) deltaPos = { 0, -1 };
	else if (movingDirection == Direction::Down) deltaPos = { 0, 1 };
	else if (movingDirection == Direction::Left) deltaPos = { -1, 0 };
	else if (movingDirection == Direction::Right) deltaPos = { 1, 0 };
	else return Direction::None;

	Point nextPos = position + deltaPos;

	// 盤上範囲内チェック
	if (nextPos.x < 0 || nextPos.y < 0 || nextPos.y >= tiles.size()) return Direction::None;
	if (nextPos.x >= tiles[nextPos.y].size()) return Direction::None;

	// タイルチェック
	if (tiles[nextPos.y][nextPos.x] == Tiles::Kind::Wall) {
		// 壁だった場合
		// 移動せず終了
		return Direction::None;
	}
	else if (tiles[nextPos.y][nextPos.x] == Tiles::Kind::Box) {
		if (not tiles.moveBox(nextPos.x, nextPos.y, movingDirection)) {
			return Direction::None;
		}
	}
	else if (tiles[nextPos.y][nextPos.x] == Tiles::Kind::Target) {
		// ターゲットだった場合
		// ターゲットを破壊して進む
		tiles.breakTarget(nextPos);
		if (tiles.getTargetNum() == 0) gameClearFlag = true;
	}

	// 移動確定
	position = nextPos;

	return movingDirection;
}

size_t Player::get_walk_count() const {
	return walk_count;
}

bool Player::isGameCleared() const {
	return gameClearFlag;
}
