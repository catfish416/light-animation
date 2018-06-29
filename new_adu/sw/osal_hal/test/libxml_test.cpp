#ifdef __cplusplus
extern "C" {
#endif


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>

#include<libxml/parser.h>
#include<libxml/tree.h>
#include "led_xml.h"
#include "resue.h"
#include "tsp92662_defines.h"
#include "led_uart.h"
#include "hal_uart.h"



#define str_xml_file            "data_0625.xml"

#define str_FrameId             "FrameId"
#define str_IntervalTime        "IntervalTime"
#define str_CommunicationType   "CommunicationType"
#define str_Channel             "Channel"
#define str_ImageType           "ImageType"
#define str_Height              "Height"
#define str_Width               "Width"
#define str_Data                "Data"
#define str_Line                "Line1"
#define str_single_data         "d"
#define str_X                   "x"
#define str_Y                   "y"

extern int serial_fd;


//输出缩进用的空格（4个）
void PrintSpace(int level)
{
    for(int counter = 0; counter < level; counter++)
    {
        printf("    ");
    }
}

void PrintFrameDetail(xmlNodePtr frame)
{
    //逐个打印出属性
    spc(1); printf("Node: %s\n", frame -> name);
    spc(2); printf("%s: %s\n", str_FrameId,             xmlGetProp(frame, BAD_CAST str_FrameId));
    spc(2); printf("%s: %s\n", str_IntervalTime,        xmlGetProp(frame, BAD_CAST str_IntervalTime));
    spc(2); printf("%s: %s\n", str_CommunicationType,   xmlGetProp(frame, BAD_CAST str_CommunicationType));
    spc(2); printf("%s: %s\n", str_Channel,             xmlGetProp(frame, BAD_CAST str_Channel));
    spc(2); printf("%s: %s\n", str_ImageType,           xmlGetProp(frame, BAD_CAST str_ImageType));
    spc(2); printf("%s: %s\n", str_Height,              xmlGetProp(frame, BAD_CAST str_Height));
    spc(2); printf("%s: %s\n", str_Width,               xmlGetProp(frame, BAD_CAST str_Width));

}

void getFrameProp(STRC_FRAME* to, xmlNodePtr from)
{
    to->iFrameId      = atoi((const char*)xmlGetProp(from, BAD_CAST str_FrameId));
    to->iIntervalTime = atoi((const char*)xmlGetProp(from, BAD_CAST str_IntervalTime));
    to->iCommunType  = atoi((const char*)xmlGetProp(from, BAD_CAST str_CommunicationType));
    to->iChannel     = atoi((const char*)xmlGetProp(from, BAD_CAST str_Channel));
    to->iImageType   = atoi((const char*)xmlGetProp(from, BAD_CAST str_ImageType));
    to->iHeight      = atoi((const char*)xmlGetProp(from, BAD_CAST str_Height));
    to->iWidth       = atoi((const char*)xmlGetProp(from, BAD_CAST str_Width));
}


//主函数
int main()
{
    serial_fd = init_uart();
    printf("serial_fd = %d\n", serial_fd);

    if (serial_fd < 0)
    {
        printf("init serial failed!\n");
        return -1;
    }
    //tcflush(serial_fd, TCIOFLUSH);

    //打开XML文件
    xmlDocPtr doc = xmlParseFile(str_xml_file);
    if(doc == NULL)
    {
        printf("Error: Can not open file: %s\n", str_xml_file);
        exit(1);
    }

    //找到首结点
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL)
    {
        printf("Error: Can not find the root!\n");
        exit(1);
    }

    xmlNodePtr frame = NULL; //Person结点指针
    xmlNodePtr data = NULL; //Achievement结点指针
//    xmlNodePtr curNode = NULL;

    //遍历一个Frame结点
    spc(0); printf("Name: %s\n", root -> name);

    STRC_FRAME strc_frame;
    memset(&strc_frame, 0, sizeof(strc_frame));
    uint16_t data_val = 0;
    char str_data_name[12];

    frame = root -> children;
    while (frame)
    {
        if (!strcmp((const char*)frame -> name, "text"))
        {
            //printf("jump space or tbl\n");
            frame = frame -> next;
            continue;
        }

        getFrameProp(&strc_frame, frame);
        PrintFrameDetail(frame);

        // parse data
        data = frame->children;
        while(data)
        {
            if (!xmlStrcmp(data->name, BAD_CAST str_Data))
            {
                for(int i = 0; i < strc_frame.iHeight; i++)
                {
                    for (int j = 0; j < strc_frame.iWidth; j++)
                    {
                        memset(str_data_name, 0, sizeof(str_data_name));
                        sprintf(str_data_name, "X%dY%d", i + 1, j + 1);     // data 格式必须与xml中保持一致
                        data_val = atoi((const char*)xmlGetProp(data, BAD_CAST str_data_name));
                        spc(3); printf("frameID = %d, %s: %d\n", strc_frame.iFrameId, str_data_name, data_val);
                        send_led_uart_data(WRITE1, i+1, j+1, &data_val);
                        //usleep(500 * 1000);
                        sleep(1);
                    }
                }
            }
            data = data->next;
        }
        spc(1); printf("data End\n");
        usleep(strc_frame.iIntervalTime * 1000);    // usec to msec

        //一个Frame结点遍历结束
        //转到下一个Frame结点
        frame = frame -> next;
    }
    spc(0); printf("animation frame End\n");

    xmlFree(frame);
    xmlFree(root);
    xmlFree(doc);
    xmlCleanupParser();

    if (serial_fd > 0)
        close(serial_fd);
    return 0;
}




#ifdef __cplusplus
}
#endif /* end of __cplusplus */
