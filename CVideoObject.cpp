#include "CVideoObject.h"

namespace KGF
{

	void CVideoObject::Load(std::string filename)
	{
		if (avformat_open_input(&fmt_ctx, filename.c_str(), nullptr, nullptr) != 0) 
		{
			std::cerr << "Не удалось открыть файл" << std::endl;

		}


		setUp();

		texture.resize(sf::Vector2u(codec_ctx->width, codec_ctx->height));
	}

	void CVideoObject::Update(unsigned short fpsleep, bool isLoop)
	{
		if (av_read_frame(fmt_ctx, pkt) >= 0)
		{
			if (pkt->stream_index == video_stream_index)
			{
				if (avcodec_send_packet(codec_ctx, pkt) == 0)
				{
					while (avcodec_receive_frame(codec_ctx, frame) == 0)
					{
						// -- Преобразование в BGRA
						sws_scale(sws_ctx, frame->data, frame->linesize, 0, codec_ctx->height,
							frame_bgra->data, frame_bgra->linesize);

						texture.update(frame_bgra->data[0]);

						// -- Типа контроллер скорости, придумал только это :/
						sf::sleep(sf::milliseconds(fpsleep));
					}
				}
			}
			av_packet_unref(pkt);
		}
		else 
		{
			if (isLoop)
			{
				av_seek_frame(fmt_ctx, video_stream_index, 0, AVSEEK_FLAG_BACKWARD);
			}
		}
	}

	void CVideoObject::Start(unsigned short fpssleep, bool isLoop)
	{
		if (running) return; 
		running = true;
		workerThread = std::thread(&CVideoObject::threadFunc, this, fpssleep, isLoop);
	}

	void CVideoObject::Stop()
	{
		if (running)
		{
			running = false;
			if (workerThread.joinable())
				workerThread.join();
		}
	}

	void CVideoObject::threadFunc(unsigned short fpssleep, bool isLoop)
	{
		while (running)
		{
			Update(fpssleep, isLoop);
		}
	}

	void CVideoObject::setUp()
	{
		if (avformat_find_stream_info(fmt_ctx, nullptr) < 0)
		{
			std::cerr << "Не удалось найти информацию о потоках" << std::endl;
			avformat_close_input(&fmt_ctx);
		}


		for (unsigned int i = 0; i < fmt_ctx->nb_streams; i++)
		{
			if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			{
				video_stream_index = i;
				codecpar = fmt_ctx->streams[i]->codecpar;
				break;
			}
		}

		if (video_stream_index == -1)
		{
			std::cerr << "Видео поток не найден" << std::endl;
			avformat_close_input(&fmt_ctx);
		}

		codec = avcodec_find_decoder(codecpar->codec_id);
		if (!codec)
		{
			std::cerr << "Декодер не найден" << std::endl;
			avformat_close_input(&fmt_ctx);
		}

		codec_ctx = avcodec_alloc_context3(codec);

		avcodec_parameters_to_context(codec_ctx, codecpar);

		if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
		{
			std::cerr << "Не удалось открыть декодер" << std::endl;
			avcodec_free_context(&codec_ctx);
			avformat_close_input(&fmt_ctx);
		}

		sws_ctx = sws_getContext
		(
			codec_ctx->width, codec_ctx->height, codec_ctx->pix_fmt,
			codec_ctx->width, codec_ctx->height, AV_PIX_FMT_BGRA,
			SWS_BILINEAR, nullptr, nullptr, nullptr
		);

		frame = av_frame_alloc();
		frame_bgra = av_frame_alloc();

		num_bytes = av_image_get_buffer_size(AV_PIX_FMT_BGRA, codec_ctx->width, codec_ctx->height, 1);

		buffer = (uint8_t*)av_malloc(num_bytes * sizeof(uint8_t));

		av_image_fill_arrays(frame_bgra->data, frame_bgra->linesize, buffer, AV_PIX_FMT_BGRA, codec_ctx->width, codec_ctx->height, 1);
		pkt = av_packet_alloc();
	}


	void CVideoObject::Clean()
	{
		av_frame_free(&frame);
		av_frame_free(&frame_bgra);
		avformat_close_input(&fmt_ctx);
		avcodec_free_context(&codec_ctx);
		sws_freeContext(sws_ctx);
		av_free(buffer);
		av_packet_free(&pkt);
	}

}