#pragma once
#include"common.hpp"
class Tiles {
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

	// @brief 残っているターゲットの数を返す
	int32 getTargetNum() const;

	// @brief 指定位置にあるターゲットを破壊する
	// @param position 対象ターゲットの位置
	// @return 正常に破壊できた場合trueを返す
	bool breakTarget(Point position);

private:
	Array<Array<Kind>> tiles;
};

// プレイヤー
class Player {
public:
	/**
	* @param tiles 盤面を参照するために必要
	* @param position 初期位置
	*/
	Player(Tiles &tiles, Point position);

	// 毎フレーム呼ぶ
	void update();

	// @brief 描画
	void draw(Point left_upper, Point right_bottom) const;
	// @brief 描画
	void draw(int, int, int, int) const;
	/**
	* @brief 移動する
	* @param direction 移動方向（上:0 下:1 左:2 右:3）
	* @param isDash ダッシュを有効にするか
	*/
	void move(int direction, bool isDash = false);
	// @return ゲームクリア時true
	bool isGameCleared() const;
private:
	Tiles& tiles;
	// 盤上での位置
	Point position{ 0, 0 };
	// ゲームクリア時にtrue
	bool gameClearFlag = false;
};
