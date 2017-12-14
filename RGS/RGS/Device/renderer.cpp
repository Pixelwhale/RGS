//-------------------------------------------------------
// 作成者：林佳叡
// 作成日：2017.12.10
// 内容　：Render機能を整理したクラス
//-------------------------------------------------------
#include <Device\renderer.h>

using namespace Device;

Renderer::Renderer(std::shared_ptr<ContentManager> contents)
{
	m_contents = contents;
	m_depthSort = std::make_shared<DepthSort>();
	m_depthSort->Clear();
}

Renderer::Renderer(const Renderer&)
{
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	SetDrawScreen(DX_SCREEN_BACK);		//描画先をBackBufferに設定
	SetUseZBuffer3D(TRUE);				// Ｚバッファを有効にする
	SetWriteZBuffer3D(TRUE);			// Ｚバッファへの書き込みを有効にする
}

void Renderer::Release()
{
	m_contents->Release();						//Textureを解放処理
	m_contents = 0;

	m_depthSort->Clear();
	m_depthSort = 0;
}

void Renderer::Clear(int r, int g, int b)
{
	DxLib::SetBackgroundColor(r, g, b);			//ClearColor設定
	DxLib::ClearDrawScreen();					//画面クリア
	m_depthSort->Clear();
}

void Renderer::Swap()
{
	DxLib::ScreenFlip();						//BackBufferと交換
}

std::shared_ptr<DepthSort> Renderer::GetDepthSort()
{
	return m_depthSort;
}

#pragma region 2D Render関連

void Renderer::DrawTexture(std::string texture_name, Math::Vector2 position, float alpha)
{
	SetDrawBright((int)(255 * alpha), (int)(255 * alpha), (int)(255 * alpha));		//色設定
	DrawGraph((int)position.x, (int)position.y, m_contents->TextureHandle(texture_name), true);
	SetDrawBright(255, 255, 255);													//色を戻す
}

void Renderer::DrawTexture(
	std::string texture_name, Math::Vector2 position,
	Math::Vector2 pivot, Math::Vector2 scale,
	float angle, Color color, bool horizen_flip)
{
	color = color * color.A();						//Alpha適用
	SetDrawBright(color.r, color.g, color.b);		//色設定

	DrawRotaGraph3((int)position.x, (int)position.y, (int)pivot.x, (int)pivot.y,
		scale.x, scale.y, angle, m_contents->TextureHandle(texture_name), true, horizen_flip);

	SetDrawBright(255, 255, 255);					//色を戻す
}

void Renderer::DrawMotion(std::string texture_name, int index, Math::Vector2 position, float alpha)
{
	SetDrawBright((int)(255 * alpha), (int)(255 * alpha), (int)(255 * alpha));		//色設定
	DrawGraph((int)position.x, (int)position.y, m_contents->MotionHandle(texture_name, index), true);
	SetDrawBright(255, 255, 255);													//色を戻す
}

#pragma endregion

#pragma region 3D Render関連

void Renderer::DrawTexture3D(std::string texture_name, Math::Vector3 position,
	float scale, float alpha)
{
	SetDrawBright((int)(255 * alpha), (int)(255 * alpha), (int)(255 * alpha));		//色設定

	DrawBillboard3D(
		VGet(position.x, position.y, position.z),	//位置
		0.5f, 0.5f,									//Pivotポイント
		scale, 0.0,									//大きさ、回転
		m_contents->TextureHandle(texture_name),		//Tetxure Handle
		true, false);								//alpha使用, 水平反転

	SetDrawBright(255, 255, 255);					//色を戻す
}

void Renderer::DrawTexture3D(
	std::string texture_name, Math::Vector3 position,
	float scale, float angle, Color color, bool horizen_flip)
{
	color = color * color.A();						//Alpha適用
	SetDrawBright(color.r, color.g, color.b);		//色設定

	DrawBillboard3D(
		VGet(position.x, position.y, position.z),	//位置
		0.5f, 0.5f,									//Pivotポイント
		scale, angle,								//大きさ、回転
		m_contents->TextureHandle(texture_name),		//Tetxure Handle
		true, horizen_flip);							//alpha使用, 水平反転

	SetDrawBright(255, 255, 255);					//色を戻す
}

void Renderer::DrawMotion3D(std::string texture_name, int index,
	Math::Vector3 position, float scale, float alpha)
{
	SetDrawBright((int)(255 * alpha), (int)(255 * alpha), (int)(255 * alpha));		//色設定

	DrawBillboard3D(
		VGet(position.x, position.y, position.z),		//位置
		0.5f, 0.5f,										//Pivotポイント
		scale, 0.0f,									//大きさ、回転
		m_contents->MotionHandle(texture_name, index),	//Tetxure Handle
		true, false);									//alpha使用, 水平反転

	SetDrawBright(255, 255, 255);						//色を戻す
}

void Renderer::DrawMotion3D(std::string texture_name, int index,
	Math::Vector3 position, float scale, float angle, Color color)
{
	SetDrawBright(
		(int)(color.r * color.A()),
		(int)(color.g * color.A()),
		(int)(color.b * color.A()));					//色設定

	DrawBillboard3D(
		VGet(position.x, position.y, position.z),		//位置
		0.5f, 0.5f,										//Pivotポイント
		scale, angle,									//大きさ、回転
		m_contents->MotionHandle(texture_name, index),	//Tetxure Handle
		true, false);									//alpha使用, 水平反転

	SetDrawBright(255, 255, 255);						//色を戻す
}

void Renderer::DrawTransparentObj()
{
	std::vector<TransparentObj> draw_list = m_depthSort->DrawList();

	for (auto& obj : draw_list)
	{
		if (obj.index == -1)
		{
			DrawTexture3D(
				obj.texture_name,
				obj.position,
				obj.scale,
				obj.angle,
				obj.color);
			continue;
		}

		DrawMotion3D(
			obj.texture_name,
			obj.index,
			obj.position,
			obj.scale,
			obj.angle,
			obj.color);
	}
}

#pragma endregion

#pragma region 文字関連

void Renderer::DrawString(
	std::string text, std::string font_name, Math::Vector2 position, Color color,
	bool center)
{
	color = color * color.A();						//Alpha適用
	int handle = m_contents->FontHandle(font_name);
	if (center)
	{
		int xSize = GetDrawStringWidthToHandle(
			text.c_str(), text.length(), handle);
		position.x -= (int)(xSize / 2.0f);
	}

	DrawStringToHandle(
		(int)position.x, (int)position.y,						//位置
		text.c_str(),								//文字
		GetColor(color.r, color.g, color.b),		//色
		handle);									//使用Font
}

void Renderer::DrawString(
	std::string text, Math::Vector2 position, Color color,
	bool center)
{
	color = color * color.A();						//Alpha適用
	int handle = m_contents->FontHandle("Arial");
	if (center)
	{
		int xSize = GetDrawStringWidthToHandle(
			text.c_str(), text.length(), handle);
		position.x -= (int)(xSize / 2.0f);
	}

	DrawStringToHandle(
		(int)position.x, (int)position.y,						//位置
		text.c_str(),								//文字
		GetColor(color.r, color.g, color.b),		//色
		handle);									//使用Font
}

void Renderer::DrawString(
	std::string text, Math::Vector2 position, bool center)
{
	int handle = m_contents->FontHandle("Arial");
	if (center)
	{
		int xSize = GetDrawStringWidthToHandle(
			text.c_str(), text.length(), handle);
		position.x -= (int)(xSize / 2.0f);
	}

	DrawStringToHandle(
		(int)position.x, (int)position.y,		//位置
		text.c_str(),				//文字
		GetColor(0, 0, 0),			//色
		handle);					//使用Font
}

#pragma endregion


