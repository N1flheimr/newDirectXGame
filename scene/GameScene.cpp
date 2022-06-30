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

	worldTransform_[kRoot].Initialize();

	worldTransform_[kSpine].Initialize();
	worldTransform_[kSpine].parent_ = &worldTransform_[kRoot];
	worldTransform_[kSpine].translation_ = { 0,2.5f,0 };

	worldTransform_[kChest].Initialize();
	worldTransform_[kChest].parent_ = &worldTransform_[kSpine];

	worldTransform_[kHead].Initialize();
	worldTransform_[kHead].parent_ = &worldTransform_[kChest];
	worldTransform_[kHead].translation_ = { 0,2.5f,0 };

	worldTransform_[kArmL].Initialize();
	worldTransform_[kArmL].parent_ = &worldTransform_[kChest];
	worldTransform_[kArmL].translation_ = { -2.5f,0,0 };

	worldTransform_[kArmR].Initialize();
	worldTransform_[kArmR].parent_ = &worldTransform_[kChest];
	worldTransform_[kArmR].translation_ = { 2.5f,0,0 };

	worldTransform_[kHip].Initialize();
	worldTransform_[kHip].parent_ = &worldTransform_[kSpine];
	worldTransform_[kHip].translation_ = { 0,-2.5f,0 };

	worldTransform_[kLegL].Initialize();
	worldTransform_[kLegL].parent_ = &worldTransform_[kHip];
	worldTransform_[kLegL].translation_ = { -2.5f,-2.5f,0 };

	worldTransform_[kLegR].Initialize();
	worldTransform_[kLegR].parent_ = &worldTransform_[kHip];
	worldTransform_[kLegL].translation_ = { 2.5f,-2.5f,0 };

	worldTransform_[kRoot].matWorld_.Rotation(worldTransform_[kRoot].rotation_);
	worldTransform_[kRoot].matWorld_.Scale(worldTransform_[kRoot].scale_);
	worldTransform_[kRoot].matWorld_.Transform(worldTransform_[kRoot].translation_);

	worldTransform_[kSpine].matWorld_.Rotation(worldTransform_[kSpine].rotation_);
	worldTransform_[kSpine].matWorld_.Scale(worldTransform_[kSpine].scale_);
	worldTransform_[kSpine].matWorld_.Transform(worldTransform_[kSpine].translation_);

	worldTransform_[kRoot].TransferMatrix();
	worldTransform_[kSpine].TransferMatrix();
}

void GameScene::Update() {
	debugCamera_->Update();
	Vector3 move = move.Zero();
	Vector3 moveKeyInput;
	const float moveSpeed = 0.125f;

	moveKeyInput.x += input_->PushKey(DIK_D) - input_->PushKey(DIK_A);
	moveKeyInput.z += input_->PushKey(DIK_W) - input_->PushKey(DIK_S);

	move.x += moveKeyInput.x * moveSpeed;
	move.z += moveKeyInput.z * moveSpeed;

	worldTransform_[kRoot].translation_ += move;
	//worldTransform_[kRoot].matWorld_.Transform(worldTransform_[0].translation_);
	//worldTransform_[kRoot].TransferMatrix();


	//子の更新
	worldTransform_[kSpine].matWorld_.Rotation(worldTransform_[kSpine].rotation_);
	worldTransform_[kSpine].matWorld_.Scale(worldTransform_[kSpine].scale_);
	worldTransform_[kSpine].matWorld_.Transform(worldTransform_[kSpine].translation_);
	worldTransform_[kSpine].matWorld_ *= worldTransform_[kRoot].matWorld_;
	worldTransform_[kSpine].TransferMatrix();

	//デバッグ用
	debugText_->SetPos(30, 30);
	debugText_->Printf("WT[0].Translation: (%f, %f, %f)",
		worldTransform_[kRoot].translation_.x,
		worldTransform_[kRoot].translation_.y,
		worldTransform_[kRoot].translation_.z);

	debugText_->SetPos(30, 50);
	debugText_->Printf("WT[1].Translation: (%f, %f, %f)",
		worldTransform_[kSpine].translation_.x,
		worldTransform_[kSpine].translation_.y,
		worldTransform_[kSpine].translation_.z);
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
