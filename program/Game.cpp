#include "Main.h"
#include "Game.h"
#include "Hit.h"
#include "Float2.h"

//	ＮＰＣの回転スピード
#define NPC_ROT_SPEED	1.0f
//	プレイヤーとＮＰＣの半径（円として判定するので）
#define CHARA_RADIUS	25.0f

//	プレイヤー用の変数
int player_image;		//	画像
Float2 player_pos;		//	座標（ＸとＹの２つの float 型がある構造体）
float player_rot;		//	向き
//	プレイヤーの移動量
Float2 player_mov;

//	ＮＰＣ用の変数
int npc_image;
Float2 npc_pos;
float npc_rot;

//	マウス座標を入れる用の変数（更新処理と描画処理でマウス座標を使うのでここで宣言しています）
Float2 mouse;

//	マウスドラッグ中かどうか
bool mouse_drag;

Float2 circle_pos;

//---------------------------------------------------------------------------------
//	初期化処理
//---------------------------------------------------------------------------------
void GameInit()
{
	//	画像の読み込み
	player_image = LoadGraph( "data/player.bmp" );
	npc_image = LoadGraph( "data/npc.bmp" );
	//	初期座標や向きの設定
	//	とりあえず画面の中央に置いています
	player_pos.x = SCREEN_W / 2;
	player_pos.y = SCREEN_H / 2;
	player_rot = 0.0f;

	npc_pos.x = (float)GetRand( SCREEN_W );
	npc_pos.y = (float)GetRand( SCREEN_H );
	npc_rot = 0.0f;
}
//---------------------------------------------------------------------------------
//	更新処理
//---------------------------------------------------------------------------------
void GameUpdate()
{
	//	マウス座標の取得
	mouse.x = (float)GetMouseX();
	mouse.y = (float)GetMouseY();

	//	マウスを押した瞬間
	if( PushMouseInput() ){
		//	１：マウスクリックでプレイヤーをマウスの所へ
		circle_pos.x = mouse.x;
		circle_pos.y = mouse.y;
	}

	//	マウスを押しているとき
	if( CheckMouseInput() ){
		//	マウスを押しているので
		mouse_drag = true;

		//	２：プレイヤーの向きをマウス座標の所へ
		//	プレイヤーから見てマウス座標がどれだけ進んだところにあるか
		float x = circle_pos.x - mouse.x;
		float y = circle_pos.y - mouse.y;

		player_pos.x = mouse.x;
		player_pos.y = mouse.y;

		//	上で求めたプレイヤーからマウスまでの値を移動量として設定します
		//	そのままだと大きすぎるので適当に小さくします（ 0.05 倍します：２０分の１）
		player_mov.x = x * 0.05f;
		player_mov.y = y * 0.05f;

		//	上で求めたプレイヤーからマウスまでの値を使って角度を求める
//		float radian = GetRadian_LengthMukaiSotte( y, x );
		//	同じ割合で短くした player_mov の値を使っても同じ角度がもらえます
		float radian = GetRadian_LengthMukaiSotte( player_mov.y, player_mov.x );
		//	プレイヤーの角度変数は「度」なので、ラジアン角を度に変換して渡します
		player_rot = TO_DEGREE( radian );

	}else{
		//	じゃないとき（マウスを押していないとき）
		mouse_drag = false;

		//	３：マウスを離したらプレイヤーは移動を続ける
		player_pos.x += player_mov.x;
		player_pos.y += player_mov.y;
	}

	//	ＮＰＣの回転
	npc_rot += NPC_ROT_SPEED;

	//	プレイヤーとＮＰＣで当たり判定（円同士の当たり判定：Float2 バージョン）
	if( CheckCircleHit( player_pos, CHARA_RADIUS, npc_pos, CHARA_RADIUS ) ){
		//	当たったらＮＰＣの座標をランダムに設定
		npc_pos.x = (float)GetRand( SCREEN_W );
		npc_pos.y = (float)GetRand( SCREEN_H );
	}
}
//---------------------------------------------------------------------------------
//	描画処理
//---------------------------------------------------------------------------------
void GameRender()
{
	//	ＮＰＣの描画
	DrawRotaGraphF( npc_pos.x, npc_pos.y, 1.0f, TO_RADIAN( npc_rot ), npc_image, TRUE );
	//	プレイヤーの描画
	DrawRotaGraphF( player_pos.x, player_pos.y, 1.0f, TO_RADIAN( player_rot ), player_image, TRUE );

	//	マウスドラッグ中の時だけ描画
	if( mouse_drag ){
		//	プレイヤー座標とマウス座標を結んで線の描画
		DrawLineAA( player_pos.x, player_pos.y, circle_pos.x, circle_pos.y, GetColor( 255, 255, 0 ) );
		//	マウス座標のところに円の描画
		DrawCircleAA( circle_pos.x, circle_pos.y, 5.0f, 100, GetColor( 255, 255, 0 ) );
	}

	DrawString( 20, 20, "マウスクリック：クリック座標設定", GetColor( 255, 255, 0 ) );
	DrawString( 20, 40, "マウスドラッグ：マウスの所にプレイヤーを置く（プレイヤーはクリック座標の方を向く）", GetColor( 255, 255, 0 ) );
	DrawString( 20, 60, "→　プレイヤーとクリック座標を結んだ線を描画", GetColor( 255, 255, 0 ) );
	DrawString( 20, 80, "マウスボタンを離したらプレイヤーは向いている方向に移動する → 線が長いほど移動が速い", GetColor( 255, 255, 255 ) );
	DrawString( 20, 120, "プレイヤーとＮＰＣで当たり判定（ドラッグ中は当たらない）", GetColor( 255, 255, 255 ) );
}
//---------------------------------------------------------------------------------
//	終了処理
//---------------------------------------------------------------------------------
void GameExit()
{
	DeleteGraph( player_image );
	DeleteGraph( npc_image );
}
