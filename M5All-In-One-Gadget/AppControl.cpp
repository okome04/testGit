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
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_DECIDE_IMG_PATH, MUSIC_BACK_X_CRD, MUSIC_BACK_Y_CRD);
    mlcd.displayJpgImageCoordinate(COMMON_BUTTON_NEXT_IMG_PATH, MUSIC_NEXT_X_CRD, MUSIC_NEXT_Y_CRD);
}

void AppControl::displayMusicTitle()
{
    // 関数 MdMusicPlayer::getTitle()により音楽ファイルのファイル名を取得し、それを描画する
    mlcd.displayText(mmplay.getTitle(), MUSIC_TITLE_X_CRD, MUSIC_TITLE_Y_CRD);
}

void AppControl::displayNextMusic()
{
}

void AppControl::displayMusicPlay()
{
}

void AppControl::displayMeasureInit()
{
}

void AppControl::displayMeasureDistance()
{
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
                // 左ボタンを押下 → MUSIC_PLAY
                if (m_flag_btnA_is_pressed)
                {
                    setBtnAFlg(true);
                    displayMusicPlay();
                }
                // 右ボタンを押下 → 次の曲を表示中
                else if (m_flag_btnC_is_pressed)
                {
                    setBtnCFlg(true);
                    displayMenuInit();
                }
                // 中ボタンを押下 → MENU
                else if (m_flag_btnB_is_pressed)
                {
                    setBtnBFlg(true);
                    displayNextMusic();
                }

                break;

            case EXIT:
                break;

            default:
                break;
            }

            break;

        case MUSIC_PLAY:

            switch (getAction())
            {
            case ENTRY:
                break;

            case DO:
                break;

            case EXIT:
                break;

            default:
                break;
            }

            break;

        case MEASURE:

            switch (getAction())
            {
            case ENTRY:
                break;

            case DO:
                break;

            case EXIT:
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
                    setStateAction(DATE, EXIT);
                }
                break;

            case EXIT:
                setStateAction(MENU, ENTRY);
                setBtnAllFlgFalse(); // ボタンをfalseにセット
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
