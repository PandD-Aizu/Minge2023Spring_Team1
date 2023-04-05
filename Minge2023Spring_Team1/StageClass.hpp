#pragma once
#include"common.hpp"

class Tiles {
public:
	// @param stage_number ステージ番号
	Tiles(int stage_number=0);

	// @param stage_number ステージ番号
	void setStageNumber(int);

	// マスの種類
	enum class Kind {
		None,
		Wall,
		Target,
	};

	Array<Kind>& operator[](size_t y);
	void operator<<(Array<Kind> row);

	// @brief マスの縦方向のサイズ
	size_t size() const;
	// @brief マスの横方向のサイズを返す
	size_t width_size() const;

	Array<Array<Kind>>::iterator begin();
	Array<Array<Kind>>::iterator end();

	// @brief マスの描画
	// @param left_upper,right_bottom 描画できる範囲の左上の点と右下の点
	void draw(Point, Point) const;
	// @brief マスの描画
	// @param left,upper,right,bottom 描画できる範囲の上下左右
	void draw(int, int, int, int) const;

	// @brief 残っているターゲットの数を返す
	int32 getTargetNum() const;

	// @brief 指定位置にあるターゲットを破壊する
	// @param position 対象ターゲットの位置
	// @return 正常に破壊できた場合trueを返す
	bool breakTarget(Point position);

private:
	Array<Array<Kind>> tiles;

	// @breif 何もないフィールドの描画
	void drawNone(RectF,int,int) const;

	// ステージ番号
	int stage_number=0;

	const Array<Texture> none_tile_texture{
		Texture{U"sprites/grass.png"},
		Texture{U"sprites/grass_flower.png"},
		Texture{U"sprites/grass_rock.png" },
	};
};

// プレイヤー
class Player {
public:
	// 向き
	enum class Direction {
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,
		None
	};
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
	Direction direction = Direction::Down;
	// 描画上の向いている方向
	Direction directionForDraw = Direction::Down;
	// 行動遅延タイマー（アニメーション用のタイマー）
	Timer delayTimer{ 0.1s };
	// ダッシュ中true
	bool dashFlag = false;

	// 歩行回数
	size_t walk_count=0;

	// ゲームクリア時にtrue
	bool gameClearFlag = false;


	// テクスチャ
	Array<Texture> textures;
};
