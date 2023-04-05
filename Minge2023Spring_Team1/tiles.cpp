#include"common.hpp"
#include"StageClass.hpp"

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

	//ブロックのサイズ算出
	double block_size = Min((double)(right_bottom.y - left_upper.y) / size(), (double)(right_bottom.x - left_upper.x) / width_size());

	// 仮の描画
	for (int i = 0; i < tiles.size(); i++) {
		for (int j = 0; j < tiles[i].size(); j++) {
			// マスの作成
			RectF box((Vec2)left_upper+Vec2(j*block_size,i*block_size),block_size);

			// マスの種類によって描画を変える
			switch (tiles[i][j]) {
			case Tiles::Kind::None:
				box.draw(Palette::White);
				break;
			case Tiles::Kind::Wall:
				box.draw(Palette::Brown);
				break;
			case Tiles::Kind::Target:
				Circle(box.pos + Vec2(block_size / 2, block_size / 2), block_size / 4).draw(Palette::White);
				break;
			case Tiles::Kind::Box:
				box.draw(Palette::Rosybrown);
				break;
			}

			// マスのフレームを描画
			box.drawFrame(3, 3, Palette::Green);
		}
	}
}

void Tiles::draw(int x1, int y1, int x2, int y2) const {
	draw(Point(x1, y1), Point(x2, y2));
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

bool Tiles::breakTarget(Point position) {
	if (tiles[position.y][position.x] == Tiles::Kind::Target) {
		tiles[position.y][position.x] = Tiles::Kind::None;
		return true;
	}
	return false;
}

bool Tiles::moveBox(int x, int y, Direction direction) {
	if (tiles[y][x] != Kind::Box) {
		return true;
	}
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

	tiles[y][x] = Kind::None;
	if (0>new_pos.x or new_pos.x >=width_size() or new_pos.y < 0 or new_pos.y >= size() or tiles[new_pos.y][new_pos.x] == Kind::Wall or tiles[new_pos.y][new_pos.x] == Kind::Box) {
		return true;
	}
	tiles[new_pos.y][new_pos.x] = Kind::Box;

	return true;
}
