/**
 * Name: Nickolas Hoover
 * Student number: 1024486
 * email: hoovern@uoguelph.ca
 * **/

#ifndef HEADER_H
#define HEADER_H


Rectangle *createRectangle(xmlNode *curNode);
/** Function to create a rectangle in an SVG image.
 *@pre xmlNode pointer is not null
 *@post xmlNode has not been modified in any way
 *@return a pointer to a struct of type Rectangle
 *@param curNode - a pointer to an xmlNode
**/

Circle *createCircle(xmlNode *curNode);
/** Function to create a circle in an SVG image.
 *@pre xmlNode pointer is not null
 *@post xmlNode has not been modified in any way
 *@return a pointer to a struct of type Circle
 *@param curNode - a pointer to an xmlNode
**/

Path *createPath(xmlNode *curNode);
/** Function to create a circle in an SVG image.
 *@pre xmlNode pointer is not null
 *@post xmlNode has not been modified in any way
 *@return a pointer to a struct of type Path
 *@param curNode - a pointer to an xmlNode
**/

Group *createGroup(xmlNode *curNode);
/** Function to create a group in an SVG image.
 *@pre xmlNode pointer is not null
 *@post xmlNode has not been modified in any way
 *@return a pointer to a struct of type Group
 *@param curNode - a pointer to an xmlNode
**/

//helper functions
bool buildImage(xmlNode * a_node, SVGimage *image);
xmlDocPtr buildTree(SVGimage *image, xmlNsPtr nsPtr);
void searchRectanglesInGroup(Group *group, List *rectList);
void searchGroupsInGroups(Group *parentGroup, List* rectList, char* label);
void freeShapesList(List *shapes);
void searchCirclesInGroup(Group *group, List *circList);
void searchPathsInGroup(Group *group, List *pathList);
bool checkNameSpaceSVG(xmlNode *startNode);
bool checkTitleSVG(xmlNode *curNode);
bool checkDescriptionSVG(xmlNode *curNode);
bool checkCircleSVG(xmlNode *curNode);
bool checkRectangleSVG(xmlNode *curNode);
bool checkPathSVG(xmlNode *curNode);
bool checkGroupSVG(xmlNode *curNode);
bool checkParentGroupSVG(xmlNode *curNode);
bool checkSVGGroupSVG(xmlNode *curNode);
bool checkObjectInGroupSVG(xmlNode *curNode);
void initSVGimage(SVGimage *image);
void initCirc(Circle * circ);
void initRect(Rectangle * rect);
void initPath(Path *path);
void initGroup(Group *group);
bool checkUnits(char *pend, elementType type, Circle *circ, Rectangle *rect);
bool handleCircleAttributes(Circle *circ, char *attrName, char *cont, float test);
bool handleRectangleAttributes(Rectangle *rect, char *attrName, char *cont, float test);
bool handlePathAttributes(Path *path, char *attrName, char *cont, float test);
void handleOtherAttributes(Attribute **attributes, char *attrName, char *cont, void *shape, elementType type, int i);
void deleteSVGimage(SVGimage *img);
void deleteCircle(void *data);
void deleteRectangle(void *data);
void deletePath(void *data);
void deleteGroup(void *data);
void deleteAttribute(void *data);
bool handleObjectInGroup(SVGimage *image, xmlNode *curNode, elementType type);
void handleAttributeSVG(xmlNode *curNode, SVGimage *image);
bool buildSVGimage(xmlNode *startNode, SVGimage *image);
SVGimage *createValidSVGimage(char *fileName, char *schemaFile);
void printer(xmlNodePtr root_node);
void addListsToTree(xmlNodePtr cur_node, SVGimage *image);
void addRectangleChild(xmlNodePtr cur_node, List *rectangles);
void addCircleChild(xmlNodePtr cur_node, List* circles);
void addPathChild(xmlNodePtr cur_node, List* paths);
void addGroupChild(xmlNodePtr cur_node, List *groups);
bool checkAgainstSchemaFile(char *schemaFile, xmlDocPtr doc);
bool modifyOtherAttributes(List *lptr, Attribute *attributeToAdd);
bool changeAttributeFloatValue(float *value, Attribute *newAtt, char *label);
bool changeAttributeString(char *string, Attribute *newAtt);
char *listToString(List *lptr, elementType type);
bool validateOtherAttributes(List *lptr);
void cleanXML(xmlDocPtr doc);
#endif