#pragma once
# include <Siv3D.hpp>

// シーンの名前
enum class SceneList {
	Title,
	Stage
};

// 共有するデータ
struct GameData
{
};

using App = SceneManager<SceneList,GameData>;

// キーコンフィグ
extern InputGroup inputUp; // 上入力グループ
extern InputGroup inputDown; // 下入力グループ
extern InputGroup inputLeft; // 左入力グループ
extern InputGroup inputRight; // 右入力グループ
