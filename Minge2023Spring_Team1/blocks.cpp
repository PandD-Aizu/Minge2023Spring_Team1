#include"common.hpp"
#include"StageClass.hpp"

Array<Blocks::Kind>& Blocks::operator[](size_t y) {
	return blocks[y];
}

// ブロックの追加
void Blocks::operator<<(Array<Kind> row) {
	blocks.operator<<(row);
}

size_t Blocks::size() const {
	return blocks.size();
}

size_t Blocks::width_size() const {
	size_t m = 0;
	for (auto& row : blocks) {
		m = Max(row.size(), m);
	}
	return m;
}

Array<Array<Blocks::Kind>>::iterator Blocks::begin() {
	return blocks.begin();
}

Array<Array<Blocks::Kind>>::iterator Blocks::end() {
	return blocks.end();
}

// ブロックの描画
void Blocks::draw(Point left_upper, Point right_bottom) const {

	//ブロックのサイズ算出
	double block_size = Min((double)(right_bottom.y - left_upper.y) / size(), (double)(right_bottom.x - left_upper.x) / width_size());

	// 仮の描画
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].size(); j++) {
			// マスの作成
			RectF box((Vec2)left_upper+Vec2(i*block_size,j*block_size),block_size);

			// マスの種類によって描画を変える
			switch (blocks[i][j]) {
			case Blocks::Kind::None:
				box.draw(Palette::White);
				break;
			case Blocks::Kind::Wall:
				box.draw(Palette::Brown);
				break;
			case Blocks::Kind::Target:
				Circle(box.pos + Vec2(block_size / 2, block_size / 2), block_size / 4).draw(Palette::White);
				break;
			}

			// マスのフレームを描画
			box.drawFrame(3, 3, Palette::Green);
		}
	}
}

void Blocks::draw(int x1, int y1, int x2, int y2) const {
	draw(Point(x1, y1), Point(x2, y2));
}
