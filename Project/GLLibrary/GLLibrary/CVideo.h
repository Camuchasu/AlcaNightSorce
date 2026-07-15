#pragma once
#ifdef USE_VIDEO_LIB
#include "GL.h"
#include "CImage.h"
#include "CSound.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
}

#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Mfuuid.lib")
#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libavutil.a")
#pragma comment(lib, "libswscale.a")
#pragma comment(lib, "libswresample.a")
#pragma comment(lib, "libavcodec.a")
#pragma comment(lib, "libavformat.a")
#pragma comment(lib, "libavfilter.a")
#pragma comment(lib, "libavdevice.a")

class CVideoTextrue : public CTexture {
    struct VideoReaderState {
        int width, height;
        AVRational time_base;

        AVFormatContext* av_format_ctx;
        AVCodecContext* video_codec_ctx;
        AVCodecContext* audio_codec_ctx;
        int video_stream_index;
        int audio_stream_index;
        AVFrame* av_frame;
        AVPacket* av_packet;
        SwsContext* sws_scaler_ctx;
        int frame_number;
        SwrContext* swr_ctx;
    };

    uint8_t* m_swr_buf = 0;
    int m_swr_buf_len = 0;
    float m_time;
    float m_duration;
    float m_speed_scale;
    bool m_loop;
    std::string m_file_name;
    CSoundBase* m_sound;
    VideoReaderState m_state;

    /// <summary>
    /// 動画ファイルを開く
    /// </summary>
    /// <param name="filename">動画ファイル名</param>
    /// <returns>true:成功 false:失敗</returns>
    bool video_reader_open(const char* filename);

    /// <summary>
    /// 動画フレームを読み込む
    /// </summary>
    /// <param name="frame_buffer">フレームバッファ</param>
    /// <param name="pts">表示タイムスタンプ</param>
    /// <returns>true:成功 false:失敗</returns>
    bool video_reader_read_frame(uint8_t* frame_buffer, int64_t* pts);

    /// <summary>
    /// 音声をデコード
    /// </summary>
    /// <returns>true:成功 false:失敗</returns>
    bool video_decode_audio();

    /// <summary>
    /// 指定位置へシーク
    /// </summary>
    /// <param name="ts">シーク位置</param>
    /// <returns>true:成功 false:失敗</returns>
    bool video_reader_seek_frame(int64_t ts);

    /// <summary>
    /// 動画ファイルを閉じる
    /// </summary>
    void video_reader_close();

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="filename">動画ファイル名</param>
    CVideoTextrue(const char* filename);

    /// <summary>
    /// フレームを描画
    /// </summary>
    void RenderFrame();

    /// <summary>
    /// テクスチャをマッピング
    /// </summary>
    void MapTexture();

    /// <summary>
    /// リソースを解放
    /// </summary>
    void Release();

    /// <summary>
    /// 動画を再生
    /// </summary>
    /// <param name="loop">ループ再生するか</param>
    void Play(bool loop);

    /// <summary>
    /// 動画を停止
    /// </summary>
    void Stop();

    /// <summary>
    /// 再生終了したか判定
    /// </summary>
    /// <returns>true:終了 false:再生中</returns>
    bool isEnd();

    friend class CVideo;
};

class CVideo : private CImage {
    CVideoTextrue* mp_video_texture;

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="filename">動画ファイル名</param>
    CVideo(const char* filename);

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw();

    /// <summary>
    /// 動画を再生
    /// </summary>
    /// <param name="loop">ループ再生するか</param>
    void Play(bool loop = true);

    /// <summary>
    /// 動画を停止
    /// </summary>
    void Stop();

    /// <summary>
    /// 再生終了したか判定
    /// </summary>
    /// <returns>true:終了 false:再生中</returns>
    bool isEnd() {
        return mp_video_texture->isEnd();
    }

    /// <summary>
    /// 再生中か判定
    /// </summary>
    /// <returns>true:再生中 false:停止中</returns>
    bool isPlay() {
        return mp_video_texture->m_speed_scale == 0 ? false : true;
    }

    /// <summary>
    /// サイズを設定
    /// </summary>
    /// <param name="w">幅</param>
    /// <param name="h">高さ</param>
    void SetSize(float w, float h) {
        CImage::SetSize(w, h);
    }

    /// <summary>
    /// 座標を設定
    /// </summary>
    /// <param name="x">X座標</param>
    /// <param name="y">Y座標</param>
    void SetPos(float x, float y) {
        CImage::SetPos(x, y);
    }

    /// <summary>
    /// 座標を設定
    /// </summary>
    /// <param name="pos">設定する座標</param>
    void SetPos(CVector2D pos) {
        CImage::SetPos(pos);
    }
};

#endif