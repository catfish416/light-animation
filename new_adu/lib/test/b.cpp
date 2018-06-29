#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<libxml/parser.h>
#include<libxml/tree.h>

#define spc(level) PrintSpace(level)
#define str_xml_file            "templet_0525.xml"

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


//输出缩进用的空格（4个）
void PrintSpace(int level)
{
    for(int counter = 0; counter < level; counter++)
    {
        printf("    ");
    }
}

//主函数
int main()
{
    //打开XML文件
    xmlDocPtr doc = xmlParseFile(str_xml_file);
    if (doc == NULL) {
        printf("Error: Can not open List.xml\n");
        exit(1);
    }

    //找到首结点
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        printf("Error: Can not find the root!\n");
        exit(1);
    }

    xmlNodePtr frame; //frame结点指针
    xmlNodePtr data; //data结点指针

    //遍历一个frame结点
    spc(0); printf("Name: %s\n", root -> name);

    int iHeight = 0;
    int iWidth = 0;
    frame = root -> children;
    while (frame) {
        if (!strcmp((const char*)frame -> name, "text")) {
            //printf("jump space or tbl\n");
            frame = frame -> next;
            continue;
        }
        //逐个找出属性
        spc(1); printf("Node: %s\n", frame -> name);
        spc(2); printf("%s: %s\n", str_FrameId, xmlGetProp(frame, BAD_CAST str_FrameId));
        spc(2); printf("%s: %s\n", str_IntervalTime, xmlGetProp(frame, BAD_CAST str_IntervalTime));
        spc(2); printf("%s: %s\n", str_CommunicationType, xmlGetProp(frame, BAD_CAST str_CommunicationType));
        spc(2); printf("%s: %s\n", str_Channel, xmlGetProp(frame, BAD_CAST str_Channel));
        spc(2); printf("%s: %s\n", str_ImageType, xmlGetProp(frame, BAD_CAST str_ImageType));
        spc(2); printf("%s: %s\n", str_Height, xmlGetProp(frame, BAD_CAST str_Height));
        spc(2); printf("%s: %s\n", str_Width, xmlGetProp(frame, BAD_CAST str_Width));
        // 解析 data
        iHeight = atoi((const char*)xmlGetProp(frame, BAD_CAST str_Height));
        iWidth = atoi((const char*)xmlGetProp(frame, BAD_CAST str_Width));
        data = frame->children;
        while (data) {
            if (!xmlStrcmp(data->name, BAD_CAST str_Data)) {
                // 遍历   i * j个数据
                for(int i = 0; i < iHeight; i++) {
                    for (int j = 0; j < iWidth; j++) {
                        char buf[8];
                        memset(buf, 0,sizeof(buf));
                        sprintf(buf, "d%d%d", i + 1, j + 1);
                        spc(3); printf("%s: %s\n", buf, xmlGetProp(data, BAD_CAST buf));
                    }
                }
            }
            data = data->next;
        }
        spc(1); printf("End\n");

        //一个Person结点遍历结束
        //转到下一个Person结点
        frame = frame -> next;
    }
#if 0
    //查看某属性是否与某字符串相等
    if(xmlStrcmp(xmlGetProp(person, BAD_CAST "Name"), (const xmlChar*)"Galate"))
    {
        spc(1); printf(">> The next person is Galatea!\n");
    }
    else
    {
        spc(1); printf(">> The next person is not Galatea!\n");
    }

    //查看某结点是否有某属性
    if(xmlHasProp(person, BAD_CAST "Height"))
    {
        spc(1); printf(">> The node person has attribute: Height!\n");
    }
    else
    {
        spc(1); printf(">> The next person does not have attribute: Height!\n");
    }

    //修改属性（Attribute）
    xmlSetProp(person, (const xmlChar*)"Age", (const xmlChar*)"22");
    spc(1); printf(">> Change Galatea's age from 21 to 22!\n");

    //修改子结点中的内容（Content）
    xmlNodeSetContent(person -> children, (const xmlChar*) "NEW Rank 1");
    spc(1); printf(">> Change Galatea's 1st achievement to NEW Rank 1\n");

    //打印修改后的person结点
    spc(1); printf("Node: %s\n", person -> name);
    spc(2); printf("Id: %s\n", xmlGetProp(person, BAD_CAST "Id"));
    spc(2); printf("Name: %s\n", xmlGetProp(person, BAD_CAST "Name"));
    spc(2); printf("Sex: %s\n", xmlGetProp(person, BAD_CAST "Sex"));
    spc(2); printf("Age: %s\n", xmlGetProp(person, BAD_CAST "Age"));     

    for(achivm = person -> children; achivm; achivm = achivm -> next)
    {
        spc(3);
        printf("Node: %s\t", achivm -> name);
        printf("Content: %s\t", (char*)xmlNodeGetContent(achivm));
        printf("End\n");
    }
#endif
    spc(1); printf("End\n");

    spc(0); printf("End\n");

    //xmlFree(achivm);
    xmlFree(frame);
    xmlFree(root);
    xmlFree(doc);
    xmlCleanupParser();

    return 0;
}