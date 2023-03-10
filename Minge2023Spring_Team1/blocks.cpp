#include"common.hpp"
#include"StageClass.hpp"

Array<Blocks::Kind>& Blocks::operator[](size_t y) {
	return blocks[y];
}

// ブロックの追加
void Blocks::operator<<(Array<Kind> row) {
	blocks.operator<<(row);
}

size_t Blocks::size() {
	return blocks.size();
}

size_t Blocks::width_size() {
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
	// todoo
}

void Blocks::draw(int x1, int y1, int x2, int y2) const {
	draw(Point(x1, y1), Point(x2, y2));
}
