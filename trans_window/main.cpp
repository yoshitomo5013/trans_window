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

	constexpr int kScreeWidth = 640;
	constexpr int kScreeHeight = 480;

	SetGraphMode( kScreeWidth , kScreeHeight , 32 );

	// 透過ウインドウ設定
	SetUseBackBufferTransColorFlag( TRUE );

	// ＤＸライブラリの初期化
	if( DxLib_Init() < 0 ) return -1;

	// 描画対象にできるアルファチャンネル付き画面を作成
	Screen = MakeScreen( kScreeWidth , kScreeHeight , TRUE );

	// 画面取り込み用のソフトウエア用画像を作成
	SoftImage = MakeARGB8ColorSoftImage( kScreeWidth , kScreeHeight );

	// 画像を読み込む際にアルファ値をRGB値に乗算するように設定する
	SetUsePremulAlphaConvertLoad( TRUE );

	// 画像の読み込み
	GrHandle = LoadGraph( "syaro_body.png" );

	// 描画先を描画対象にできるアルファチャンネル付き画面にする
	SetDrawScreen( Screen );

	// 常に処理を実行
	SetAlwaysRunFlag( true );

	
	
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
			// ウィンドウを最前面にする
			SetWindowPos( GetMainWindowHandle() , HWND_TOPMOST , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE );
		}

		if( CheckHitKey( KEY_INPUT_LCONTROL ) && CheckHitKey( KEY_INPUT_B ) )
		{
			// ウィンドウを最前面にする
			SetWindowPos( GetMainWindowHandle() , HWND_BOTTOM , 0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE );
		}

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
		DrawGraph( 0 , 0 , GrHandle , TRUE );

		// 描画先の画像をソフトイメージに取得する
		GetDrawScreenSoftImage( 0 , 0 , 640 , 480 , SoftImage );

		// 取り込んだソフトイメージを使用して透過ウインドウの状態を更新する
		UpdateLayerdWindowForPremultipliedAlphaSoftImage( SoftImage );

		// 少し待つ
		Sleep( 12 );
	}

	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}