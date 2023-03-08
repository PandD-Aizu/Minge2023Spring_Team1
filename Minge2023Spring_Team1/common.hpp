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
