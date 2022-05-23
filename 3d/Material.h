﻿#pragma once

#include "Vector3.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
#include <wrl.h>

/// <summary>
/// マテリアル
/// </summary>
class Material {
  public: // サブクラス
	// 定数バッファ用データ構造体
	struct ConstBufferData {
		Vector3 ambient;  // アンビエント係数
		float pad1;        // パディング
		Vector3 diffuse;  // ディフューズ係数
		float pad2;        // パディング
		Vector3 specular; // スペキュラー係数
		float alpha;       // アルファ
	};

  public: // 静的メンバ関数
	/// <summary>
	/// マテリアル生成
	/// </summary>
	/// <returns>生成されたマテリアル</returns>
	static Material* Create();

  public:
	std::string name_;            // マテリアル名
	Vector3 ambient_;            // アンビエント影響度
	Vector3 diffuse_;            // ディフューズ影響度
	Vector3 specular_;           // スペキュラー影響度
	float alpha_;                 // アルファ
	std::string textureFilename_; // テクスチャファイル名

  public:
	/// <summary>
	/// 定数バッファの取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetConstantBuffer() { return constBuff_.Get(); }

	/// テクスチャ読み込み
	/// </summary>
	/// <param name="directoryPath">読み込みディレクトリパス</param>
	void LoadTexture(const std::string& directoryPath);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// グラフィックスコマンドのセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	void SetGraphicsCommand(
	  ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
	  UINT rooParameterIndexTexture);

	/// <summary>
	/// グラフィックスコマンドのセット（テクスチャ差し替え版）
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="rooParameterIndexMaterial">マテリアルのルートパラメータ番号</param>
	/// <param name="rooParameterIndexTexture">テクスチャのルートパラメータ番号</param>
	/// <param name="textureHandle">差し替えるテクスチャハンドル</param>
	void SetGraphicsCommand(
	  ID3D12GraphicsCommandList* commandList, UINT rooParameterIndexMaterial,
	  UINT rooParameterIndexTexture, uint32_t textureHandle);

	// テクスチャハンドル
	uint32_t GetTextureHadle() { return textureHandle_; }

  private:
	// 定数バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_;
	// 定数バッファのマップ
	ConstBufferData* constMap_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

  private:
	// コンストラクタ
	Material() {
		ambient_ = {0.3f, 0.3f, 0.3f};
		diffuse_ = {0.0f, 0.0f, 0.0f};
		specular_ = {0.0f, 0.0f, 0.0f};
		alpha_ = 1.0f;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 定数バッファの生成
	/// </summary>
	void CreateConstantBuffer();
};
