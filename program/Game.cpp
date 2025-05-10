#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Float2.h"

//	�m�o�b�̉�]�X�s�[�h
#define NPC_ROT_SPEED	1.0f
//	�v���C���[�Ƃm�o�b�̔��a�i�~�Ƃ��Ĕ��肷��̂Łj
#define CHARA_RADIUS	25.0f

//	�v���C���[�p�̕ϐ�
int player_image;		//	�摜
Float2 player_pos;		//	���W�i�w�Ƃx�̂Q�� float �^������\���́j
float player_rot;		//	����
//	�v���C���[�̈ړ���
Float2 player_mov;

//	�m�o�b�p�̕ϐ�
int npc_image;
Float2 npc_pos;
float npc_rot;

//	�}�E�X���W������p�̕ϐ��i�X�V�����ƕ`�揈���Ń}�E�X���W���g���̂ł����Ő錾���Ă��܂��j
Float2 mouse;

//	�}�E�X�h���b�O�����ǂ���
bool mouse_drag;

Float2 circle_pos;

//---------------------------------------------------------------------------------
//	����������
//---------------------------------------------------------------------------------
void GameInit()
{
	//	�摜�̓ǂݍ���
	player_image = LoadGraph( "data/player.bmp" );
	npc_image = LoadGraph( "data/npc.bmp" );
	//	�������W������̐ݒ�
	//	�Ƃ肠������ʂ̒����ɒu���Ă��܂�
	player_pos.x = SCREEN_W / 2;
	player_pos.y = SCREEN_H / 2;
	player_rot = 0.0f;

	npc_pos.x = (float)GetRand( SCREEN_W );
	npc_pos.y = (float)GetRand( SCREEN_H );
	npc_rot = 0.0f;
}
//---------------------------------------------------------------------------------
//	�X�V����
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	�}�E�X���W�̎擾
	mouse.x = (float)GetMouseX();
	mouse.y = (float)GetMouseY();

	//	�}�E�X���������u��
	if( PushMouseInput() ){
		//	�P�F�}�E�X�N���b�N�Ńv���C���[���}�E�X�̏���
		circle_pos.x = mouse.x;
		circle_pos.y = mouse.y;
	}

	//	�}�E�X�������Ă���Ƃ�
	if( CheckMouseInput() ){
		//	�}�E�X�������Ă���̂�
		mouse_drag = true;

		//	�Q�F�v���C���[�̌������}�E�X���W�̏���
		//	�v���C���[���猩�ă}�E�X���W���ǂꂾ���i�񂾂Ƃ���ɂ��邩
		float x = circle_pos.x - mouse.x;
		float y = circle_pos.y - mouse.y;

		player_pos.x = mouse.x;
		player_pos.y = mouse.y;

		//	��ŋ��߂��v���C���[����}�E�X�܂ł̒l���ړ��ʂƂ��Đݒ肵�܂�
		//	���̂܂܂��Ƒ傫������̂œK���ɏ��������܂��i 0.05 �{���܂��F�Q�O���̂P�j
		player_mov.x = x * 0.05f;
		player_mov.y = y * 0.05f;

		//	��ŋ��߂��v���C���[����}�E�X�܂ł̒l���g���Ċp�x�����߂�
//		float radian = GetRadian_LengthMukaiSotte( y, x );
		//	���������ŒZ������ player_mov �̒l���g���Ă������p�x�����炦�܂�
		float radian = GetRadian_LengthMukaiSotte( player_mov.y, player_mov.x );
		//	�v���C���[�̊p�x�ϐ��́u�x�v�Ȃ̂ŁA���W�A���p��x�ɕϊ����ēn���܂�
		player_rot = TO_DEGREE( radian );

	}else{
		//	����Ȃ��Ƃ��i�}�E�X�������Ă��Ȃ��Ƃ��j
		mouse_drag = false;

		//	�R�F�}�E�X�𗣂�����v���C���[�͈ړ��𑱂���
		player_pos.x += player_mov.x;
		player_pos.y += player_mov.y;
	}

	//	�m�o�b�̉�]
	npc_rot += NPC_ROT_SPEED;

	//	�v���C���[�Ƃm�o�b�œ����蔻��i�~���m�̓����蔻��FFloat2 �o�[�W�����j
	if( CheckCircleHit( player_pos, CHARA_RADIUS, npc_pos, CHARA_RADIUS ) ){
		//	����������m�o�b�̍��W�������_���ɐݒ�
		npc_pos.x = (float)GetRand( SCREEN_W );
		npc_pos.y = (float)GetRand( SCREEN_H );
	}
}
//---------------------------------------------------------------------------------
//	�`�揈��
//---------------------------------------------------------------------------------
void GameRender()
{
	//	�m�o�b�̕`��
	DrawRotaGraphF( npc_pos.x, npc_pos.y, 1.0f, TO_RADIAN( npc_rot ), npc_image, TRUE );
	//	�v���C���[�̕`��
	DrawRotaGraphF( player_pos.x, player_pos.y, 1.0f, TO_RADIAN( player_rot ), player_image, TRUE );

	//	�}�E�X�h���b�O���̎������`��
	if( mouse_drag ){
		//	�v���C���[���W�ƃ}�E�X���W������Ő��̕`��
		DrawLineAA( player_pos.x, player_pos.y, circle_pos.x, circle_pos.y, GetColor( 255, 255, 0 ) );
		//	�}�E�X���W�̂Ƃ���ɉ~�̕`��
		DrawCircleAA( circle_pos.x, circle_pos.y, 5.0f, 100, GetColor( 255, 255, 0 ) );
	}

	DrawString( 20, 20, "�}�E�X�N���b�N�F�N���b�N���W�ݒ�", GetColor( 255, 255, 0 ) );
	DrawString( 20, 40, "�}�E�X�h���b�O�F�}�E�X�̏��Ƀv���C���[��u���i�v���C���[�̓N���b�N���W�̕��������j", GetColor( 255, 255, 0 ) );
	DrawString( 20, 60, "���@�v���C���[�ƃN���b�N���W�����񂾐���`��", GetColor( 255, 255, 0 ) );
	DrawString( 20, 80, "�}�E�X�{�^���𗣂�����v���C���[�͌����Ă�������Ɉړ����� �� ���������قǈړ�������", GetColor( 255, 255, 255 ) );
	DrawString( 20, 120, "�v���C���[�Ƃm�o�b�œ����蔻��i�h���b�O���͓�����Ȃ��j", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	�I������
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( player_image );
	DeleteGraph( npc_image );
}
