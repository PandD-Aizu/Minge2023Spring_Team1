#pragma once
# include <Siv3D.hpp>

// シーンの名前
enum class SceneList {
	Title,
	Stage,
	EndlessStage,
	StageSelect
};

// 共有するデータ
struct GameData
{
	//ステージセレクト→ゲームへのデータ
	//ステージのCSVのパス
	String StagePass;
	//ステージの番号
	int StageNo;
};

using App = SceneManager<SceneList,GameData>;

// キーコンフィグ
extern InputGroup inputUp; // 上入力グループ
extern InputGroup inputDown; // 下入力グループ
extern InputGroup inputLeft; // 左入力グループ
extern InputGroup inputRight; // 右入力グループ
