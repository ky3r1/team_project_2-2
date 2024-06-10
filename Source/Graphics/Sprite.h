#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <cstring>
#include <string>

// スプライト
class Sprite
{
public:
	Sprite();
	Sprite(const char* filename);
	~Sprite() {}

	struct Vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT4	color;
		DirectX::XMFLOAT2	texcoord;
	};

	// 描画実行
	void Render(ID3D11DeviceContext *dc,
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// 描画実行
	void Render(ID3D11DeviceContext* dc,
		DirectX::XMFLOAT2 pos,
		DirectX::XMFLOAT2 endpos,
		DirectX::XMFLOAT2 size,
		DirectX::XMFLOAT2 endsize,
		float angle,
		DirectX::XMFLOAT4 color ) const;

	void textout(ID3D11DeviceContext* immediate_context, std::string s,
		float x, float y, float w, float h, float color[]);

	// テクスチャ幅取得
	int GetTextureWidth() const { return textureWidth; }

	// テクスチャ高さ取得
	int GetTextureHeight() const { return textureHeight; }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>				vertexBuffer;

	Microsoft::WRL::ComPtr<ID3D11BlendState>			blendState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>		rasterizerState;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>		depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>			samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	shaderResourceView;
	D3D11_TEXTURE2D_DESC texture2d_desc;

	int textureWidth = 0;
	int textureHeight = 0;
};