#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Global.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#define PI 3.1415

GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; delete debugCamera_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugCamera_ = new DebugCamera(WIN_WIDTH, WIN_HEIGHT);
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());

	textureHandle_ = TextureManager::Load("boys.png");
	model_ = Model::Create();

	viewProjection_.Initialize();

	std::random_device seedGen;
	std::mt19937_64 engine(seedGen());
	std::uniform_real_distribution<float> distRange(-15, 15);
	std::uniform_real_distribution<float> scaleRange(1, 2);
	std::uniform_real_distribution<float> rotRange(0, 2 * PI);

	Matrix4 matScale;
	Matrix4 matRot;
	Matrix4 matTrans;
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 1.5f,1.5f,1.5f };
	worldTransform_.rotation_ = { rotRange(engine),rotRange(engine),rotRange(engine) };
	worldTransform_.translation_ = { 0,0,0 };
	worldTransform_.matWorld_.Identity();

	matScale.Identity();
	matScale.Scale(worldTransform_.scale_);

	matRot.Identity();
	matRot.Rotation(worldTransform_.rotation_);

	matTrans.Identity();
	matTrans.Transform(worldTransform_.translation_);

	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRot;
	worldTransform_.matWorld_ *= matTrans;

	worldTransform_.TransferMatrix();
}

void GameScene::Update() {
	debugCamera_->Update();
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
	//model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);
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
