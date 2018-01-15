//-------------------------------------------------------
// �쐬�ҁF���@�~��
// �쐬���F2018.01.05
// ���e�@�F�A�C�e���x�[�X�N���X
//-------------------------------------------------------

#include "item_base.h"

using namespace Item;
using namespace Device;
using namespace Actor;

Item_Base::Item_Base(int times, int interval, int delay)
{
	m_renderer = GameDevice::GetInstance()->GetRenderer();
	m_input = GameDevice::GetInstance()->GetInput();
	
	m_times = times;
	m_interval = interval;
	m_delay = delay;
}

Item_Base::~Item_Base()
{
	
}