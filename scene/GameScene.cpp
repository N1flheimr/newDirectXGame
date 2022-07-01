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

#pragma region Initialize
	viewProjection_.Initialize();

	for (int i = 0; i < kNumPartID; i++) {
		worldTransform_[i].Initialize();
	}
#pragma endregion



#pragma region WTParentStructure
	worldTransform_[kSpine].parent_ = &worldTransform_[kRoot];
	worldTransform_[kSpine].translation_ = { 0,2.5f,0 };

	worldTransform_[kChest].parent_ = &worldTransform_[kSpine];

	worldTransform_[kHead].parent_ = &worldTransform_[kChest];
	worldTransform_[kHead].translation_ = { 0,2.5f,0 };

	worldTransform_[kArmL].parent_ = &worldTransform_[kChest];
	worldTransform_[kArmL].translation_ = { -2.5f,0,0 };

	worldTransform_[kArmR].parent_ = &worldTransform_[kChest];
	worldTransform_[kArmR].translation_ = { 2.5f,0,0 };

	worldTransform_[kHip].parent_ = &worldTransform_[kSpine];
	worldTransform_[kHip].translation_ = { 0,-2.5f,0 };

	worldTransform_[kLegL].parent_ = &worldTransform_[kHip];
	worldTransform_[kLegL].translation_ = { -2.5f,-2.5f,0 };

	worldTransform_[kLegR].parent_ = &worldTransform_[kHip];
	worldTransform_[kLegR].translation_ = { 2.5f,-2.5f,0 };

	for (int i = 0; i < kNumPartID; i++) {
		worldTransform_[i].Set();
		worldTransform_[i].TransferMatrix();
	}
#pragma endregion

}

void GameScene::Update() {
	debugCamera_->Update();
	Vector3 move = move.Zero();
	Vector3 moveKeyInput;
	const float moveSpeed = 0.125f;
	const float rotSpeed = 0.005f;

	moveKeyInput.x += input_->PushKey(DIK_D) - input_->PushKey(DIK_A);
	moveKeyInput.z += input_->PushKey(DIK_W) - input_->PushKey(DIK_S);

	move.x += moveKeyInput.x * moveSpeed;
	move.z += moveKeyInput.z * moveSpeed;

	worldTransform_[kRoot].translation_ += move;
	//if (input_->PushKey(DIK_Q)) {
	//	worldTransform_[kChest].rotation_ =
	//	{ worldTransform_[kChest].rotation_.x,
	//	worldTransform_[kChest].rotation_.y - rotSpeed,
	//	worldTransform_[kChest].rotation_.z };
	//	min(worldTransform_[kChest].rotation.y, PI * 2);
	//}

	if (input_->PushKey(DIK_SPACE)) {
		worldTransform_[kChest].scale_.x += 0.002f;
		worldTransform_[kChest].scale_.y += 0.002f;
		worldTransform_[kChest].scale_.z += 0.002f;
		worldTransform_[kHip].scale_.x += 0.002f;
		worldTransform_[kHip].scale_.y += 0.002f;
		worldTransform_[kHip].scale_.z += 0.002f;
		worldTransform_[kHead].scale_.x += 0.002f;
		worldTransform_[kHead].scale_.y += 0.002f;
		worldTransform_[kHead].scale_.z += 0.002f;
	}

	for (int i = 0; i < kNumPartID; i++) {
		worldTransform_[i].Set();
		//子の更新
		if (i != kRoot)
			worldTransform_[i].matWorld_ *= worldTransform_[kRoot].matWorld_;
		worldTransform_[i].TransferMatrix();
	}
	for (int i = kArmL; i <= kArmR; i++) {
		worldTransform_[i].Set();
		worldTransform_[i].matWorld_ *= worldTransform_[kChest].matWorld_;
		worldTransform_[i].TransferMatrix();
	}

	for (int i = kLegL; i <= kLegR; i++) {
		worldTransform_[i].Set();
		worldTransform_[i].matWorld_ *= worldTransform_[kHip].matWorld_;
		worldTransform_[i].TransferMatrix();
	}

	//デバッグ用
	debugText_->SetPos(30, 30);
	debugText_->Printf("WT[Root].Translation: (%f, %f, %f)",
		worldTransform_[kRoot].translation_.x,
		worldTransform_[kRoot].translation_.y,
		worldTransform_[kRoot].translation_.z);
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
	for (int i = 0; i < kNumPartID; i++) {
		if (i == kRoot || i == kSpine) {
			continue;
		}
		model_->Draw(worldTransform_[i], debugCamera_->GetViewProjection(), textureHandle_);
	}

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
