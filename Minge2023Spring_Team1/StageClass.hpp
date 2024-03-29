﻿#pragma once
#include"common.hpp"

// 向き
enum class Direction {
	Up,
	Down,
	Left,
	Right,
	None
};

// ゲーム上のイベント
enum class GameEvent {
	BreakTarget,
	BreakBox,
	None
};

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
		Box,
		ReflectiveWallL,
		ReflectiveWallR,
		Rock1,
		Rock2,
		Rock3,
		WarpHole,
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

	// @brief もう一つのワープホールの位置を返す
	// @param position 検索から除外するワープホールの位置
	// @return ワープホールの位置
	Point getAnotherWarpHolePos(Point position);

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

	// @param 岩の移動方向
	// @return プレイヤーが移動可能かを返す
	bool moveRock(int, int, Direction);

	// @brief イベントのキューから一つ取り出す
	// @return イベント種別　なにも無かった場合、GameEvent::None
	GameEvent popEventQueue();

	/**
	* @brief アニメーションを起こす。target - deltaPosを起点、targetを終点としてアニメーションされる。
	* @param target アニメーション対象のタイル
	* @param deltaPos 移動ベクトル
	*/
	void invokeAnimation(Point target, Point deltaPos);

private:
	Array<Array<Kind>> tiles;

	// 箱とプレイヤーが隣接しているかのフラグ
	bool adjacent_flag = false;
	// @brief 何もないフィールドの描画
	void drawNone(RectF,int,int) const;

	// ステージ番号
	int stage_number=0;

	const Array<Texture> none_tile_texture{
		Texture{Resource(U"sprites/grass.png")},
		Texture{Resource(U"sprites/grass_flower.png")},
		Texture{Resource(U"sprites/grass_rock.png") },
	};

	// ターゲットのテクスチャ
	const Texture target_tile_texture{ Resource(U"sprites/target.png") };
	// 箱のテクスチャ
	const Texture box_tile_texture{ Resource(U"sprites/box.png") };
	// 反射壁のテクスチャ
	const Texture reflectL_tile_texture{ Resource(U"sprites/reflector_l.png") };
	const Texture reflectR_tile_texture{ Resource(U"sprites/reflector_r.png") };
	// 岩のテクスチャ
	const Array<Texture> rock_tile_texture{
		Texture{ Resource(U"sprites/big_rock_4.png") },
		Texture{ Resource(U"sprites/big_rock_5.png") },
		Texture{ Resource(U"sprites/big_rock_6.png") }
	};

	// 箱、ターゲットの破壊などイベント情報を保持するキュー
	Array<GameEvent> eventQueue;

	// === アニメーション関連 ===
	/**
	* @brief 現在の描画座標を取得する
	* @param destBox アニメーション終点位置の描画形状
	* @param block_size 1タイルあたりのサイズ
	*/
	RectF getAnimBox(RectF destBox, double block_size) const;
	// アニメーション対象座標
	Point animTarget;
	// アニメーション上の移動方向
	Point animDeltaPos;
	// タイマーが動いている間アニメーション
	Timer animTimer{ 0.1s };
	// ==========================
};

// プレイヤー
class Player {
public:
	enum class MoveStatus {
		None, // 正常に終了
		AutoWalk, // 自動的にもう一度移動する
		Failed // 移動不可
	};
	/**
	* @param tiles 盤面を参照するために必要
	* @param position 初期位置
	*/
	Player(Tiles& tiles, Point position);

	// @brief 毎フレーム呼ぶ
	void update();

	// @brief 描画
	void draw(Point left_upper, Point right_bottom) const;
	// @brief 描画
	void draw(int, int, int, int) const;
	/**
	* @brief 移動する。次に移動すべき向きが戻り値で指定される。
	* @param[in,out] direction 移動方向（上:0 下:1 左:2 右:3）
	* @return 処理結果 詳しくはMoveStatus参照
	*/
	MoveStatus move(Direction &direction);

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
	// 自動歩行中true
	bool autoWalkFlag = false;
	// ワープ可能な状態か（ワープ直後にfalse）
	bool warpIsEnabled = true;

	// 歩行回数
	size_t walk_count = 0;

	// ゲームクリア時にtrue
	bool gameClearFlag = false;

	// テクスチャ
	Array<Texture> textures;
};
