extern "C"{
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
}

class CtFfmpeg {
public:

    CtFfmpeg();
    ~CtFfmpeg();

    void init();
    int getInput();
    int getOutputName(const char *filename);
    int release();
    int ret;
    AVFormatContext *ifmt_ctx, *ofmt_ctx;
    AVStream *in_stream, *out_stream;
    AVPacket pkt;
    const char *in_filename;
    char *out_filename;

private:
    int setOutput(const char *outfilename);
    AVOutputFormat *ofmt;
};