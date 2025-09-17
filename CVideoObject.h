#ifndef C_VIDEOOBJECT_H
#define C_VIDEOOBJECT_H

#include <SFML/Graphics.hpp>

extern "C" 
{
#include <ffmpeg/libavformat/avformat.h>
#include <ffmpeg/libswscale/swscale.h>
#include <ffmpeg/libavutil/imgutils.h>
}

#include <iostream>

#include <thread>
#include <atomic>


namespace KGF
{
	class CVideoObject
	{
	public:
		CVideoObject() = default;
		~CVideoObject() = default;

		void Load(std::string filename);
		void Update(unsigned short fpsleep, bool isLoop);
		void Clean();

		void Start(unsigned short fpssleep, bool isLoop);
		void Stop();

		sf::Texture texture;
		
		
	private:
		void setUp();
		void threadFunc(unsigned short fpssleep, bool isLoop);

		std::thread workerThread;
		std::atomic<bool> running{ false };

		AVFormatContext* fmt_ctx;
		AVCodecParameters* codecpar;
		int video_stream_index = -1;
		AVCodec* codec;
		AVCodecContext* codec_ctx;
		SwsContext* sws_ctx;
		AVFrame* frame;
		AVFrame* frame_bgra;
		int num_bytes;
		uint8_t* buffer;
		AVPacket* pkt;

		

	};
}

#endif