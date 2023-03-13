#include"common.hpp"
#include"Scene.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{
	button_startGame = RectF{
		Arg::center(Scene::Width() / 2, Scene::Height() * 0.6),
		Vec2{ Scene::Width() * 0.2, Scene::Height() * 0.05 }
	};
	button_quit = RectF{
		Arg::center(Scene::Width() / 2, Scene::Height() * 0.7),
		Vec2{ Scene::Width() * 0.2, Scene::Height() * 0.05 }
	};
}

// 更新関数
void Title::update()
{
	// ボタン選択
	if (inputUp.down() && cursorPosition != 0) {
		cursorPosition--;
		// cursorPosition = (cursorPosition + buttonNum) % buttonNum; // ボタン選択の上下ループ
	}
	if (inputDown.down() && cursorPosition != buttonNum - 1) {
		cursorPosition++;
		// cursorPosition %= buttonNum; // ボタン選択の上下ループ
	}

	if (button_startGame.mouseOver()) cursorPosition = 0;
	else if (button_quit.mouseOver()) cursorPosition = 1;

	// ボタン決定
	if ((KeyEnter.down() && cursorPosition == 0) || button_startGame.leftClicked()) changeScene(SceneList::StageSelect); // ステージセレクトへ遷移する
	if ((KeyEnter.down() && cursorPosition == 1) || button_quit.leftClicked()) System::Exit(); // ゲーム終了
}

// 描画関数
void Title::draw() const
{
	static const Font font(60);
	static const Font font_button(30);

	// タイトル名描画
	font(U"This is Title").drawAt(Scene::Center() - Vec2{ 0, Scene::Height() * 0.2 });

	// ボタン描画
	if (cursorPosition == 0) {
		// 選択されていた場合
		button_startGame.draw(Palette::Gray);
		button_startGame.drawFrame(3, Palette::Black);
	}
	else {
		// されていない場合
		button_startGame.draw();
	}
	font_button(U"ゲームスタート").drawAt(button_startGame.center(), Palette::Black);

	if (cursorPosition == 1) {
		// 選択されていた場合
		button_quit.draw(Palette::Gray);
		button_quit.drawFrame(3, Palette::Black);
	}
	else {
		// されていない場合
		button_quit.draw();
	}
	font_button(U"やめる").drawAt(button_quit.center(), Palette::Black);

}
