#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	int GrHandle;
	int Screen;
	int SoftImage;
	int AlphaAdd;
	int Alpha;

	// ウインドウモードで起動
	ChangeWindowMode( TRUE );

	constexpr int kScreeWidth = 1920;
	constexpr int kScreeHeight = 1080;

	SetGraphMode( kScreeWidth , kScreeHeight , 32 );

	// 透過ウインドウ設定
	SetUseBackBufferTransColorFlag( TRUE );

	SetFullSceneAntiAliasingMode( 4 , 2 ); // フルスクリーンアンチエイリアスを有効

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 ) return -1;

	// 描画対象にできるアルファチャンネル付き画面を作成
	Screen = MakeScreen( kScreeWidth , kScreeHeight , TRUE );

	// 画面取り込み用のソフトウエア用画像を作成
	SoftImage = MakeARGB8ColorSoftImage( kScreeWidth , kScreeHeight );

	// 画像を読み込む際にアルファ値をRGB値に乗算するように設定する
	SetUsePremulAlphaConvertLoad( TRUE );

	// アンチエイリアスと縁取りの両方を行う
	ChangeFontType( DX_FONTTYPE_ANTIALIASING_EDGE_4X4 );

	// ZバッファとZバッファへの書き込みを有効にする
	SetUseZBuffer3D( true );
	SetWriteZBuffer3D( true );

	MV1SetLoadModelUsePackDraw( true );// モデルの複数を有効

	//SetUseLighting( false );

	// 画像の読み込み
	//GrHandle = LoadGraph( "syaro_body.png" );
	auto model = MV1LoadModel( "mmd/シャロちゃん Ver. 1.01/Sharo Kirima Ver. 1.01.pmx" );
	//auto model = MV1LoadModel( "mmd/桐間紗路の水着 Ver. 1.00/Sharo's Default Swimsuit  Ver. 1.00.pmx" );
	//auto model = MV1LoadModel( "mmd/桐間紗路の水着 Ver. 1.00/Sharo Kirima's Extra Swimsuit.pmx" );

	// 初期アニメーション
	constexpr int kStartAnimation =2;

	// アニメーション
	int attach_no_ = MV1AttachAnim( model , kStartAnimation );
	float totalTime = MV1GetAttachAnimTotalTime( model , attach_no_ );
	float time = 0;

	// 描画先を描画対象にできるアルファチャンネル付き画面にする
	SetDrawScreen( Screen );

	// 常に処理を実行
	SetAlwaysRunFlag( true );
	//SetUseLighting( false );

	// カメラ
	constexpr float kCameraSpeed = 0.1f;
	VECTOR camera=VGet( 0.0f , 15.0f , -40.0f );
	VECTOR target = VGet( 0.0f , 14.0f , 0.0f );
	SetCameraPositionAndTarget_UpVecY( camera,target );
	SetCameraNearFar( 1.0f , 100.0f );
	
	constexpr int kX = 0;
	constexpr int kY = 1;
	int pos[ 2 ] = { 0 , 0 };

	constexpr int kSpeed = 10;

	// メインループ
	AlphaAdd = -8;
	Alpha = 255;
	while( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 )
	{
		// 処理
		if( CheckHitKey( KEY_INPUT_W ) )// ウィンドウ
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
		else if( CheckHitKey( KEY_INPUT_LSHIFT ) )// カメラ座標
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
		else// カメラ注視点
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
			// ウィンドウを最前面にする
			SetWindowPos( GetMainWindowHandle() , HWND_TOPMOST , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE );
		}

		if( CheckHitKey( KEY_INPUT_LCONTROL ) && CheckHitKey( KEY_INPUT_B ) )
		{
			// ウィンドウを最前面にする
			SetWindowPos( GetMainWindowHandle() , HWND_BOTTOM , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE );
		}

		// アニメーション更新
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

		// 画面をクリア
		ClearDrawScreen();

		// アルファ値の変更
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

		// 描画ブレンドモードを乗算済みアルファにセット
		//SetDrawBlendMode( DX_BLENDMODE_PMA_ALPHA , Alpha );

		SetWindowPosition( pos[ kX ] , pos[ kY ] );

		// 画像を描画
		//DrawGraph( 0 , 0 , GrHandle , TRUE );
		//DrawBox( 0 , 0 , kScreeWidth , kScreeHeight , 0xffffff, true );

		MV1SetPosition( model , VGet( 0.0f , 0.0f , 0.0f ) );
		MV1DrawModel( model );
		//DrawCube3D( VGet( 0.0f , 8.0f , 0.0f ) , VGet( 0.5f , 8.5f , -5.0f ) , 0xff0000 , 0xffffff , true );

		// 描画先の画像をソフトイメージに取得する
		GetDrawScreenSoftImage( 0 , 0 , kScreeWidth , kScreeHeight , SoftImage );

		// 取り込んだソフトイメージを使用して透過ウインドウの状態を更新する
		UpdateLayerdWindowForPremultipliedAlphaSoftImage( SoftImage );

		// 少し待つ
		Sleep( 16 );
	}

	MV1DeleteModel( model );
	InitGraph();

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}