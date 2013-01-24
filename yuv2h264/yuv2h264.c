#include <stdio.h>
#include <stdint.h>
#include <x264.h>

static const int width = 1280;
static const int height = 720;

#define MFILE "/mnt/hgfs/E/temp/dongfang"

int main(int argc, char* argv[]){
    x264_param_t param;
    x264_t *h = NULL;
    x264_picture_t pic_in;
    x264_picture_t pic_out;
    x264_nal_t *nal;
    uint8_t *data = NULL;
    int widthXheight = width * height;
    int frame_size = width * height * 1.5;
    int read_sum = 0, write_sum = 0;
    int frames = 0;
    int i, rnum, i_size;
    x264_nal_t* pNals = NULL;

    x264_param_default(&param);
    param.i_width = width;
    param.i_height = height;
    param.i_bframe = 3;
    param.i_fps_num = 25;
    param.i_fps_den = 1;
    param.b_vfr_input = 0;
    param.i_keyint_max = 250;
    param.rc.i_bitrate = 1500;
    param.i_scenecut_threshold = 40;
    param.i_level_idc = 51;

    x264_param_apply_profile(&param, "high");

    h = x264_encoder_open( &param );

//    printf("param.rc.i_qp_min=%d, param.rc.i_qp_max=%d, param.rc.i_qp_step=%d param.rc.i_qp_constant=%d param.rc.i_rc_method=%d\n",
//            param.rc.i_qp_min, param.rc.i_qp_max, param.rc.i_qp_step, param.rc.i_qp_constant, param.rc.i_rc_method);
    printf("param:%s\n", x264_param2string(&param, 1));


    x264_picture_init( &pic_in );
    x264_picture_alloc(&pic_in, X264_CSP_YV12, width, height);
    pic_in.img.i_csp = X264_CSP_YV12;
    pic_in.img.i_plane = 3;

    data = (uint8_t*)malloc(0x400000);

    FILE* fpr = fopen(MFILE ".yuv", "rb");
    FILE* fpw1 = fopen(MFILE".szhu.h264", "wb");
//    FILE* fpw2 = fopen(MFILE".h264", "wb");

    if(!fpr || !fpw1 ) {
        printf("file open failed\n");
        return -1;
    }

    while(!feof(fpr)){
        rnum = fread(data, 1, frame_size, fpr);
        if(rnum != frame_size){
            printf("read file failed\n");
            break;
        }
        memcpy(pic_in.img.plane[0], data, widthXheight);
        memcpy(pic_in.img.plane[1], data + widthXheight, widthXheight >> 2);
        memcpy(pic_in.img.plane[2], data + widthXheight + (widthXheight >> 2), widthXheight >> 2);
        read_sum += rnum;
        frames ++;
//        printf("read frames=%d %.2fMB write:%.2fMB\n", frames, read_sum * 1.0 / 0x100000, write_sum * 1.0 / 0x100000);
        int i_nal;
        int i_frame_size = 0;

        if(0 && frames % 12 == 0){
            pic_in.i_type = X264_TYPE_I;
        }else{
            pic_in.i_type = X264_TYPE_AUTO;
        }
        i_frame_size = x264_encoder_encode( h, &nal, &i_nal, &pic_in, &pic_out );

        if(i_frame_size <= 0){
            //printf("\t!!!FAILED encode frame \n");
        }else{
            fwrite(nal[0].p_payload, 1, i_frame_size, fpw1);
//            printf("\t+++i_frame_size=%d\n", i_frame_size);
            write_sum += i_frame_size;
        }
#if 0
        for(i = 0; i < i_nal; i ++){
            i_size = nal[i].i_payload;
//            fwrite(nal[i].p_payload, 1, nal[i].i_payload, fpw1);
            fwrite(nal[i].p_payload, 1, i_frame_size, fpw1);
            x264_nal_encode(h, data, &nal[i]);
            if(i_size != nal[i].i_payload){
                printf("\t\ti_size=%d nal[i].i_payload=%d\n", i_size, nal[i].i_payload);
            }
            fwrite(data, 1, nal[i].i_payload, fpw2);
        }
#endif
    }

    free(data);
    x264_picture_clean(&pic_in);
    x264_picture_clean(&pic_out);
    if(h){
        x264_encoder_close(h);
        h = NULL;
    }
    fclose(fpw1);
//    fclose(fpw2);
    fclose(fpr);
    printf("h=0x%X", h);
    return 0;
}
