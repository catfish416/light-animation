#include<stdio.h>

#include<libxml/parser.h>
#include<libxml/tree.h>

int main()
{
    //建立XML文档和根结点
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    xmlNodePtr root = xmlNewNode(NULL, BAD_CAST"List");

    //将根节点绑定到XML文档
    xmlDocSetRootElement(doc, root);

    //建立Person结点，为其安装四个属性
    xmlNodePtr nodeTsybius = xmlNewNode(NULL, BAD_CAST"Person");
    xmlNewProp(nodeTsybius, BAD_CAST"Id", BAD_CAST"1001");
    xmlNewProp(nodeTsybius, BAD_CAST"Name", BAD_CAST"Tsybius");
    xmlNewProp(nodeTsybius, BAD_CAST"Sex", BAD_CAST"Male");
    xmlNewProp(nodeTsybius, BAD_CAST"Age", BAD_CAST"23");
    xmlAddChild(root, nodeTsybius);

    //在Person结点下安放子结点，并为子节点添加内容
    xmlNewTextChild(nodeTsybius, NULL, BAD_CAST"Achievement", BAD_CAST"ABC Rank 2");
    xmlNewTextChild(nodeTsybius, NULL, BAD_CAST"Achievement", BAD_CAST"DEF Rank 4");

    xmlNodePtr nodeGalatea = xmlNewNode(NULL, BAD_CAST"Person");
    xmlNewProp(nodeGalatea, BAD_CAST"Id", BAD_CAST"1002");
    xmlNewProp(nodeGalatea, BAD_CAST"Name", BAD_CAST"Galatea");
    xmlNewProp(nodeGalatea, BAD_CAST"Sex", BAD_CAST"Female");
    xmlNewProp(nodeGalatea, BAD_CAST"Age", BAD_CAST"21");
    xmlAddChild(root, nodeGalatea);

    xmlNewTextChild(nodeGalatea, NULL, BAD_CAST"Achievement", BAD_CAST"ABC Rank 1");
    xmlNewTextChild(nodeGalatea, NULL, BAD_CAST"Achievement", BAD_CAST"XYZ Rank 2");
    xmlNewTextChild(nodeGalatea, NULL, BAD_CAST"Achievement", BAD_CAST"MNOP Rank 5");

    //保存XML文档
    int nRel = xmlSaveFile("List.xml", doc);
    if(nRel != -1)
    {
        printf("List.xml: Created Successfully!\n");
    }

    //释放资源
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return 0;
}