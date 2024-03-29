﻿#include"common.hpp"
#include"StageClass.hpp"

Tiles::Tiles(int stage_number)
	:stage_number{ stage_number }
{
}

void Tiles::setStageNumber(int n) {
	stage_number = n;
}

Array<Tiles::Kind>& Tiles::operator[](size_t y) {
	return tiles[y];
}

// ブロックの追加
void Tiles::operator<<(Array<Kind> row) {
	tiles.operator<<(row);
}

size_t Tiles::size() const {
	return tiles.size();
}

size_t Tiles::width_size() const {
	size_t m = 0;
	for (auto& row : tiles) {
		m = Max(row.size(), m);
	}
	return m;
}

Array<Array<Tiles::Kind>>::iterator Tiles::begin() {
	return tiles.begin();
}

Array<Array<Tiles::Kind>>::iterator Tiles::end() {
	return tiles.end();
}

// ブロックの描画
void Tiles::draw(Point left_upper, Point right_bottom) const {
	// ドット絵をクリアに表示させる
	const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

	//ブロックのサイズ算出
	double block_size = Min((double)(right_bottom.y - left_upper.y) / size(), (double)(right_bottom.x - left_upper.x) / width_size());

	// タイルの描画
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			// マスの作成
			RectF box((Vec2)left_upper + Vec2(j * block_size, i * block_size), block_size);

			// マスの種類によって描画を変える
			switch (tiles[i][j]) {
			case Kind::None:
				drawNone(box, j, i);
				break;
			case Kind::Wall:
				box.draw(Palette::Brown);
				break;
			case Kind::Target:
				drawNone(box, j, i);
				box(target_tile_texture).draw();
				//Circle(box.pos + Vec2(block_size / 2, block_size / 2), block_size / 4).draw(Palette::White);
				break;
			case Tiles::Kind::Box:
				drawNone(box, j, i);
				if (Point{ j, i } != animTarget) box(box_tile_texture).draw();
				break;
			case Kind::ReflectiveWallL:
				drawNone(box, j, i);
				box(reflectL_tile_texture).draw();
				break;
			case Kind::ReflectiveWallR:
				drawNone(box, j, i);
				box(reflectR_tile_texture).draw();
				break;
			case Kind::Rock3:
				drawNone(box, j, i);
				if (Point{ j, i } != animTarget) box(rock_tile_texture[0]).draw();
				break;
			case Kind::Rock2:
				drawNone(box, j, i);
				if (Point{ j, i } != animTarget) box(rock_tile_texture[1]).draw();
				break;
			case Kind::Rock1:
				drawNone(box, j, i);
				if (Point{ j, i } != animTarget) box(rock_tile_texture[2]).draw();
        break;
			case Tiles::Kind::WarpHole:
				drawNone(box, j, i);
				Circle(box.pos + box.size / 2, box.size.x / 4).draw(Palette::Purple);
				Circle(box.pos + box.size / 2, box.size.x / 6).draw(Palette::Mediumpurple);
				break;
			}

			// マスのフレームを描画
			box.drawFrame(3, 3, Palette::Green);
		}
	}

	// アニメーション処理
	if (animTimer.isRunning()) {
		RectF box((Vec2)left_upper + Vec2(animTarget.x * block_size, animTarget.y * block_size), block_size);
		switch (tiles[animTarget.y][animTarget.x]) {
		case Tiles::Kind::Box:
			getAnimBox(box, block_size)(box_tile_texture).draw();
			break;
		case Tiles::Kind::Rock3:
			getAnimBox(box, block_size)(rock_tile_texture[0]).draw();
			break;
		case Tiles::Kind::Rock2:
			getAnimBox(box, block_size)(rock_tile_texture[1]).draw();
			break;
		case Tiles::Kind::Rock1:
			getAnimBox(box, block_size)(rock_tile_texture[2]).draw();
			break;
		}
	}
}

void Tiles::draw(int x1, int y1, int x2, int y2) const {
	draw(Point(x1, y1), Point(x2, y2));
}

void Tiles::drawNone(RectF box, int x, int y) const {
	Texture tile_image;

	// xとyとステージ番号からハッシュ値を取る
	int hash = ((Hash::XXHash3(x) >> 1) xor Hash::XXHash3(y) xor Hash::XXHash3(stage_number)) % 6;
	switch (hash)
	{
	case 0:
		tile_image = none_tile_texture[1];
		break;
	case 1:
		tile_image = none_tile_texture[2];
		break;
	default:
		tile_image = none_tile_texture[0];
		break;
	}

	// 拡大率を計算する
	const double scaleX = static_cast<double>(box.w) / tile_image.width();
	const double scaleY = static_cast<double>(box.h) / tile_image.height();

	// 描画
	tile_image.scaled(scaleX, scaleY).draw(box.pos);
}

int32 Tiles::getTargetNum() const {
	int32 targetNum = 0;
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j] == Tiles::Kind::Target) targetNum++;
		}
	}
	return targetNum;
}

Point Tiles::getAnotherWarpHolePos(Point position) {
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			if (tiles[i][j] == Tiles::Kind::WarpHole && Point{ j, i } != position) return Point{ j, i };
		}
	}

	throw Error{ U"ワープホールが見つかりませんでした。ワープホールが2つ設置されていない可能性があります。" };
}

bool Tiles::breakTarget(Point position) {
	if (tiles[position.y][position.x] == Tiles::Kind::Target) {
		tiles[position.y][position.x] = Tiles::Kind::None;
		// ターゲットの破壊に成功
		eventQueue << GameEvent::BreakTarget;
		return true;
	}
	return false;
}

bool Tiles::moveBox(int x, int y, Direction direction) {
	// 走り出しの時に箱が隣接していなかった場合は、プレイヤーは止まる
	if (adjacent_flag == false) {
		return false;
	}

	if (tiles[y][x] != Kind::Box) { // 安全対策
		return true;
	}

	Point old_pos(x, y);
	Point new_pos(x, y);
	switch (direction)
	{
	case Direction::Up:
		new_pos.y--;
		break;
	case Direction::Down:
		new_pos.y++;
		break;
	case Direction::Right:
		new_pos.x++;
		break;
	case Direction::Left:
		new_pos.x--;
		break;
	default:
		break;
	}

	// 現在の箱を消す
	tiles[y][x] = Kind::None;

	// 箱の新しい位置が壁か別の箱ならば、箱を消す
	if (0 > new_pos.x or new_pos.x >= width_size() or new_pos.y < 0 or new_pos.y >= size() or (tiles[new_pos.y][new_pos.x] != Kind::Target and tiles[new_pos.y][new_pos.x] != Kind::None)) {
		adjacent_flag = false;
		eventQueue << GameEvent::BreakBox;
		return true;
	}
	else if (tiles[new_pos.y][new_pos.x] == Kind::Target) {
		// 箱の移動先にターゲットがあったら破壊する
		breakTarget(new_pos);
	}

	// 新しい位置に箱を生やす
	tiles[new_pos.y][new_pos.x] = Kind::Box;
	invokeAnimation(new_pos, new_pos - old_pos);

	return true;
}

void Tiles::setAdjacentFlag(Point pos, Direction direction) {
	switch (direction)
	{
	case Direction::Up:
		pos.y--;
		break;
	case Direction::Down:
		pos.y++;
		break;
	case Direction::Right:
		pos.x++;
		break;
	case Direction::Left:
		pos.x--;
		break;
	}


	if (0 <= pos.y and pos.y < size() and 0 <= pos.x and pos.x < width_size() and (tiles[pos.y][pos.x] == Kind::Box or tiles[pos.y][pos.x] == Kind::Rock1 or tiles[pos.y][pos.x] == Kind::Rock2 or tiles[pos.y][pos.x] == Kind::Rock3)) {
		adjacent_flag = true;
	}
	else {
		adjacent_flag = false;
	}
}

bool Tiles::moveRock(int x, int y, Direction direction) {
	if (tiles[y][x] != Kind::Rock1 and tiles[y][x] != Kind::Rock2 and tiles[y][x] != Kind::Rock3) { // 安全対策
		return true;
	}

	// 走り出しの時に箱が隣接していなかった場合は、プレイヤーは止まる
	if (adjacent_flag == false) {
		switch (tiles[y][x])
		{
		case Tiles::Kind::Rock1:
			tiles[y][x] = Kind::None;
			return true;
			break;
		case Tiles::Kind::Rock2:
			tiles[y][x] = Kind::Rock1;
			return false;
			break;
		case Tiles::Kind::Rock3:
			tiles[y][x] = Kind::Rock2;
			return false;
			break;
		}
	}

	Point old_pos(x, y);
	Point new_pos(x, y);
	switch (direction)
	{
	case Direction::Up:
		new_pos.y--;
		break;
	case Direction::Down:
		new_pos.y++;
		break;
	case Direction::Right:
		new_pos.x++;
		break;
	case Direction::Left:
		new_pos.x--;
		break;
	default:
		break;
	}

	// 箱の新しい位置が壁か別の箱ならば、箱を消す
	if (0 > new_pos.x or new_pos.x >= width_size() or new_pos.y < 0 or new_pos.y >= size() or (tiles[new_pos.y][new_pos.x] != Kind::Target and tiles[new_pos.y][new_pos.x] != Kind::None)) {
		return false;
	}


	// 現在の岩の段階を取得
	Kind current_kind = tiles[y][x];

	// 現在の箱を消す
	tiles[y][x] = Kind::None;

	// 新しい位置に岩を生やす
	tiles[new_pos.y][new_pos.x] = current_kind;

	invokeAnimation(new_pos, new_pos - old_pos);

	return true;
}

GameEvent Tiles::popEventQueue() {
	if (eventQueue.isEmpty()) return GameEvent::None;
	else {
		GameEvent gameEvent = eventQueue.front();
		eventQueue.pop_front();
		return gameEvent;
	}
}

void Tiles::invokeAnimation(Point target, Point deltaPos) {
	animTarget = target;
	animDeltaPos = deltaPos;
	animTimer.restart();
}

RectF Tiles::getAnimBox(RectF destBox, double block_size) const {
	// アニメーション進捗度（1.0～0）
	double progress = animTimer.remaining() / animTimer.duration();

	return destBox.movedBy(Vec2{ block_size, block_size } *-(animDeltaPos * progress));
}
