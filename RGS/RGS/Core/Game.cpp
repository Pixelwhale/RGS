//-------------------------------------------------------
// �쐬�ҁF�щ��b
// �쐬���F2017.12.04
// ���e�@�FGame�̊�{����
//-------------------------------------------------------
#include <Core\Game.h>

using namespace Core;

//����������
void Game::Initialize()
{
	m_endFlag = false;
}

//���[�h�R���e���c
void Game::Load()
{
}

//�R���e���c���
void Game::Unload()
{
}

//�X�V����
void Game::Update()
{
	if (m_InputState->IsKeyTrigger(DIK_ESCAPE))
		m_endFlag = true;
}

//�`�揈��
void Game::Draw()
{
}

//�I����Ă��邩
bool Game::IsEnd()
{
	return m_endFlag;
}