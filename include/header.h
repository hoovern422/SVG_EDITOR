/**
 * Name: Nickolas Hoover
 * Student number: 1024486
 * email: hoovern@uoguelph.ca
 * **/

#ifndef HEADER_H
#define HEADER_H
int buildImage(xmlNode * a_node, SVGimage *image);
xmlDocPtr buildTree(SVGimage *image, xmlNsPtr nsPtr);
void searchRectanglesInGroup(Group *group, List *rectList);
void searchGroupsInGroups(Group *parentGroup, List* rectList, char* label);
void freeShapesList(List *shapes);
void searchCirclesInGroup(Group *group, List *circList);
void searchPathsInGroup(Group *group, List *pathList);
Rectangle *createRectangle(xmlNode *node);
Circle *createCircle(xmlNode *node);
Path *createPath(xmlNode *node);
Group *createGroup(xmlNode *node);

//helper functions
void printer(xmlNodePtr root_node);
void addListsToTree(xmlNodePtr cur_node, SVGimage *image);
void addRectangleChild(xmlNodePtr cur_node, List *rectangles);
void addCircleChild(xmlNodePtr cur_node, List* circles);
void addPathChild(xmlNodePtr cur_node, List* paths);
void addGroupChild(xmlNodePtr cur_node, List *groups);
int checkAgainstSchemaFile(char *schemaFile, xmlDocPtr doc);
bool validateConstraints(SVGimage *image);
int modifyOtherAttributes(List *lptr, Attribute *attributeToAdd);
int changeAttributeFloatValue(float *value, Attribute *newAtt, char *label);
int changeAttributeString(char *string, Attribute *newAtt);
char *listToString(List *lptr, elementType type);
#endif