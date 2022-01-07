#include "ctfolder.hpp"
#include "ctffmpeg.hpp"

CtFfmpeg *ff;

#include <fmt/core.h>

int main(int argc, char **argv)
{

    // if (argc < 2)
    // {
    //     printf("usage: %s <RTSP link>  \n", argv[0]);
    //     return 1;
    // }

    ff = new CtFfmpeg();

    // ff->in_filename = argv[1]; //RTSP input link
    ff->in_filename = "rtsp://admin:hk123456@192.168.104.72:554/Streaming/Channels/101";
    ff->init();
    ff->getInput();

    string filename;

    videoRecorder obj;
    int start, now;
    start = obj.get_current_min();

    if (obj.create_folder(0755))
        cout << "Cannot create folder, maybe it already exists" << endl;
    else
        cout << "Create folder succesfully" << endl;

    int skip = 0;

    while (1)
    {

        filename = obj.update_filename();
        ff->getOutputName(filename.c_str());

        int64_t pts_offset = AV_NOPTS_VALUE;

        while ((now = obj.get_current_min()) == start)
        {
            ff->ret = av_read_frame(ff->ifmt_ctx, &(ff->pkt));
            skip++;
            if (skip == 1)
                continue;

            if (skip > 2)
                skip = 2;
            if (ff->ret < 0)
                continue;
            ff->pkt.pts = av_rescale_q_rnd(ff->pkt.pts, ff->in_stream->time_base, ff->out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            if (pts_offset == AV_NOPTS_VALUE)
            {
                pts_offset = ff->pkt.pts;
            }
            ff->pkt.pts -= pts_offset;
            ff->pkt.dts = av_rescale_q_rnd(ff->pkt.dts, ff->in_stream->time_base, ff->out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            ff->pkt.duration = av_rescale_q(ff->pkt.duration, ff->in_stream->time_base, ff->out_stream->time_base);

            ff->pkt.pos = -1;
            ff->ret = av_interleaved_write_frame(ff->ofmt_ctx, &(ff->pkt));
            if (ff->ret < 0)
            {
                fprintf(stderr, "Error muxing packet\n");
                continue;
            }
            av_free_packet(&(ff->pkt));
        }
        ff->release();

        cout << "New minute!" << endl;

        if (obj.create_folder(0755))
            cout << "Cannot create folder, something's wrong" << endl;
        else
            cout << "Create folder succesfully" << endl;
        start = now;
    }

    return 0;
}