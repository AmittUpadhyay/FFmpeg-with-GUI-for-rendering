
extern "C" {
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
#include <libavutil/frame.h>
#include <stdint.h>
#include <libavutil\pixfmt.h>
}


class FFmpegEncoder
{
public:
	struct Params
	{
		uint32_t width;
		uint32_t height;
		double fps;
		uint32_t bitrate;
		const char *preset;

		uint32_t crf; //0–51

		enum AVPixelFormat src_format;
		enum AVPixelFormat dst_format;
	};
	
	FFmpegEncoder() = default;
    /*Constructor: Initializes the encoder by calling the Open method with the provided filename and parameters.*/
	FFmpegEncoder(const char *filename, const Params &params);
	/*Destructor: Cleans up resources by calling the Close method.*/
	~FFmpegEncoder();
	/*Opens the encoder and sets up the necessary FFmpeg structures for encoding video.
	Allocates the output format context.
	Finds and sets up the H.264 encoder.
	Creates a new stream for the video.
	Allocates and configures the codec context with parameters like bitrate, resolution, frame rate, and pixel format.
	Initializes the frame and conversion context.
	Opens the output file and writes the header.*/
	bool Open(const char *filename, const Params &params);

	/*Closes the encoder and releases all allocated resources.
	Sends a null frame to flush the encoder.
	Writes the trailer to finalize the file.
	Closes the output file.
	Frees the conversion context, frame, codec context, and format context.*/
	void Close();

	/*Purpose: Encodes a frame of video data.
	Makes the frame writable.
	Converts the input data to the desired pixel format.
	Sends the frame to the encoder.
	Flushes the encoded packets to the output file*/
	bool Write(const unsigned char *data);
    /*Purpose: Checks if the encoder is currently open.
	Returns: true if the encoder is open, false otherwise.*/
	bool IsOpen() const;

private:
	/*Flushes the encoded packets from the encoder to the output file.
	Receives encoded packets from the encoder.
	Rescales the packet timestamps.
	Writes the packets to the output file.
	Unreferences the packet to free its resources.*/
	bool FlushPackets();

private:
	bool mIsOpen = false;

	struct Context
	{
		struct AVFormatContext *format_context = nullptr;
		struct AVStream *stream = nullptr;
		struct AVCodecContext *codec_context = nullptr;
		struct AVFrame *frame = nullptr;
		struct SwsContext *sws_context = nullptr;
		const struct AVCodec *codec = nullptr;
		
		uint32_t frame_index = 0;
	};

	Context mContext = {};
};