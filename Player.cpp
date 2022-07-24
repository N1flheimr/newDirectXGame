#include "Player.h"
#include <cassert>
#include "Vector3.h"

void Player::Initialize()
{

}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	this->model_ = model;
	this->textureHandle_ = textureHandle;

	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	Vector3 moveInput;

	moveInput =
	{
		(float)(input_->PushKey(DIK_D) - input_->PushKey(DIK_A)) * 0.15f,
		(float)(input_->PushKey(DIK_W) - input_->PushKey(DIK_S)) * 0.15f,
		0.f
	};

	const float kMoveLimitX = 28.25f;
	const float kMoveLimitY = 16.25f;

	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, kMoveLimitY);

	worldTransform_.translation_ += moveInput;

	worldTransform_.Set();

	worldTransform_.TransferMatrix();

	//Debug
	debugText_->SetPos(30, 30);
	debugText_->Printf("pos.x,y,z: %f, %f, %f",
		worldTransform_.translation_.x,
		worldTransform_.translation_.y,
		worldTransform_.translation_.z);
}

void Player::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}