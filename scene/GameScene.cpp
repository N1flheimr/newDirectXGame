#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "Global.h"
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <random>
#define PI 3.1415

using namespace MathUtility;
GameScene::GameScene() {}

GameScene::~GameScene() { delete model_; }

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	textureHandle_ = TextureManager::Load("boys.png");
	model_ = Model::Create();

	viewProjection_.fovAngleY = DegreeToRad(10.f);
	//viewProjection_.aspectRatio = 1.f;
	viewProjection_.nearZ = 52.f;
	viewProjection_.farZ = 53.f;
	viewProjection_.Initialize();

	std::random_device seedGen;
	std::mt19937_64 engine(seedGen());
	std::uniform_real_distribution<float> distRange(-12, 12);
	std::uniform_real_distribution<float> scaleRange(1, 2);
	std::uniform_real_distribution<float> rotRange(0, 2 * PI);

	Matrix4 matScale[_countof(worldTransform_)];
	Matrix4 matRot[_countof(worldTransform_)];
	Matrix4 matTrans[_countof(worldTransform_)];
	for (int i = 0; i < _countof(worldTransform_); i++) {
		worldTransform_[i].scale_ = { 1.2f,1.2f,1.2f };
		worldTransform_[i].rotation_ = { rotRange(engine),rotRange(engine),rotRange(engine) };
		worldTransform_[i].translation_ = { distRange(engine),distRange(engine),distRange(engine) };
		worldTransform_[i].matWorld_.Identity();
		worldTransform_[i].Initialize();

		matScale[i].Identity();
		matScale[i].Scale(worldTransform_[i].scale_);

		matRot[i].Identity();
		matRot[i].Rotation(worldTransform_[i].rotation_);

		matTrans[i].Identity();
		matTrans[i].Transform(worldTransform_[i].translation_);

		worldTransform_[i].matWorld_ *= matScale[i];
		worldTransform_[i].matWorld_ *= matRot[i];
		worldTransform_[i].matWorld_ *= matTrans[i];

		worldTransform_[i].TransferMatrix();
	}
}

void GameScene::Update() {
	//Vector3 move = move.Zero();
	//Vector3 eyeMoveInput;
	//Vector3 targetMoveInput;
	//const float kEyeSpeed = 0.2f;
	//const float kTargetSpeed = 0.1f;
	//const float kUpRotSpeed = 0.05f;

	//eyeMoveInput.z += input_->PushKey(DIK_W) - input_->PushKey(DIK_S);
	//eyeMoveInput.x += input_->PushKey(DIK_D) - input_->PushKey(DIK_A);
	//targetMoveInput.x += input_->PushKey(DIK_RIGHT) - input_->PushKey(DIK_LEFT);
	//targetMoveInput.y += input_->PushKey(DIK_UP) - input_->PushKey(DIK_DOWN);

	//viewAngle += input_->PushKey(DIK_SPACE) * kUpRotSpeed;
	//viewAngle = fmodf(viewAngle, PI * 2.f);

	//move.z += eyeMoveInput.z * kEyeSpeed;
	//move.x += targetMoveInput.x * kTargetSpeed;

	//viewProjection_.target.x += move.x;
	//viewProjection_.eye.z += move.z;
	//viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0 };
	//viewProjection_.UpdateMatrix();

	if (input_->PushKey(DIK_UP)) {
		viewProjection_.fovAngleY += 0.0075f;
		viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, PI);
	}
	else if (input_->PushKey(DIK_DOWN)) {
		viewProjection_.fovAngleY -= 0.0075f;
		viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
	}

	if (input_->PushKey(DIK_W)) {
		viewProjection_.nearZ += 0.02f;
		viewProjection_.nearZ = min(viewProjection_.nearZ, viewProjection_.farZ - 0.5f);
	}
	else if (input_->PushKey(DIK_S)) {
		viewProjection_.nearZ -= 0.02f;
		viewProjection_.nearZ = max(viewProjection_.nearZ, -0.1f);
	}

	viewProjection_.UpdateMatrix();

	//デバッグ用
	debugText_->SetPos(30, 30);
	debugText_->Printf("fovAngleY(Degree): %f",
		RadToDegree(viewProjection_.fovAngleY));
	debugText_->SetPos(30, 50);
	debugText_->Printf("nearZ: %f",
		viewProjection_.nearZ);
	debugText_->SetPos(30, 70);
	debugText_->Printf("farZ: %f",
		viewProjection_.farZ);
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
	for (int i = 0; i < _countof(worldTransform_); i++) {
		model_->Draw(worldTransform_[i], viewProjection_, textureHandle_);
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
