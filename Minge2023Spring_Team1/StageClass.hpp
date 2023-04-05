#pragma once
#include"common.hpp"

// 向き
enum class Direction {
	Up,
	Down,
	Left,
	Right,
	None
};

class Tiles {
public:
	// マスの種類
	enum class Kind {
		None,
		Wall,
		Target,
		Box,
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


	// @param 箱の移動方向
	// @return プレイヤーが移動可能かを返す
	bool moveBox(int, int, Direction);

	// @brief 箱とプレイヤーが隣接してたらフラグをtrueにする
	// @param プレイヤーの位置と移動方向
	void setAdjacentFlag(Point, Direction);

private:
	Array<Array<Kind>> tiles;

	// 箱とプレイヤーが隣接しているかのフラグ
	bool adjacent_flag = false;
};

// プレイヤー
class Player {
public:
	/**
	* @param tiles 盤面を参照するために必要
	* @param position 初期位置
	*/
	Player(Tiles& tiles, Point position);

	// 毎フレーム呼ぶ
	void update();

	// @brief 描画
	void draw(Point left_upper, Point right_bottom) const;
	// @brief 描画
	void draw(int, int, int, int) const;
	/**
	* @brief 移動する。次に移動すべき向きが戻り値で指定される。
	* @param direction 移動方向（上:0 下:1 左:2 右:3）
	* @return 次に移動する向き。壁があり移動できない場合はDirection::Noneが返される。
	*/
	Direction move(Direction direction);

	// @brief 一マス移動の回数のゲッター関数
	size_t get_walk_count() const;

	// @return ゲームクリア時true
	bool isGameCleared() const;

	const Duration WALKING_DELAY = 0.4s; // 歩行遅延
	const Duration DASHING_DELAY = 0.1s; // ダッシュ遅延

private:
	Tiles& tiles;

	// 盤上での位置
	Point position{ 0, 0 };
	// アニメーション開始時点での位置
	Point lastPosition{ 0, 0 };
	// 向いている方向
	Direction direction;
	// 行動遅延タイマー（アニメーション用のタイマー）
	Timer delayTimer{ 0.1s };
	// ダッシュ中true
	bool dashFlag = false;

	// 歩行回数
	size_t walk_count = 0;

	// ゲームクリア時にtrue
	bool gameClearFlag = false;
};
