#include<stdio.h>
#include<stdlib.h>

#include<libxml/parser.h>
#include<libxml/tree.h>

#define spc(level) PrintSpace(level)

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
    xmlDocPtr doc = xmlParseFile("List.xml");
    if(doc == NULL)
    {
        printf("Error: Can not open List.xml\n");
        exit(1);
    }

    //找到首结点
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if(root == NULL)
    {
        printf("Error: Can not find the root!\n");
        exit(1);
    }

    xmlNodePtr person; //Person结点指针
    xmlNodePtr achivm; //Achievement结点指针

    //遍历一个Person结点
    spc(0); printf("Name: %s\n", root -> name);

    person = root -> children;

    //逐个找出属性
    spc(1); printf("Node: %s\n", person -> name);
    spc(2); printf("Id: %s\n", xmlGetProp(person, BAD_CAST "Id"));
    spc(2); printf("Name: %s\n", xmlGetProp(person, BAD_CAST "Name"));
    spc(2); printf("Sex: %s\n", xmlGetProp(person, BAD_CAST "Sex"));
    spc(2); printf("Age: %s\n", xmlGetProp(person, BAD_CAST "Age"));

    //用循环遍历子节点，打印内容
    for(achivm = person -> children; achivm; achivm = achivm -> next)
    {
        spc(3);
        printf("Node: %s\t", achivm -> name);
        printf("Content: %s\t", (char*)xmlNodeGetContent(achivm));
        printf("End\n");
    }

    spc(1); printf("End\n");

    //一个Person结点遍历结束
    //转到下一个Person结点
    person = person -> next;

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

    spc(1); printf("End\n");

    spc(0); printf("End\n");

    xmlFree(achivm);
    xmlFree(person);
    xmlFree(root);
    xmlFree(doc);
    xmlCleanupParser();

    return 0;
}