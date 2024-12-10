#include "MdMusicPlayer.h"
#include "AudioFileSourceID3.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3* mp3;
AudioFileSourceSD* file;
AudioOutputI2S* out;
AudioFileSourceID3* id3;

File musicfolder;
File entry;

void MdMusicPlayer::init()
{
//musicフォルダを開き、次に再生する楽曲を グローバル変数entry にセットする。
    SD.begin();
    musicfolder = SD.open("/music");
    if (!musicfolder) {
        // musicフォルダが無かった場合、開けなかった場合
        Serial.println("musicfolder open error");
    } else {
        entry = musicfolder.openNextFile();
    }
}

char* MdMusicPlayer::getTitle()
{
    //グローバル変数entry　にセットされた楽曲のファイル名を取得し、それを返す。
    return (char*)entry.name();
}

void MdMusicPlayer::selectNextMusic()
{
    //グローバル変数entry に次の楽曲をセットする。次の音楽ファイルがない場合、フォルダの先頭に戻り一番最初のファイルを次の楽曲としてセットする。
    entry = musicfolder.openNextFile();
    if (!entry) {
        musicfolder.rewindDirectory();
        entry = musicfolder.openNextFile();
        Serial.println("return latestfile");
    }
}

void MdMusicPlayer::prepareMP3()
{
    //グローバル変数file, id3, out, mp3 の各クラスのインスタンスを生成し、そのアドレスを格納する。
    //音楽ファイルのデコードを開始する
    file = new AudioFileSourceSD(entry.path());
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(0, 1); // Output to builtInDAC
    out->SetOutputModeMono(true);
    out->SetGain(0.045);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);
}

bool MdMusicPlayer::isRunningMP3()
{
    //音楽ファイルが現在デコード中かを確認し、デコード中ならtrue、そうでないならfalseを返す。
    return mp3->isRunning();
}

bool MdMusicPlayer::playMP3()
{
    //音楽ファイルを再生するために楽曲データのデコードを行う。
    //次のデータがある場合はtrue、データがない（楽曲の終了時）はfalseを返す
    return mp3->loop();
}

void MdMusicPlayer::stopMP3()
{
    //音楽ファイルのデコードを停止する。
    mp3->stop();
}
