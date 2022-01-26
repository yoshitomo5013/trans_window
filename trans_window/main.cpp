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

	constexpr int kScreeWidth = 1920;
	constexpr int kScreeHeight = 1080;

	SetGraphMode( kScreeWidth , kScreeHeight , 32 );

	// ���߃E�C���h�E�ݒ�
	SetUseBackBufferTransColorFlag( TRUE );

	SetFullSceneAntiAliasingMode( 4 , 2 ); // �t���X�N���[���A���`�G�C���A�X��L��

	// �c�w���C�u�����̏�����
	if( DxLib_Init() < 0 ) return -1;

	// �`��Ώۂɂł���A���t�@�`�����l���t����ʂ��쐬
	Screen = MakeScreen( kScreeWidth , kScreeHeight , TRUE );

	// ��ʎ�荞�ݗp�̃\�t�g�E�G�A�p�摜���쐬
	SoftImage = MakeARGB8ColorSoftImage( kScreeWidth , kScreeHeight );

	// �摜��ǂݍ��ލۂɃA���t�@�l��RGB�l�ɏ�Z����悤�ɐݒ肷��
	SetUsePremulAlphaConvertLoad( TRUE );

	// �A���`�G�C���A�X�Ɖ����̗������s��
	ChangeFontType( DX_FONTTYPE_ANTIALIASING_EDGE_4X4 );

	// Z�o�b�t�@��Z�o�b�t�@�ւ̏������݂�L���ɂ���
	SetUseZBuffer3D( true );
	SetWriteZBuffer3D( true );

	MV1SetLoadModelUsePackDraw( true );// ���f���̕�����L��

	//SetUseLighting( false );

	// �摜�̓ǂݍ���
	//GrHandle = LoadGraph( "syaro_body.png" );
	auto model = MV1LoadModel( "mmd/�V��������� Ver. 1.01/Sharo Kirima Ver. 1.01.pmx" );
	//auto model = MV1LoadModel( "mmd/�ˊԎјH�̐��� Ver. 1.00/Sharo's Default Swimsuit  Ver. 1.00.pmx" );
	//auto model = MV1LoadModel( "mmd/�ˊԎјH�̐��� Ver. 1.00/Sharo Kirima's Extra Swimsuit.pmx" );

	// �����A�j���[�V����
	constexpr int kStartAnimation =2;

	// �A�j���[�V����
	int attach_no_ = MV1AttachAnim( model , kStartAnimation );
	float totalTime = MV1GetAttachAnimTotalTime( model , attach_no_ );
	float time = 0;

	// �`����`��Ώۂɂł���A���t�@�`�����l���t����ʂɂ���
	SetDrawScreen( Screen );

	// ��ɏ��������s
	SetAlwaysRunFlag( true );
	//SetUseLighting( false );

	// �J����
	constexpr float kCameraSpeed = 0.1f;
	VECTOR camera=VGet( 0.0f , 15.0f , -40.0f );
	VECTOR target = VGet( 0.0f , 14.0f , 0.0f );
	SetCameraPositionAndTarget_UpVecY( camera,target );
	SetCameraNearFar( 1.0f , 100.0f );
	
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
		if( CheckHitKey( KEY_INPUT_W ) )// �E�B���h�E
		{
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
		}
		else if( CheckHitKey( KEY_INPUT_LSHIFT ) )// �J�������W
		{
			if( CheckHitKey( KEY_INPUT_UP ) )
			{
				camera.z += kCameraSpeed;
			}
			if( CheckHitKey( KEY_INPUT_DOWN ) )
			{
				camera.z -= kCameraSpeed;
			}
			if( CheckHitKey( KEY_INPUT_RIGHT ) )
			{
				camera.x += kCameraSpeed;
			}
			if( CheckHitKey( KEY_INPUT_LEFT ) )
			{
				camera.x -= kCameraSpeed;
			}
		}
		else// �J���������_
		{
			if( CheckHitKey( KEY_INPUT_UP ) )
			{
				target.y += kCameraSpeed;
			}
			if( CheckHitKey( KEY_INPUT_DOWN ) )
			{
				target.y -= kCameraSpeed;
			}
			if( CheckHitKey( KEY_INPUT_RIGHT ) )
			{
				target.x += kCameraSpeed;
			}
			if( CheckHitKey( KEY_INPUT_LEFT ) )
			{
				target.x -= kCameraSpeed;
			}
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

		// �A�j���[�V�����X�V
		time += 1.0f;
		if( totalTime < time )
		{
			MV1DetachAnim( model , attach_no_ );
			attach_no_ = MV1AttachAnim( model , GetRand( 2 ) );
			totalTime = MV1GetAttachAnimTotalTime( model , attach_no_ );
			time = 0;
		}

		MV1SetAttachAnimTime( model , attach_no_ , time );
		SetCameraPositionAndTarget_UpVecY( camera ,target );

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
		//DrawGraph( 0 , 0 , GrHandle , TRUE );
		//DrawBox( 0 , 0 , kScreeWidth , kScreeHeight , 0xffffff, true );

		MV1SetPosition( model , VGet( 0.0f , 0.0f , 0.0f ) );
		MV1DrawModel( model );
		//DrawCube3D( VGet( 0.0f , 8.0f , 0.0f ) , VGet( 0.5f , 8.5f , -5.0f ) , 0xff0000 , 0xffffff , true );

		// �`���̉摜���\�t�g�C���[�W�Ɏ擾����
		GetDrawScreenSoftImage( 0 , 0 , kScreeWidth , kScreeHeight , SoftImage );

		// ��荞�񂾃\�t�g�C���[�W���g�p���ē��߃E�C���h�E�̏�Ԃ��X�V����
		UpdateLayerdWindowForPremultipliedAlphaSoftImage( SoftImage );

		// �����҂�
		Sleep( 16 );
	}

	MV1DeleteModel( model );
	InitGraph();

	// �c�w���C�u�����̌�n��
	DxLib_End();

	// �\�t�g�̏I��
	return 0;
}