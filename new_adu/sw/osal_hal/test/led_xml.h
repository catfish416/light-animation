#ifndef _LED_XML_H
#define _LED_XML_H
typedef struct frame {
    int iFrameId        = -1;
    int iIntervalTime   = 0;    // ms
    int iCommunType     = 0;
    int iChannel        = 0;
    int iImageType      = 0;
    int iHeight         = 0;
    int iWidth          = 0;
//    char str_data_val[4];
//    char str_data_name[12];
} STRC_FRAME;

#endif
