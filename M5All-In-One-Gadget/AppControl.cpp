#include "AppControl.h"
#include <Arduino.h>
#include <M5Stack.h>

// AppControlクラスが直接従えているクラスをインスタンス化
MdLcd mlcd;
MdWBGTMonitor mwbgt;
MdMusicPlayer mmplay;
MdMeasureDistance mmdist;
MdDateTime mdtime;

const char *g_str_orange[] = {
    COMMON_ORANGE0_IMG_PATH,
    COMMON_ORANGE1_IMG_PATH,
    COMMON_ORANGE2_IMG_PATH,
    COMMON_ORANGE3_IMG_PATH,
    COMMON_ORANGE4_IMG_PATH,
    COMMON_ORANGE5_IMG_PATH,
    COMMON_ORANGE6_IMG_PATH,
    COMMON_ORANGE7_IMG_PATH,
    COMMON_ORANGE8_IMG_PATH,
    COMMON_ORANGE9_IMG_PATH,
};

const char *g_str_blue[] = {
    COMMON_BLUE0_IMG_PATH,
    COMMON_BLUE1_IMG_PATH,
    COMMON_BLUE2_IMG_PATH,
    COMMON_BLUE3_IMG_PATH,
    COMMON_BLUE4_IMG_PATH,
    COMMON_BLUE5_IMG_PATH,
    COMMON_BLUE6_IMG_PATH,
    COMMON_BLUE7_IMG_PATH,
    COMMON_BLUE8_IMG_PATH,
    COMMON_BLUE9_IMG_PATH,
};

void AppControl::setBtnAFlg(bool flg) // 割込み関数から呼んだ先の関数
{
    m_flag_btnA_is_pressed = flg; //_flag_btnA_is_pressed変数はボタンが押されると「true」1回でも押すとずっと「true」
}

void AppControl::setBtnBFlg(bool flg) // 割込み関数から呼んだ先の関数
{
    m_flag_btnB_is_pressed = flg;
}

void AppControl::setBtnCFlg(bool flg) // 割込み関数から呼んだ先の関数
{
    m_flag_btnC_is_pressed = flg;
}

void AppControl::setBtnAllFlgFalse() // すべてのボタンをfalseにする→ボタンをリセットする
{
    m_flag_btnA_is_pressed = false;
    m_flag_btnB_is_pressed = false;
    m_flag_btnC_is_pressed = false;
}

State AppControl::getState()
{
    return m_state;
}

void AppControl::setState(State state)
{
    m_state = state;
}

Action AppControl::getAction()
{
    return m_action;
}

void AppControl::setAction(Action action)
{
    m_action = action;
}

void AppControl::setStateAction(State state, Action action)
{
    setState(state);
    setAction(action);
}

FocusState AppControl::getFocusState()
{
    return m_focus_state;
}

void AppControl::setFocusState(FocusState fs)
{
    m_focus_state = fs;
}

void AppControl::displayTitleInit()
{
    mlcd.displayJpgImage(TITLE_IMG_PATH);
}

void AppControl::displayMenuInit()
{
    mlcd.fillBackgroundWhite(); // 背景色を白に設定
    mlcd.displayJpgImageCoordinate(MENU_WBGT_FOCUS_IMG_PATH, MENU_WBGT_X_CRD, MENU_WBGT_Y_CRD);
    mlcd.displayJpgImageCoordinate(MENU_MUSIC_IMG_PATH, MENU_MUSIC_X_CRD, MENU_MUSIC_Y_CRD);
    mlcd.displayJpgImageCoordinate(MENU_MEASURE_IMG_PATH, MENU_MEASURE_X_CRD, MENU_MEASURE_Y_CRD);
    mlcd.displayJpgImageCoordinate(MENU_DATE_IMG_PATH, MENU_DATE_X_CRD, MENU_DATE_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_UP_IMG_PATH, MENU_UP_X_CRD, MENU_UP_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_DOWN_IMG_PATH, MENU_DOWN_X_CRD, MENU_DOWN_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_DECIDE_IMG_PATH, MENU_DECIDE_X_CRD, MENU_DECIDE_Y_CRD);
}

void AppControl::focusChangeImg(FocusState current_state, FocusState next_state)
{
    // メニュー画面においてフォーカスを移動させる
    // 現在フォーカスする項目と次にフォーカスする項目
    switch (current_state)
    {
    case MENU_WBGT: // 熱中症
        mlcd.displayJpgImageCoordinate(MENU_WBGT_IMG_PATH, MENU_WBGT_X_CRD, MENU_WBGT_Y_CRD);
        break;
    case MENU_MUSIC: // 音楽
        mlcd.displayJpgImageCoordinate(MENU_MUSIC_IMG_PATH, MENU_MUSIC_X_CRD, MENU_MUSIC_Y_CRD);
        break;
    case MENU_MEASURE: // 距離
        mlcd.displayJpgImageCoordinate(MENU_MEASURE_IMG_PATH, MENU_MEASURE_X_CRD, MENU_MEASURE_Y_CRD);
        break;
    case MENU_DATE: // 時刻
        mlcd.displayJpgImageCoordinate(MENU_DATE_IMG_PATH, MENU_DATE_X_CRD, MENU_DATE_Y_CRD);
        break;
    }
    switch (next_state)
    {
    case MENU_WBGT: // 熱中症
        mlcd.displayJpgImageCoordinate(MENU_WBGT_FOCUS_IMG_PATH, MENU_WBGT_X_CRD, MENU_WBGT_Y_CRD);
        break;
    case MENU_MUSIC: // 音楽
        mlcd.displayJpgImageCoordinate(MENU_MUSIC_FOCUS_IMG_PATH, MENU_MUSIC_X_CRD, MENU_MUSIC_Y_CRD);
        break;
    case MENU_MEASURE: // 距離
        mlcd.displayJpgImageCoordinate(MENU_MEASURE_FOCUS_IMG_PATH, MENU_MEASURE_X_CRD, MENU_MEASURE_Y_CRD);
        break;
    case MENU_DATE: // 時刻
        mlcd.displayJpgImageCoordinate(MENU_DATE_FOCUS_IMG_PATH, MENU_DATE_X_CRD, MENU_DATE_Y_CRD);
        break;
    }
}

void AppControl::displayWBGTInit()
{
    mlcd.clearDisplay();
    mlcd.displayJpgImageCoordinate(MEASURE_NOTICE_IMG_PATH, MEASURE_NOTICE_X_CRD, MEASURE_NOTICE_Y_CRD);
    mlcd.displayJpgImageCoordinate(MEASURE_CM_IMG_PATH, MEASURE_CM_X_CRD, MEASURE_CM_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_BACK_IMG_PATH, MEASURE_BACK_X_CRD, MEASURE_BACK_Y_CRD);
}

void AppControl::displayTempHumiIndex()
{
}

void AppControl::displayMusicInit()
{
    // 音楽停止画面を描画し、displayMusicTitle()を呼出して音楽ファイルのファイル名を描画する。
    mlcd.fillBackgroundWhite(); // 背景色を白に設定
    displayMusicStop();
    displayMusicTitle();
}

void AppControl::displayMusicStop()
{
    // 音楽停止画面を描画する
    mlcd.displayJpgImageCoordinate(MUSIC_NOWSTOPPING_IMG_PATH, MUSIC_NOTICE_X_CRD, MUSIC_NOTICE_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_PLAY_IMG_PATH, MUSIC_PLAY_X_CRD, MUSIC_PLAY_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_BACK_IMG_PATH, MUSIC_BACK_X_CRD, MUSIC_BACK_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_NEXT_IMG_PATH, MUSIC_NEXT_X_CRD, MUSIC_NEXT_Y_CRD);
}

void AppControl::displayMusicTitle()
{
    // 関数 MdMusicPlayer::getTitle()により音楽ファイルのファイル名を取得し、それを描画する
    mlcd.displayText(mmplay.getTitle(), MUSIC_TITLE_X_CRD, MUSIC_TITLE_Y_CRD);
}

void AppControl::displayNextMusic() // 次の音楽ファイル開き、そのファイル名を描画する
{
    mmplay.selectNextMusic();
    displayMusicTitle();
}

void AppControl::displayMusicPlay()
{
    // 音楽再生画面を描画する
    mlcd.fillBackgroundWhite();
    mlcd.displayJpgImageCoordinate(MUSIC_NOWPLAYING_IMG_PATH, MUSIC_NOTICE_X_CRD, MUSIC_NOTICE_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_STOP_IMG_PATH, MUSIC_STOP_X_CRD, MUSIC_STOP_Y_CRD);
}

void AppControl::displayMeasureInit()
{
    mlcd.fillBackgroundWhite();
    mlcd.displayJpgImageCoordinate(MEASURE_NOTICE_IMG_PATH, MEASURE_NOTICE_X_CRD, MEASURE_NOTICE_Y_CRD);
    mlcd.displayJpgImageCoordinate(MEASURE_CM_IMG_PATH, MEASURE_CM_X_CRD, MEASURE_CM_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_BACK_IMG_PATH, MEASURE_BACK_X_CRD, MEASURE_BACK_Y_CRD);
    displayMeasureDistance();
}

void AppControl::displayMeasureDistance()
{
    double distance1 = mmdist.getDistance();
    int distance2 = distance1 * 10;

    int ahundred = (distance2 / 1000) % 10;
    int ten = (distance2 / 100) % 10;
    int one = (distance2 / 10) % 10;
    int first_decimal = distance2 % 10;

    Serial.println(distance1);
    if (distance2 >= 20 && distance2 < 450)
    {
        // 百の位
        mlcd.displayJpgImageCoordinate(g_str_blue[ahundred], MEASURE_DIGIT3_X_CRD, MEASURE_DIGIT3_Y_CRD);
        if (ahundred == 0) // 百の値が0の場合画像を白に変える
        {
            mlcd.displayJpgImageCoordinate(COMMON_BLUEFILLWHITE_IMG_PATH, MEASURE_DIGIT3_X_CRD, MEASURE_DIGIT3_Y_CRD);
        }
        // 十の位
        mlcd.displayJpgImageCoordinate(g_str_blue[ten], MEASURE_DIGIT2_X_CRD, MEASURE_DIGIT2_Y_CRD);
        if (ahundred == 0 && ten == 0) // 十の位が0の場合画像を白に変える
        {
            mlcd.displayJpgImageCoordinate(COMMON_BLUEFILLWHITE_IMG_PATH, MEASURE_DIGIT2_X_CRD, MEASURE_DIGIT2_Y_CRD);
        }
        // 一の位
        mlcd.displayJpgImageCoordinate(g_str_blue[one], MEASURE_DIGIT1_X_CRD, MEASURE_DIGIT1_Y_CRD);
        if (ahundred == 0 && ten == 0 && one == 0) // 一の位が0の場合画像を白に変える
        {
            mlcd.displayJpgImageCoordinate(COMMON_BLUEFILLWHITE_IMG_PATH, MEASURE_DIGIT1_X_CRD, MEASURE_DIGIT1_Y_CRD);
        }
        mlcd.displayJpgImageCoordinate(COMMON_BLUEDOT_IMG_PATH, MEASURE_DOT_X_CRD, MEASURE_DOT_Y_CRD);
        mlcd.displayJpgImageCoordinate(g_str_blue[first_decimal], MEASURE_DECIMAL_X_CRD, MEASURE_DECIMAL_Y_CRD);
    }
                mlcd.displayJpgImageCoordinate(COMMON_BLUEFILLWHITE_IMG_PATH, MEASURE_DIGIT3_X_CRD, MEASURE_DIGIT3_Y_CRD);
            mlcd.displayJpgImageCoordinate(COMMON_BLUEFILLWHITE_IMG_PATH, MEASURE_DIGIT2_X_CRD, MEASURE_DIGIT2_Y_CRD);
            mlcd.displayJpgImageCoordinate(COMMON_BLUEFILLWHITE_IMG_PATH, MEASURE_DIGIT1_X_CRD, MEASURE_DIGIT1_Y_CRD);


}

void AppControl::displayDateInit()
{
    mlcd.displayJpgImageCoordinate(DATE_NOTICE_IMG_PATH, DATE_NOTICE_X_CRD, DATE_NOTICE_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_BACK_IMG_PATH, DATE_BACK_X_CRD, DATE_BACK_Y_CRD);
}

void AppControl::displayDateUpdate()
{
    // 関数 MdDateTime::readDate()とMdDateTime::readTime()により現在の日時を取得し、描画する
    mlcd.displayDateText(mdtime.readDate(), DATE_YYYYMMDD_X_CRD, DATE_YYYYMMDD_Y_CRD);
    mlcd.displayDateText(mdtime.readTime(), DATE_HHmmSS_X_CRD, DATE_HHmmSS_Y_CRD);
}

void AppControl::controlApplication()
{
    mmplay.init();
    while (1) // 無限ループ
    {
        switch (getState())
        {
        case TITLE:
            switch (getAction())
            {
            case ENTRY:
                displayTitleInit(); // タイトル画面表示
                /*
                ** まずはここにタイトル画面の表示処理を呼び出してみよう。
                ** タイトル画面表示の関数はdisplayTitleInit()である。
                ** この関数の中身はまだ何もないので、この関数にタイトル画面表示処理を書いてみよう。
                */
                setStateAction(TITLE, DO);
                break;

            case DO:
                // ボタンを押したらメニューに遷移
                if (m_flag_btnA_is_pressed || m_flag_btnB_is_pressed || m_flag_btnC_is_pressed)
                {
                    setStateAction(TITLE, EXIT);
                }
                break;

            case EXIT:
                setBtnAllFlgFalse(); // ボタンをfalseにセット
                mlcd.clearDisplay(); // 画面をクリア

                setStateAction(MENU, ENTRY);
                break;

            default:
                break;
            }
            break;

        case MENU:

            switch (getAction())
            {
            case ENTRY:
                displayMenuInit(); // メニュー画面を表示
                setStateAction(MENU, DO);
                break;

            case DO:
                if (m_flag_btnA_is_pressed) // 左ボタン「↑」を押したときフォーカスchangeの
                {
                    switch (m_focus_state)
                    {
                    case MENU_WBGT:
                        focusChangeImg(m_focus_state, MENU_DATE);
                        m_focus_state = MENU_DATE;
                        break;
                    case MENU_MUSIC:
                        focusChangeImg(m_focus_state, MENU_WBGT);
                        m_focus_state = MENU_WBGT;
                        break;
                    case MENU_MEASURE:
                        focusChangeImg(m_focus_state, MENU_MUSIC);
                        m_focus_state = MENU_MUSIC;
                        break;
                    case MENU_DATE:
                        focusChangeImg(m_focus_state, MENU_MEASURE);
                        m_focus_state = MENU_MEASURE;
                        break;
                    }
                    setBtnAllFlgFalse();
                }
                else if (m_flag_btnB_is_pressed)
                {
                    setStateAction(MENU, EXIT);
                }
                else if (m_flag_btnC_is_pressed)
                {
                    switch (m_focus_state)
                    {
                    case MENU_WBGT:
                        focusChangeImg(m_focus_state, MENU_MUSIC);
                        m_focus_state = MENU_MUSIC;
                        break;
                    case MENU_MUSIC:
                        focusChangeImg(m_focus_state, MENU_MEASURE);
                        m_focus_state = MENU_MEASURE;
                        break;
                    case MENU_MEASURE:
                        focusChangeImg(m_focus_state, MENU_DATE);
                        m_focus_state = MENU_DATE;
                        break;
                    case MENU_DATE:
                        focusChangeImg(m_focus_state, MENU_WBGT);
                        m_focus_state = MENU_WBGT;
                        break;
                    }
                    setBtnAllFlgFalse();
                }
                break;

            case EXIT:
                switch (m_focus_state)
                {
                case MENU_WBGT:
                    setStateAction(WBGT, ENTRY);
                    break;
                case MENU_MUSIC:
                    setStateAction(MUSIC_STOP, ENTRY);
                    break;
                case MENU_MEASURE:
                    setStateAction(MEASURE, ENTRY);
                    break;
                case MENU_DATE:
                    setStateAction(DATE, ENTRY);
                    break;
                }
                mlcd.clearDisplay();
                setBtnAllFlgFalse(); // ボタンをfalseにセット
            default:
                break;
            }

            break;

        case WBGT:

            switch (getAction())
            {
            case ENTRY:
                // 熱中症モニタ画面表示 温度・湿度・熱中症アラート表示
                displayWBGTInit();

                break;

            case DO:
                // 100ms毎に温度、湿度、熱中症アラート表示を更新
                break;

            case EXIT:
                break;
                mlcd.clearDisplay();
                setBtnAllFlgFalse();

            default:
                break;
            }

            break;

        case MUSIC_STOP:
            switch (getAction())
            {
            case ENTRY:
                // 停止画面表示現在の曲表示
                displayMusicInit();
                setStateAction(MUSIC_STOP, DO);
                break;

            case DO:
                if (m_flag_btnA_is_pressed || m_flag_btnB_is_pressed)
                {
                    setStateAction(MUSIC_STOP, EXIT);
                }
                else if (m_flag_btnC_is_pressed)
                {
                    displayNextMusic();
                    setBtnAllFlgFalse();
                }
                break;

            case EXIT:
                if (m_flag_btnA_is_pressed)
                {
                    setStateAction(MUSIC_PLAY, ENTRY);
                    setBtnAllFlgFalse();
                }
                // 中ボタンを押下 → MENU
                else if (m_flag_btnB_is_pressed)
                {
                    setBtnAllFlgFalse();
                    m_focus_state = MENU_WBGT;
                    setStateAction(MENU, ENTRY);
                }
                break;
            default:
                break;
            }

            break;

        case MUSIC_PLAY:

            switch (getAction())
            {
            case ENTRY:
                // 音楽再生
                displayMusicPlay();
                mmplay.prepareMP3();
                while (!(m_flag_btnA_is_pressed || mmplay.playMP3()))
                {
                    mmplay.playMP3();
                }
                setStateAction(MUSIC_PLAY, DO);
                break;

            case DO:
                // 音楽停止
                // 左ボタンを押下または音楽再生終了時 → MUSIC_STOP
                while (mmplay.isRunningMP3())
                {
                    if (!mmplay.playMP3() || m_flag_btnA_is_pressed)
                    {
                        // 以下3行ほど記述
                        mmplay.stopMP3();
                        setBtnAllFlgFalse();
                        setStateAction(MUSIC_STOP, EXIT);
                    }
                }
                break;

            case EXIT:
                mlcd.clearDisplay();
                setStateAction(MUSIC_PLAY, ENTRY);
                break;

            default:
                break;
            }

            break;

        case MEASURE:

            switch (getAction())
            {
            case ENTRY:
                // 距離測定画面表示
                // 測定距離表示
                displayMeasureInit();
                setStateAction(MEASURE, DO);
                break;

            case DO:
                // 250ms毎に測定距離表示を更新
                displayMeasureDistance();
                delay(250);
                //(分岐)距離測定フォーカス時に中ボタンを押下
                if (m_flag_btnB_is_pressed)
                {
                    setStateAction(MEASURE, EXIT);
                }
                break;

            case EXIT:

                mlcd.clearDisplay();
                setBtnAllFlgFalse();
                setStateAction(MENU, ENTRY);
                break;

            default:
                break;
            }

            break;

        case DATE:

            switch (getAction())
            {
            case ENTRY:
                // 時刻表示画面表示
                mlcd.fillBackgroundWhite(); // 背景色を白に設定
                displayDateInit();
                setStateAction(DATE, DO);
                break;

            case DO:
                // 100ms毎に時刻表示を更新
                displayDateUpdate();
                delay(100);
                // 中ボタンを押下→メニュー
                if (m_flag_btnB_is_pressed)
                {
                    mmplay.stopMP3();
                    setStateAction(DATE, EXIT);
                }
                break;

            case EXIT:
                setStateAction(MENU, ENTRY);
                setBtnAllFlgFalse(); // ボタンをfalseにセット
                mlcd.clearDisplay();
                m_focus_state = MENU_WBGT;
                break;

            default:
                break;
            }

        default:
            break;
        }
    }
}
