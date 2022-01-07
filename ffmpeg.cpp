#include "ctffmpeg.hpp"

CtFfmpeg::CtFfmpeg() {
    in_filename = new char [1024];
    out_filename = new char [1024];
}

CtFfmpeg::~CtFfmpeg() {
    delete [] in_filename;
    delete [] out_filename;
}

void CtFfmpeg::init() {
    avcodec_register_all();
    av_register_all();
    avformat_network_init();
    pkt = { 0 };

    av_init_packet(&pkt);
    ofmt = NULL;
    ifmt_ctx = NULL;
    ofmt_ctx = NULL;
    return;
}

int CtFfmpeg::release() {
    av_write_trailer(ofmt_ctx);
    avcodec_close(out_stream->codec);

    // avcodec_close(in_stream->codec);
    // avformat_close_input(&ifmt_ctx);

    /* close output */
    if (!(ofmt->flags & AVFMT_NOFILE))
        avio_close(ofmt_ctx->pb);

    avformat_free_context(ofmt_ctx);
    av_free_packet(&pkt);
    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred\n");
        return 1;
    }

    return 0;
}

int CtFfmpeg::getInput() {
    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
        fprintf(stderr, "Could not open input file '%s'", in_filename);
        release();
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
        fprintf(stderr, "Failed to retrieve input stream information");
        release();
    }

    av_dump_format(ifmt_ctx, 0, in_filename, 0);

    return 0;
}


int CtFfmpeg::setOutput(const char *outfilename) {
    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, outfilename);
    if (!ofmt_ctx) {
        fprintf(stderr, "Could not create output context\n");
        ret = AVERROR_UNKNOWN;
        release();
    }

    ofmt = ofmt_ctx->oformat;
    for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
        in_stream = ifmt_ctx->streams[i];
        out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);

        if (!out_stream) {
             fprintf(stderr, "Failed allocating output stream\n");
             ret = AVERROR_UNKNOWN;
             release();
        }
        ret = avcodec_copy_context(out_stream->codec, in_stream->codec);

        if (ret < 0) {
            fprintf(stderr, "Failed to copy context from input to output stream codec context\n");
            release();
        }

        out_stream->codec->codec_tag = 0;
        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
            out_stream->codec->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    } // for

    av_dump_format(ofmt_ctx, 0, outfilename, 1);
    if (!(ofmt->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, outfilename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", outfilename);
            release();
        }
    }
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
        release();
    }

    return 0;
}

int CtFfmpeg::getOutputName(const char *filename){
    sprintf(out_filename,filename);
    setOutput(out_filename);
    return 0;
}