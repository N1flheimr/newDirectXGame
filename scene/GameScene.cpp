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

	Matrix4 matScale[2];
	Matrix4 matRot[2];
	Matrix4 matTrans[2];

	worldTransform_[0].Initialize();
	worldTransform_[1].Initialize();
	worldTransform_[1].translation_ = { 0,4.5f,0 };
	worldTransform_[1].parent_ = &worldTransform_[0];

	matScale[0].Identity();
	matScale[0].Scale(worldTransform_[0].scale_);

	matRot[0].Identity();
	matRot[0].Rotation(worldTransform_[0].rotation_);

	matTrans[0].Identity();
	matTrans[0].Transform(worldTransform_[0].translation_);

	matScale[1].Identity();
	matScale[1].Scale(worldTransform_[1].scale_);

	matRot[1].Identity();
	matRot[1].Rotation(worldTransform_[1].rotation_);

	matTrans[1].Identity();
	matTrans[1].Transform(worldTransform_[1].translation_);

	worldTransform_[0].matWorld_ *= matScale[0];
	worldTransform_[0].matWorld_ *= matRot[0];
	worldTransform_[0].matWorld_ *= matTrans[0];

	worldTransform_[1].matWorld_ *= matScale[1];
	worldTransform_[1].matWorld_ *= matRot[1];
	worldTransform_[1].matWorld_ *= matTrans[1];

	worldTransform_[0].TransferMatrix();
	worldTransform_[1].TransferMatrix();
}

void GameScene::Update() {
	debugCamera_->Update();
	Vector3 move = move.Zero();
	Vector3 moveKeyInput;
	const float moveSpeed = 0.01f;

	moveKeyInput.x += input_->PushKey(DIK_D) - input_->PushKey(DIK_A);
	moveKeyInput.z += input_->PushKey(DIK_W) - input_->PushKey(DIK_S);

	move += moveKeyInput * moveSpeed;

	worldTransform_[0].translation_ += move;
	worldTransform_[0].TransferMatrix();

	//デバッグ用
	debugText_->SetPos(30, 30);
	debugText_->Printf("Translation: (%f, %f, %f)",
		worldTransform_[0].translation_.x,
		worldTransform_[0].translation_.y,
		worldTransform_[0].translation_.z);
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
	model_->Draw(worldTransform_[0], debugCamera_->GetViewProjection(), textureHandle_);
	model_->Draw(worldTransform_[1], debugCamera_->GetViewProjection(), textureHandle_);
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
