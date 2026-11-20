/*======================================================================================//
*  ・controller.cpp
*    pad入力状態の処理を書いてる
*
*    作成者 大塚  変更日 2026/11/19
*
*    0 : Aボタン,1 : Bボタン,2 : Xボタン,3 : Yボタン,4 : LB,5 : RB,6 : VEIW,7 : MENU
*    8 : 左スティック押し込み, 9 : 右スティック押し込み
*    currentState.lX : 左スティックの左右, lY : 上下
*    currentState.lRx ; 右スティックの左右, lRy : 上下
*    currentState.rgdwPOV[0] : 十字キー
*      0 : 真上, 9000 : 右, 18000 : 下, 27000 : 左,    間の値も取得可能だが、左上等実装したい場合は別途計算して下さい
* 
*
//======================================================================================*/

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "controller.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
namespace {
    // 入力デバイスの検索・作成
    IDirectInput8* directInput = nullptr;
    // 特定のデバイスからの状態取得
    IDirectInputDevice8* gamepad = nullptr;

    // 公式指定の型
    DIJOYSTATE2 currentState{};
    DIJOYSTATE2 previousState{};

    bool inputAvailable = false;
}

//*****************************************************************************
// コールバック関数
//*****************************************************************************
// deviceInfoにpad情報を格納
BOOL CALLBACK EnumGamepad(const DIDEVICEINSTANCE* deviceInfo, void* context)
{
    HRESULT result = directInput->CreateDevice(
        deviceInfo->guidInstance,  // padID
        &gamepad,                  // 格納場所指定
        nullptr
    );

    // 見つからない場合検索を続行
    if (FAILED(result)) return DIENUM_CONTINUE;

    // 検索終了
    return DIENUM_STOP;
}

//*****************************************************************************
// padの初期化
//*****************************************************************************
// falseで接続エラー
bool GamepadInitialize(HINSTANCE instance, HWND window)
{
    HRESULT result = DirectInput8Create(
        instance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        reinterpret_cast<void**>(&directInput),
        nullptr
    );

    // ガード処理
    if (FAILED(result))
    {
        GamepadFinalize();
        return false;
    }

    // ↑オブジェクトの生成に成功
    // ↓padの検索

    result = directInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL,  // 検索デバイス指定
        EnumGamepad,           // 見つかったら呼ぶ
        nullptr,               // その関数に追加で情報を渡す場合ここに記入 
        DIEDFL_ATTACHEDONLY    // 条件記入　現在接続されているもの
    );

    // 見つからなかった場合
    if (FAILED(result) || gamepad == nullptr)
    {
        GamepadFinalize();
        return false;
    }

    // 公式フォーマットの順で入力受け取る
    result = gamepad->SetDataFormat(&c_dfDIJoystick2);

    // ガード処理
    if (FAILED(result))
    {
        GamepadFinalize();
        return false;
    }

    result = gamepad->SetCooperativeLevel(
        window,                                 // 入力を受け取るウィンドウ
        DISCL_FOREGROUND | DISCL_NONEXCLUSIVE   // 
    );

    // ガード処理
    if (FAILED(result))
    {
        GamepadFinalize();
        return false;
    }

    // 左スティック取得
    {
        // X軸の取得
        {
            DIPROPRANGE range{};
            range.diph.dwSize = sizeof(range);
            range.diph.dwHeaderSize = sizeof(range.diph);
            range.diph.dwObj = DIJOFS_X;                    // 軸指定
            range.diph.dwHow = DIPH_BYOFFSET;               // 軸をデータ形式内の位置で指定
            range.lMin = -1000;                             // 最小値
            range.lMax = 1000;                              // 最大値

            // 設定をpadに適用
            result = gamepad->SetProperty(DIPROP_RANGE, &range.diph);

            if (FAILED(result))
            {
                GamepadFinalize();
                return false;
            }
        }

        // Y軸の取得
        {
            DIPROPRANGE range{};
            range.diph.dwSize = sizeof(range);
            range.diph.dwHeaderSize = sizeof(range.diph);
            range.diph.dwObj = DIJOFS_Y;                    // 軸指定
            range.diph.dwHow = DIPH_BYOFFSET;               // 軸をデータ形式内の位置で指定
            range.lMin = -1000;                             // 最小値
            range.lMax = 1000;                              // 最大値

            // 設定をpadに適用
            result = gamepad->SetProperty(DIPROP_RANGE, &range.diph);

            if (FAILED(result))
            {
                GamepadFinalize();
                return false;
            }
        }
    }

    // 右スティック取得
    {
        // X軸の取得
        {
            DIPROPRANGE range{};
            range.diph.dwSize = sizeof(range);
            range.diph.dwHeaderSize = sizeof(range.diph);
            range.diph.dwObj = DIJOFS_RX;                   // 軸指定
            range.diph.dwHow = DIPH_BYOFFSET;               // 軸をデータ形式内の位置で指定
            range.lMin = -1000;                             // 最小値
            range.lMax = 1000;                              // 最大値

            // 設定をpadに適用
            result = gamepad->SetProperty(DIPROP_RANGE, &range.diph);

            if (FAILED(result))
            {
                GamepadFinalize();
                return false;
            }
        }

        // Y軸の取得
        {
            DIPROPRANGE range{};
            range.diph.dwSize = sizeof(range);
            range.diph.dwHeaderSize = sizeof(range.diph);
            range.diph.dwObj = DIJOFS_RY;                   // 軸指定
            range.diph.dwHow = DIPH_BYOFFSET;               // 軸をデータ形式内の位置で指定
            range.lMin = -1000;                             // 最小値
            range.lMax = 1000;                              // 最大値

            // 設定をpadに適用
            result = gamepad->SetProperty(DIPROP_RANGE, &range.diph);

            if (FAILED(result))
            {
                GamepadFinalize();
                return false;
            }
        }
    }

    // 入力取得を試す 取得できない場合は Update で再試行
    result = gamepad->Acquire();


    return true;
}

//*****************************************************************************
// 更新
//*****************************************************************************
// player更新前に呼ぶ
void GamepadUpdate()
{
    previousState = currentState;
    inputAvailable = false;

    // ガード処理
    if (!gamepad)
    {
        currentState = {};
        return;
    }

    // 最新の状態を更新
    HRESULT result = gamepad->Poll();

    // ガード処理
    if (FAILED(result))
    {
        // フォーカスを失うなどして取得できなくなった場合に再取得する。
        gamepad->Acquire();
        currentState = {};
        return;
    }

    // 最新状態の書き込み
    result = gamepad->GetDeviceState(
        sizeof(currentState),  // 受取先のサイズ
        &currentState          // 受取先のアドレス
    ); 

    // 入力失敗の場合は値をクリア
    if (FAILED(result))
    {
        currentState = {};
        return;
    }
  
    inputAvailable = true;
}

//*****************************************************************************
// 終了処理
//*****************************************************************************
void GamepadFinalize()
{

    if (gamepad != nullptr)
    {
        // 入力取得の終了
        gamepad->Unacquire();
        gamepad->Release();
        gamepad = nullptr;
    }

    if (directInput != nullptr)
    {
        directInput->Release();
        directInput = nullptr;
    }

    currentState = {};
    previousState = {};
    inputAvailable = false;
}

//*****************************************************************************
// 接続判定
//*****************************************************************************
bool InputAvailable()
{
    return inputAvailable;
}

//*****************************************************************************
// 入力判定
//*****************************************************************************
#pragma region 入力判定処理
// 押している間
// ボタンの入力
bool GamepadIsDown(int button)
{
    if (!gamepad || button < 0 || button >= 128)
    {
        return false;
    }

    return (currentState.rgbButtons[button] & 0x80) != 0;
}

// 押した瞬間
bool GamepadIsPressed(int button)
{
    return GamepadIsDown(button) && (previousState.rgbButtons[button] & 0x80) == 0;
}

// スティックの入力
// 左 -1、中央 0,右 1
// 移動を傾きの大きさにする場合は、この数値をかければいい
float GamepadGetLeftAxisX()
{
    return static_cast<float>(currentState.lX) / 1000.0f;
}

// 上 -1,中央 0,下 1
float GamepadGetLeftAxisY()
{
    return static_cast<float>(currentState.lY) / 1000.0f;
}

// 右スティック
float GamepadGetRightAxisX()
{
    return static_cast<float>(currentState.lRx) / 1000.0f;
}

float GamepadGetRightAxisY()
{
    return static_cast<float>(currentState.lRy) / 1000.0f;
}

// 十字キーの入力
// 押した瞬間
bool IsUpPressed()
{
    return inputAvailable && currentState.rgdwPOV[0] == 0 && previousState.rgdwPOV[0] != 0;
}

bool IsDownPressed()
{
    return inputAvailable && currentState.rgdwPOV[0] == 18000 && previousState.rgdwPOV[0] != 18000;
}

bool IsLeftPressed()
{
    return inputAvailable && currentState.rgdwPOV[0] == 27000 && previousState.rgdwPOV[0] != 27000;
}

bool IsRightPressed()
{
    return inputAvailable && currentState.rgdwPOV[0] == 9000 && previousState.rgdwPOV[0] != 9000;
}

// 押している間
bool IsUpHeld()
{
    return inputAvailable && currentState.rgdwPOV[0] == 0;
}

bool IsRightHeld()
{
    return inputAvailable && currentState.rgdwPOV[0] == 9000;
}

bool IsDownHeld()
{
    return inputAvailable && currentState.rgdwPOV[0] == 18000;
}

bool IsLeftHeld()
{
    return inputAvailable && currentState.rgdwPOV[0] == 27000;
}


#pragma endregion

