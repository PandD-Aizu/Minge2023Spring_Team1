#pragma once
#include"common.hpp"
class Blocks {
public:
	// マスの種類
	enum class Kind {
		None,
		Wall,
		Target,
	};
	Array<Kind>& operator[](size_t y);
	void operator<<(Array<Kind> row);

	size_t size() const;
	size_t width_size() const;

	Array<Array<Kind>>::iterator begin();
	Array<Array<Kind>>::iterator end();

	void draw(Point, Point) const;
	void draw(int, int, int, int) const;

private:
	Array<Array<Kind>> blocks;
};
