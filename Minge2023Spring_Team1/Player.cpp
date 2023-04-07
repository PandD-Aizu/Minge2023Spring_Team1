#include "StageClass.hpp"

Player::Player(Tiles &tiles, Point position)
	: tiles{ tiles }, position{ position }
{
	delayTimer.setRemaining(0s);
	delayTimer.start();

	// テクスチャ読み込み
	for (auto c : Array<char>{ 'b', 'f', 'l', 'r'}) {
		for (int n = 0; n < 3; n++) {
			textures << Texture{ U"sprites/player_{}{}.png"_fmt(c, n) };
		}
	}
}

void Player::update() {
	if (delayTimer.reachedZero()) {
		// 移動処理が終わったら描画上の向きも更新
		directionForDraw = direction;

		if (!dashFlag && !autoWalkFlag) {
			// ダッシュ、自動歩行中でない場合
			// 方向入力受付
			if (inputUp.pressed()) direction = Direction::Up;
			else if (inputDown.pressed()) direction = Direction::Down;
			else if (inputLeft.pressed()) direction = Direction::Left;
			else if (inputRight.pressed()) direction = Direction::Right;
			else return; // 移動キーを押さなかった場合はここで処理終了

			// 押下キーに合わせて描画上の向きも更新
			directionForDraw = direction;

			// ↓↓↓方向キーを押した場合の処理↓↓↓
			tiles.setAdjacentFlag(position, direction); // プレイヤーと箱が隣接しているのかの判定

			if (KeyShift.pressed()) {
				// ダッシュ移動開始
				dashFlag = true;
			}
			else {
				// 通常歩行処理
				MoveStatus moveStatus = move(direction);
				if (moveStatus != MoveStatus::Failed) {
					if (moveStatus == MoveStatus::AutoWalk) autoWalkFlag = true;
					// 進行方向に壁がなく移動に成功した場合
					walk_count++;
					// 行動遅延設定
					delayTimer.set(WALKING_DELAY);
					delayTimer.start();
				}
			}

			return;
			// ↑↑↑方向キーを押した場合の処理↑↑↑　ここまで
		} else if (dashFlag) {
			// ダッシュ移動中の場合

			// そのまま移動する
			MoveStatus moveStatus = move(direction);

			if (moveStatus != MoveStatus::Failed) {
				// 移動できた場合
				// 行動遅延設定
				delayTimer.set(DASHING_DELAY);
				delayTimer.start();
			}
			else {
				// 移動できなかった場合、ダッシュ移動を終了する
				dashFlag = false;
			}
		}
		else if (autoWalkFlag) {
			// 自動歩行中の場合

			// そのまま移動する
			MoveStatus moveStatus = move(direction);

			autoWalkFlag = false;

			if (moveStatus != MoveStatus::Failed) {
				// 移動できた場合
				// 自動歩行継続判定
				if (moveStatus == MoveStatus::AutoWalk) autoWalkFlag = true;
				// 行動遅延設定
				delayTimer.set(WALKING_DELAY);
				delayTimer.start();
			}
			else {
				// 移動できなかった場合、自動歩行を終了する
				autoWalkFlag = false;
			}
		}
	}
}

void Player::draw(Point left_upper, Point right_bottom) const {
	//ブロックのサイズ算出 (tiles.cppから引用)
	double block_size = Min((double)(right_bottom.y - left_upper.y) / tiles.size(), (double)(right_bottom.x - left_upper.x) / tiles.width_size());

	// 移動元座標
	Vec2 originPos = left_upper + (lastPosition + Vec2{ 0.5, 0.5 }) * block_size;
	// 移動先座標
	Vec2 destinationPos = left_upper + (position + Vec2{ 0.5, 0.5 }) * block_size;
	// 移動道のりベクトル
	Vec2 pathVector = destinationPos - originPos;
	// 移動の進行度(0～1.0)
	double motionProgress = 1 - delayTimer.remaining() / delayTimer.duration();
	// 現在の表示座標
	Vec2 drawPos = originPos + pathVector * motionProgress;

	// 向いている方向 0:上 1:下 2:左 3:右
	int32 directionInt = static_cast<int>(directionForDraw != Direction::None ? directionForDraw : Direction::Up);
	// 描画
	RectF{ block_size }(textures[directionInt * 3 + static_cast<int>(motionProgress * 3) % 3]).drawAt(drawPos);
}

void Player::draw(int x1, int y1, int x2, int y2) const {
	draw(Point{ x1, y1 }, Point{ x2, y2 });
}

Player::MoveStatus Player::move(Direction &movingDirection) {
	// 戻り値
	MoveStatus moveStatus = MoveStatus::None;

	// 床がワープホールだった場合のワープ処理
	if (tiles[position.y][position.x] == Tiles::Kind::WarpHole) {
		position = tiles.getAnotherWarpHolePos(position);
	}

	// 移動前位置を記録
	lastPosition = position;

	// 進行方向
	Point deltaPos;
	if (movingDirection == Direction::Up) deltaPos = { 0, -1 };
	else if (movingDirection == Direction::Down) deltaPos = { 0, 1 };
	else if (movingDirection == Direction::Left) deltaPos = { -1, 0 };
	else if (movingDirection == Direction::Right) deltaPos = { 1, 0 };
	else return MoveStatus::Failed;

	Point nextPos = position + deltaPos;

	// 盤上範囲内チェック
	if (nextPos.x < 0 || nextPos.y < 0 || nextPos.y >= tiles.size()) return MoveStatus::Failed;
	if (nextPos.x >= tiles[nextPos.y].size()) return MoveStatus::Failed;

	// タイルチェック
	switch (tiles[nextPos.y][nextPos.x]) {
	case Tiles::Kind::Wall:
		// 壁だった場合
		// 移動せず終了
		return MoveStatus::Failed;
	case Tiles::Kind::Target:
		// ターゲットだった場合
		// ターゲットを破壊して進む
		tiles.breakTarget(nextPos);
		if (tiles.getTargetNum() == 0) gameClearFlag = true;
		break;
	case Tiles::Kind::Box:
		if (not tiles.moveBox(nextPos.x, nextPos.y, movingDirection)) {
			return MoveStatus::Failed;
		}
		break;
	case Tiles::Kind::ReflectiveWallL:
		// 斜め反射壁（＼）
		moveStatus = MoveStatus::AutoWalk; // 自動歩行
		// 移動方向変更
		switch (movingDirection) {
		case Direction::Up:
			movingDirection = Direction::Left;
			break;
		case Direction::Down:
			movingDirection = Direction::Right;
			break;
		case Direction::Left:
			movingDirection = Direction::Up;
			break;
		case Direction::Right:
			movingDirection = Direction::Down;
			break;
		}
		break;
	case Tiles::Kind::ReflectiveWallR:
		// 斜め反射壁（／）
		moveStatus = MoveStatus::AutoWalk; // 自動歩行
		// 移動方向変更
		switch (movingDirection) {
		case Direction::Up:
			movingDirection = Direction::Right;
			break;
		case Direction::Down:
			movingDirection = Direction::Left;
			break;
		case Direction::Left:
			movingDirection = Direction::Down;
			break;
		case Direction::Right:
			movingDirection = Direction::Up;
			break;
		}
		break;
	case Tiles::Kind::WarpHole:
		// ワープホール
		moveStatus = MoveStatus::AutoWalk; // 自動歩行
		break;
	}

	// 移動確定
	position = nextPos;

	return moveStatus;
}

size_t Player::get_walk_count() const {
	return walk_count;
}

bool Player::isGameCleared() const {
	return gameClearFlag;
}
