#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Global.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#define PI 3.1415

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; delete debugCamera_; delete player_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugCamera_ = new DebugCamera(WIN_WIDTH, WIN_HEIGHT);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	textureHandle_ = TextureManager::Load("boys.png");
	model_ = Model::Create();

	player_ = new Player();
	player_->Initialize(model_, textureHandle_);

	viewProjection_.Initialize();

	std::random_device seedGen;
	std::mt19937_64 engine(seedGen());
	std::uniform_real_distribution<float> distRange(-15, 15);
	std::uniform_real_distribution<float> scaleRange(1, 2);
	std::uniform_real_distribution<float> rotRange(0, 2 * PI);
}

void GameScene::Update() {
	debugCamera_->Update();
	player_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif

	//if (isDebugCameraActive_) {
	//	debugCamera_->Update();
	//	viewProjection_.matView = debugCamera_;
	//	viewProjection_.matProjection = debugCamera_->GetViewProjection();
	//	viewProjection_.TransferMatrix();
	//}
	//else {
	//	viewProjection_
	//}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	for (int z = 0; z < 21; z++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			{ -30.f , 0, 30.f - (float)(3 * z) },
			{ 30.f , 0, 30.f - (float)(3 * z) },
			{ 255,255,255,1.f });
	}
	for (int x = 0; x < 21; x++) {
		PrimitiveDrawer::GetInstance()->DrawLine3d(
			{ -30.f + float(3 * x),0,-30 },
			{ -30.f + float(3 * x),0,30 },
			{ 255,255,255,1.f });
	}

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
