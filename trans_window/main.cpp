#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	int GrHandle;
	int Screen;
	int SoftImage;
	int AlphaAdd;
	int Alpha;

	// �E�C���h�E���[�h�ŋN��
	ChangeWindowMode( TRUE );

	constexpr int kScreeWidth = 640;
	constexpr int kScreeHeight = 480;

	SetGraphMode( kScreeWidth , kScreeHeight , 32 );

	// ���߃E�C���h�E�ݒ�
	SetUseBackBufferTransColorFlag( TRUE );

	// �c�w���C�u�����̏�����
	if( DxLib_Init() < 0 ) return -1;

	// �`��Ώۂɂł���A���t�@�`�����l���t����ʂ��쐬
	Screen = MakeScreen( kScreeWidth , kScreeHeight , TRUE );

	// ��ʎ�荞�ݗp�̃\�t�g�E�G�A�p�摜���쐬
	SoftImage = MakeARGB8ColorSoftImage( kScreeWidth , kScreeHeight );

	// �摜��ǂݍ��ލۂɃA���t�@�l��RGB�l�ɏ�Z����悤�ɐݒ肷��
	SetUsePremulAlphaConvertLoad( TRUE );

	// �摜�̓ǂݍ���
	GrHandle = LoadGraph( "syaro_body.png" );

	// �`����`��Ώۂɂł���A���t�@�`�����l���t����ʂɂ���
	SetDrawScreen( Screen );

	// ��ɏ��������s
	SetAlwaysRunFlag( true );

	
	
	constexpr int kX = 0;
	constexpr int kY = 1;
	int pos[ 2 ] = { 0 , 0 };

	constexpr int kSpeed = 10;

	// ���C�����[�v
	AlphaAdd = -8;
	Alpha = 255;
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		// ����
		if( CheckHitKey( KEY_INPUT_UP ) )
		{
			pos[ kY ] -= kSpeed;
		}
		if( CheckHitKey( KEY_INPUT_DOWN ) )
		{
			pos[ kY ] += kSpeed;
		}
		if( CheckHitKey( KEY_INPUT_RIGHT ) )
		{
			pos[ kX ] += kSpeed;
		}
		if( CheckHitKey( KEY_INPUT_LEFT ) )
		{
			pos[ kX ] -= kSpeed;
		}

		if( CheckHitKey( KEY_INPUT_LCONTROL ) && CheckHitKey( KEY_INPUT_T ) )
		{
			// �E�B���h�E���őO�ʂɂ���
			SetWindowPos( GetMainWindowHandle() , HWND_TOPMOST , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE );
		}

		if( CheckHitKey( KEY_INPUT_LCONTROL ) && CheckHitKey( KEY_INPUT_B ) )
		{
			// �E�B���h�E���őO�ʂɂ���
			SetWindowPos( GetMainWindowHandle() , HWND_BOTTOM , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE );
		}

		// ��ʂ��N���A
		ClearDrawScreen();

		// �A���t�@�l�̕ύX
		/*Alpha += AlphaAdd;
		if( Alpha <= 0 )
		{
			Alpha = 0;
			AlphaAdd = 8;
		}
		else if( Alpha >= 255 )
		{
			Alpha = 255;
			AlphaAdd = -8;
		}*/

		// �`��u�����h���[�h����Z�ς݃A���t�@�ɃZ�b�g
		//SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA , Alpha );

		SetWindowPosition( pos[ kX ] , pos[ kY ] );

		// �摜��`��
		DrawGraph( 0 , 0 , GrHandle , TRUE );

		// �`���̉摜���\�t�g�C���[�W�Ɏ擾����
		GetDrawScreenSoftImage( 0 , 0 , 640 , 480 , SoftImage );

		// ��荞�񂾃\�t�g�C���[�W���g�p���ē��߃E�C���h�E�̏�Ԃ��X�V����
		UpdateLayerdWindowForPremultipliedAlphaSoftImage( SoftImage );

		// �����҂�
		Sleep( 12 );
	}

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}