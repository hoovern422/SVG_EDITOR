#include "SVGParser.h"
#include "LinkedListAPI.h"
#include "header.h" 

#define SHELLSCRIPT "\
#/bin/bash \n\
{ echo \"\"; } 2> /dev/null > testFileNames.txt\n\
for file in include/A2-compliantTestFiles/* \n\
do \n\
{ echo \"$(basename \"$file\")\"; } 2> /dev/null >> testFileNames.txt\n\
done \n\
"

int main(int argc, char **argv){
    
    // Get all test file names and store them in a txt file.
    system(SHELLSCRIPT);

    // Read file names in the parser.
    FILE *fptr = fopen("testFileNames.txt", "r");
    fseek(fptr, 0, SEEK_SET);

    if (!fptr){
        printf("Error opening file.\n");
        return -1;
    }

    char **fileNames = calloc(1, sizeof(char *));
    char line [256] = "";
    char path [32] = "include/A2-compliantTestFiles/";
    char schemaFile [19] = "./include/svg.xsd";
    int i = 0, j = 0, k = 0, l = 0, start = 0, stop = 0;

    while (fgets(line, sizeof(line), fptr)){

        //fgets doesn't cut off the '\n', so we need to manually do it.
        line[strlen(line) - 1] = '\0';

        if (strcmp(line, "\0") != 0){
            fileNames[i] = calloc(strlen(line) + strlen(path) + 1, sizeof(char));
            strcpy(fileNames[i], path);
            strcat(fileNames[i], line);
            fileNames = realloc(fileNames, (i+2) * sizeof(char *));
            i ++;
        }
    }

    fclose(fptr);

    SVGimage *image = NULL;
    char *name, *val;
    Attribute *attribute;
    char saveName [200];
    bool check = false;
    ListIterator itr;
    ListIterator itr2;
    Group *group = NULL;

    printf("\n");

    for (j = 0; j < i; j ++){

        printf("*****************%s*****************\n", fileNames[j]);
        image = createValidSVGimage(fileNames[j], schemaFile);

        if (!image){
            printf("Error reading and creating SVG file: %s\n", fileNames[j]);
        }

        check = validateSVGimage(image, schemaFile);

        if (check){

            printf("namespace: %s\n", image->namespace);
            printf("Title: %s\n", image->title);
            printf("Description: %s\n\n", image->description);
            printf("Num rects: %d\n", getLength(image->allRectangles));
            printf("Num circs: %d\n", getLength(image->allCircles));
            printf("Num paths: %d\n", getLength(image->allPaths));
            printf("Num groups: %d\n", getLength(image->allGroups));
            printf("length of image->groups = %d\n", getLength(image->groups));
            printf("Num attributes: %d\n\n", numAttr(image));

        }

        else{
            printf("Error validating SVG file: %s\n", fileNames[j]);
        }

        k = 0;
        l = 0;
        strcpy(saveName, "\0");
        while (fileNames[j][k] != '/'){
            k ++;
        }
        k ++;
        while (fileNames[j][k] != '/'){
            k ++;
        }
        k ++;
        start = k;
        while (fileNames[j][k] != '.'){
            k ++;
        }
        stop = k;
        while (l < stop - start){
            saveName[l] = fileNames[j][start + l];
            l ++;
        }
        saveName[l] = '_';
        l ++;
        saveName[l] = 't';
        l ++;
        saveName[l] = 'e';
        l ++;
        saveName[l] = 's';
        l ++;
        saveName[l] = 't';
        l ++;
        saveName[l] = '.';
        l ++;
        saveName[l] = 's';
        l ++;
        saveName[l] = 'v';
        l ++;
        saveName[l] = 'g';
        l ++;
        saveName[l] = '\0';

        check = writeSVGimage(image, saveName, schemaFile);   

        if (!check){
            printf("Error writting SVG file: %s\n", fileNames[j]);
        }

        free(fileNames[j]);
        deleteSVGimage(image);

    }

    free(fileNames);

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkNameSpaceSVG(xmlNode *startNode){

    bool check = true;

    if (!startNode) return check;

    if (!startNode->ns){ 
        check = false;
    }
    else if (!startNode->ns->href) {
        check = false;
    }
    else if (strcmp((char *)startNode->ns->href, "\0") == 0){
        check = false;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkTitleSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;

    if (strcmp((char *)curNode->name, "title") == 0){

        if (curNode->children){

            if (curNode->children->content){
                    
                    check = true;
                }

            }
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkDescriptionSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;

    if (strcmp((char *)curNode->name, "desc") == 0){

            if (curNode->children)
            {
                if (curNode->children->content){
                    check = true;
                }
            }

    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkCircleSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;

    if (strcmp((char *)curNode->name, "circle") == 0){
        check = true;
    }

    return check;

}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkRectangleSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;

    if (strcmp((char *)curNode->name, "rect") == 0){
        check = true;
    }

    return check;
    
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkPathSVG(xmlNode *curNode){
    bool check = false;

    if (!curNode) return check;

    if (strcmp((char *)curNode->name, "path") == 0){
        check = true;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkGroupSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;

    if (strcmp((char *)curNode->name, "g") == 0){
        check = true;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkParentGroupSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;
    if (!curNode->parent) return check;

    if (strcmp((char *)curNode->name, "g") == 0 && 
        strcmp((char *)curNode->parent->name, "g") != 0){

            check = true;

    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkSVGGroupSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;

    if (strcmp((char *)curNode->name, "svg") == 0){

        check = true;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkObjectInGroupSVG(xmlNode *curNode){

    bool check = false;

    if (!curNode) return check;
    if (!curNode->parent) return check;
    if (!curNode->parent->name) return check;
    if (strcmp((char *)curNode->parent->name, "\0") == 0) return check;

    if (strcmp((char *)curNode->parent->name, "g") == 0) check = true;

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

void initSVGimage(SVGimage *image){

    image->rectangles = initializeList(&rectangleToString, &deleteRectangle,
                                       &compareRectangles);

    image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles); 
    image->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    image->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);

    image->otherAttributes = initializeList(&attributeToString, &deleteAttribute,
                                            &compareAttributes);
    ///                                        
    image->allRectangles = initializeList(&rectangleToString, &deleteRectangle,
                                       &compareRectangles);

    image->allCircles = initializeList(&circleToString, &deleteCircle, &compareCircles); 
    image->allPaths = initializeList(&pathToString, &deletePath, &comparePaths);
    image->allGroups = initializeList(&groupToString, &deleteGroup, &compareGroups);
                                
}

/////////////////////////////////////////////////////////////////////////////////////////

void initCirc(Circle * circ){

    circ->cx = 0.0;
    circ->cy = 0.0;
    circ->r = 0.0;
    circ->otherAttributes = initializeList(&attributeToString, &deleteAttribute,
                                            &compareAttributes);
    return;                                           
}

/////////////////////////////////////////////////////////////////////////////////////////

void initRect(Rectangle * rect){

    rect->x = 0.0;
    rect->y = 0.0;
    rect->height = 0.0;
    rect->width = 0.0;
    rect->otherAttributes = initializeList(&attributeToString, &deleteAttribute,
                                            &compareAttributes);
    return;                                        

}

/////////////////////////////////////////////////////////////////////////////////////////

void initPath(Path *path){
    
    path->otherAttributes = initializeList(&attributeToString, &deleteAttribute,
                                            &compareAttributes);
    path->data = NULL;

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void initGroup(Group *group){
    
    group->otherAttributes = initializeList(&attributeToString, &deleteAttribute,
                                            &compareAttributes);
    group->rectangles = initializeList(&rectangleToString, &deleteRectangle,
                                        &compareRectangles);
    group->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    group->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    group->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkUnits(char *pend, elementType type, Circle *circ, Rectangle *rect){

    bool check = false;

    if (strcmp(pend, "\0") != 0 && pend ){

        if (strcmp(pend, "cm") == 0 || strcmp(pend, "em") == 0 ||
            strcmp(pend, "px") == 0 || strcmp(pend, "pt") == 0 || 
            strcmp(pend, "pc") == 0 || strcmp(pend, "ex") == 0 ||
            strcmp(pend, "mm") == 0 || strcmp(pend, "in") == 0){

            if (type == 1) strncpy(circ->units, pend, 50);
            else if (type == 2) strncpy(rect->units, pend, 50);
            check = true;
        }
    }

    return check;

}
/////////////////////////////////////////////////////////////////////////////////////////

bool handleCircleAttributes(Circle *circ, char *attrName, char *cont, float test){

    bool check = false;

    if (strcmp(attrName, "cx") == 0){
        sscanf(cont, "%f", &circ->cx);
        check = true;
    }
    else if (strcmp(attrName, "cy") == 0){
        sscanf(cont, "%f", &circ->cy);
        check = true;
    }
    else if (strcmp(attrName, "r") == 0){

        sscanf(cont, "%f", &test);

        if (test >= 0){
            sscanf(cont, "%f", &circ->r);
            check = true;
        }

        else{
            check = false;
            freeList(circ->otherAttributes);
            free(circ);
            circ = NULL;
        }
    }

    return check;

}

/////////////////////////////////////////////////////////////////////////////////////////

bool handleRectangleAttributes(Rectangle *rect, char *attrName, char *cont, float test){

    bool check = false;

    if (strcmp(attrName, "x") == 0){
        
        check = true;
        sscanf(cont, "%f", &rect->x);
    }
    else if (strcmp(attrName, "y") == 0){

        check = true;
        sscanf(cont, "%f", &rect->y);
    }
    else if (strcmp(attrName, "width") == 0){

        sscanf(cont, "%f", &test);
        if (test >= 0){

            check = true;
            sscanf(cont, "%f", &rect->width);
        }
        else{
            check = false;
            free(rect);
            freeList(rect->otherAttributes);
            rect = NULL;
            return check;
        }
    }
    else if (strcmp(attrName, "height") == 0)
    {
        sscanf(cont, "%f", &test);
        if (test >= 0)
        {
            check = true;
            sscanf(cont, "%f", &rect->height);
        }
        else
        {
            check = false;
            free(rect);
            freeList(rect->otherAttributes);
            rect = NULL;
            return check;
        }
    }

    return check;

}

/////////////////////////////////////////////////////////////////////////////////////////

bool handlePathAttributes(Path *path, char *attrName, char *cont,
                          float test){

    bool check = false;

    if (strcmp(attrName, "d") == 0)
    {
        if (!attrName || !cont)
        {
            check = false;
            free(path->otherAttributes);
            free(path);
            path = NULL;
            return check;

        }

        check = true;
        path->data = calloc((strlen(cont) + 1), sizeof(char));
        strcpy(path->data, cont);
    }

    return check;

}

/////////////////////////////////////////////////////////////////////////////////////////

void handleOtherAttributes(Attribute **attributes, char *attrName, char *cont, 
                           void *shape, elementType type, int i){                      

    if (attrName){
        attributes[i]->name = calloc(strlen(attrName) + 1, sizeof(char));
        strcpy(attributes[i]->name, attrName);
    }
    else{
        attributes[i]->name = calloc(1, sizeof(char));
        strcpy(attributes[i]->name, "\0");
    }

    if (cont){
        attributes[i]->value = calloc(strlen(cont) + 1, sizeof(char));
        strcpy(attributes[i]->value, cont);
    }
    else{
        attributes[i]->value = calloc(1, sizeof(char));
        strcpy(attributes[i]->value, "\0");
    }

    if (attributes[i]){

        if (type == 1) insertBack(((Circle *)shape)->otherAttributes, attributes[i]);
        if (type == 2) insertBack(((Rectangle *)shape)->otherAttributes, attributes[i]);
        if (type == 3) insertBack(((Path *)shape)->otherAttributes, attributes[i]);
        if (type == 4) insertBack(((Group *)shape)->otherAttributes, attributes[i]);
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

Circle * createCircle(xmlNode *curNode){

    Circle *circ = NULL;
    xmlAttr *attr = NULL;
    xmlNode *value = NULL;
    char *attrName = NULL;
    char *cont = NULL;
    char *pend = NULL;
    bool isValid = true;
    int numOtherAttributes = 0;
    float test = 0.0;
    bool hasUnits = false;
    Attribute **attributes = calloc(1, sizeof(Attribute *));

    circ = calloc(1, sizeof(Circle));
    initCirc(circ);

    for (attr = curNode->properties; attr ; attr = attr->next){

        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);
        test = strtof(cont, &pend);

        if (!hasUnits) hasUnits = checkUnits(pend, 1, circ, NULL);

        isValid = handleCircleAttributes(circ, attrName, cont, test);

        if (!isValid){

            if (!circ){
                free (attributes);
                return NULL;
            }

            if (numOtherAttributes != 0){
                attributes = 
                realloc(attributes, (numOtherAttributes + 1) * sizeof(Attribute *));
            }

            attributes[numOtherAttributes] = calloc(1, sizeof(Attribute));  

            handleOtherAttributes(attributes, attrName, cont, circ, 1,
                                  numOtherAttributes);                      
            numOtherAttributes ++;
        }
        
    }

    free(attributes);
    return circ;
}

/////////////////////////////////////////////////////////////////////////////////////////

Rectangle * createRectangle(xmlNode *curNode){

    Rectangle *rect = NULL;
    xmlAttr *attr = NULL;
    xmlNode *value = NULL;
    char *attrName = NULL;
    char *cont = NULL;
    char *pend = NULL;
    bool isValid = true;
    int numOtherAttributes = 0;
    float test = 0.0;
    bool hasUnits = false;
    Attribute **attributes = calloc(1, sizeof(Attribute *));

    rect = calloc(1, sizeof(Rectangle));
    initRect(rect);

    for (attr = curNode->properties; attr ; attr = attr->next){

        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);
        test = strtof(cont, &pend);

        if (!hasUnits) hasUnits = checkUnits(pend, 2, NULL, rect);

        isValid = handleRectangleAttributes(rect, attrName, cont, test);

        if (!isValid){

            if (!rect){
                free (attributes);
                return NULL;
            }

            if (numOtherAttributes != 0){
                attributes = 
                realloc(attributes, (numOtherAttributes + 1) * sizeof(Attribute *));
            }

            attributes[numOtherAttributes] = calloc(1, sizeof(Attribute)); 

            handleOtherAttributes(attributes, attrName, cont, rect, 2, 
                                  numOtherAttributes);
            numOtherAttributes ++;
        }
        
    }

    free (attributes);
    return rect;
}

/////////////////////////////////////////////////////////////////////////////////////////

Path * createPath(xmlNode *curNode){

    Path *path = NULL;
    xmlAttr *attr = NULL;
    xmlNode *value = NULL;
    char *attrName = NULL;
    char *cont = NULL;
    bool isValid = true;
    int numOtherAttributes = 0;
    float test = 0.0;
    Attribute **attributes = calloc(1, sizeof(Attribute *));

    path = calloc(1, sizeof(Path));
    initPath(path);

    for (attr = curNode->properties; attr  ; attr = attr->next){

        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);

        isValid = handlePathAttributes(path, attrName, cont, test);

        if (!isValid){

            if (!path){
                free (attributes);
                return NULL;
            }

            if (numOtherAttributes != 0){
                attributes = 
                realloc(attributes, (numOtherAttributes + 1) * sizeof(Attribute *));
            }

            attributes[numOtherAttributes] = calloc(1, sizeof(Attribute)); 

            handleOtherAttributes(attributes, attrName, cont, path, 3,
                                  numOtherAttributes);
            numOtherAttributes ++;
        }
        
    }

    free(attributes);

    if (!path->data){
        deletePath(path);
        return NULL;
    }

    return path;
}

/////////////////////////////////////////////////////////////////////////////////////////

Group * createGroup(xmlNode *curNode){

    Group *group = NULL;
    xmlAttr *attr = NULL;
    xmlNode *value = NULL;
    char *attrName = NULL;
    char *cont = NULL;
    int numOtherAttributes = 0;
    Attribute **attributes = calloc(1, sizeof(Attribute *));

    group = calloc(1, sizeof(Group));
    initGroup(group);

    for (attr = curNode->properties; attr ; attr = attr->next){

        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);

        if (numOtherAttributes != 0){
            attributes = 
            realloc(attributes, (numOtherAttributes + 1) * sizeof(Attribute *));
        }

        attributes[numOtherAttributes] = calloc(1, sizeof(Attribute)); 

        handleOtherAttributes(attributes, attrName, cont, group, 4,
                              numOtherAttributes);
        numOtherAttributes ++;
        
    }

    free(attributes);
    return group;
}

/////////////////////////////////////////////////////////////////////////////////////////

void deleteSVGimage(SVGimage *image)
{
    if (image == NULL)
        return;
    freeShapesList(image->allRectangles);
    freeShapesList(image->allCircles);
    freeShapesList(image->allPaths);
    freeShapesList(image->allGroups);
    freeList(image->otherAttributes);
    freeList(image->rectangles);
    freeList(image->circles);
    freeList(image->paths);
    freeList(image->groups);
    free(image);
    image = NULL;
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void deleteCircle(void *data)
{
    if (data == NULL)
        return;
    Circle *circle = (Circle *)data;
    freeList(circle->otherAttributes);
    free(circle);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void deleteRectangle(void *data)
{
    if (data == NULL)
        return;
    Rectangle *rectangle = (Rectangle *)data;
    freeList(rectangle->otherAttributes);
    free(rectangle);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void deletePath(void *data)
{
    if (data == NULL)
        return;
    Path *path = (Path *)data;
    freeList(path->otherAttributes);
    free(path->data);
    free(path);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void deleteGroup(void *data)
{
    if (data == NULL)
        return;
    Group *group = (Group *)data;
    freeList(group->otherAttributes);
    freeList(group->rectangles);
    freeList(group->circles);
    freeList(group->paths);
    freeList(group->groups);
    free(group);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void deleteAttribute(void *data)
{
    if (data == NULL)
        return;
    Attribute *att = (Attribute *)data;
    free(att->name);
    free(att->value);
    free(att);
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool handleObjectInGroup(SVGimage *image, xmlNode *curNode, elementType type){

    if (!image || !curNode) return false;

    bool check = true;
    Circle *circ = NULL;
    Rectangle *rect = NULL;
    Path *path = NULL;
    Group *group = NULL;
    List *lptr = NULL;
    ListIterator groupListItr, prevGroupListItr;

    // Iterate through all group lists to arrive to the desired group.
    groupListItr = createIterator(image->groups);

    do {

        while (groupListItr.current->next){
            groupListItr.current = groupListItr.current->next;
        }

        prevGroupListItr = groupListItr;
        groupListItr = createIterator(((Group *) groupListItr.current->data)->groups);

    } while(groupListItr.current);

    groupListItr = prevGroupListItr;

    // Create and add the object to the back of the desired list.
    if (type == 1) {

        circ = createCircle(curNode);

        if (!circ){

            check = false;
            return check;
        }

        lptr = (List *)(((Group *)groupListItr.current->data)->circles);
        insertBack(lptr, circ);
    }
    else if (type == 2) {

        rect = createRectangle(curNode);

        if (!rect){
            
            check = false;
            return check;
        }

        lptr = (List *)(((Group *)groupListItr.current->data)->rectangles);
        insertBack(lptr, rect);
    }
    else if (type == 3) {

        path = createPath(curNode);

        if (!path){
            
            check = false;
            return check;
        }

        lptr = (List *)(((Group *)groupListItr.current->data)->paths);
        insertBack(lptr, path);
    }
    else if (type == 4) {

        group = createGroup(curNode);

        if (!group){
            
            check = false;
            return check;
        }

        lptr = (List *)(((Group *)groupListItr.current->data)->groups);
        insertBack(lptr, group);
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

void handleAttributeSVG(xmlNode *curNode, SVGimage *image){

    xmlAttr *attr = NULL;
    xmlNode *value = NULL; 
    Attribute *SVGAttr = NULL;
    char *attrName = NULL;
    char *cont = NULL;

    for (attr = curNode->properties; attr  ; attr = attr->next){

        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);

        SVGAttr = calloc(1, sizeof(Attribute));

        if (attrName){
            SVGAttr->name = calloc(strlen(attrName) + 1, sizeof(char));
            strcpy(SVGAttr->name, attrName);
        }
        else{
            SVGAttr->name = calloc(1, sizeof(char));
            strcpy(SVGAttr->name, "\0");
        }

        if (cont){
            SVGAttr->value = calloc(strlen(cont) + 1, sizeof(char));
            strcpy(SVGAttr->value, cont);
        }
        else{
            SVGAttr->value = calloc(1, sizeof(char));
            strcpy(SVGAttr->value, "\0");
        }

        if (attr) insertBack(image->otherAttributes, SVGAttr);

        SVGAttr = NULL;
}

    return;

}

/////////////////////////////////////////////////////////////////////////////////////////

bool buildSVGimage(xmlNode *startNode, SVGimage *image){

    // parameter checking.
    if (!startNode || !image){
        return false;
    } 

    bool check = true;
    xmlNode *curNode = NULL;
    Rectangle *rect = NULL;
    Circle *circ = NULL;
    Path *path = NULL;
    Group *group = NULL;

    for (curNode = startNode; curNode!= NULL; curNode = curNode->next){

        // Check that the current node is a valid xml element node.
        if (curNode->type == XML_ELEMENT_NODE){

            // Check/allocate the namespace.
            if (curNode == startNode){

                if (!checkNameSpaceSVG(startNode)){
                    check = false; //invalid SVG image
                    break;
                }

                strncpy(image->namespace, (char*)curNode->ns->href, 255);
            }

            //check for/allocate a non-svg group/object within another group to the 
            //image.
            if (checkObjectInGroupSVG(curNode)){

                if (checkCircleSVG(curNode)){ 
                    check = handleObjectInGroup(image, curNode, 1);
                }
                else if (checkRectangleSVG(curNode)) {
                    check = handleObjectInGroup(image, curNode, 2);
                }
                else if (checkPathSVG(curNode)){
                    check = handleObjectInGroup(image, curNode, 3);
                }
                else if (checkGroupSVG(curNode)){
                    check = handleObjectInGroup(image, curNode, 4);
                }

            }

            // Check for/allocate a title to the svg layer.
            else if (checkTitleSVG(curNode)){
                strncpy(image->title, (char *) curNode->children->content, 255);
            }

            // Check for/allocate a description to the svg layer.
            else if (checkDescriptionSVG(curNode)){
                strncpy(image->description, (char *)curNode->children->content, 255);
            }

            //Check for/allocate a circle to the svg layer.
            else if (checkCircleSVG(curNode)){
                circ = createCircle(curNode);
                insertBack(image->circles, circ);
            }

            //Check for/allocate a rectangle to the svg layer.
            else if (checkRectangleSVG(curNode)){
                rect = createRectangle(curNode);
                insertBack(image->rectangles, rect);
            }

            //check for/allocate a path to the svg layer.
            else if (checkPathSVG(curNode)){
                path = createPath(curNode);
                insertBack(image->paths, path);
            }

            //check for/allocate a parent group to the svg layer.
            else if (checkParentGroupSVG(curNode)){
                group = createGroup(curNode);
                insertBack(image->groups, group);
            }

            //check for/allocate any other attributes to the svg layer.
            else{
                handleAttributeSVG(curNode, image);
            }

            // recursively call the function again.
            buildSVGimage(curNode->children, image);
        }
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool checkAgainstSchemaFile(char *schemaFile, xmlDocPtr doc){
    
    bool valid = false;

    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    ctxt = xmlSchemaNewParserCtxt(schemaFile);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    xmlSchemaValidCtxtPtr validCtxt;

    validCtxt = xmlSchemaNewValidCtxt(schema);
    if (xmlSchemaValidateDoc(validCtxt, doc) == 0) valid = true;

    xmlSchemaFreeValidCtxt(validCtxt);
    if (schema) xmlSchemaFree(schema);

    return valid;
}

/////////////////////////////////////////////////////////////////////////////////////////

void cleanXML(xmlDocPtr doc){

    xmlFreeDoc(doc);
    xmlSchemaCleanupTypes();
    xmlCleanupParser(); 
    xmlMemoryDump();
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void getAllCircles(SVGimage *image){

    List *circList = image->allCircles;
    if (!image) return;

    ListIterator circListItr;
    ListIterator groupItr;

    elementType label = CIRC;

    circListItr = createIterator(image->circles);
    while (circListItr.current)
    {
        insertBack(circList, (Circle *)circListItr.current->data);
        circListItr.current = circListItr.current->next;
    }

    if (getLength(image->groups) > 0)
    {
        groupItr = createIterator(image->groups);
        while (groupItr.current)
        {
            searchGroupsInGroups(groupItr.current->data, circList, label);
            groupItr.current = groupItr.current->next;
        }
    }

    return;

}

/////////////////////////////////////////////////////////////////////////////////////////

void getAllRects(SVGimage *image){

    List *rectList = image->allRectangles;
    if (!image) return;
    ListIterator rectListItr;
    ListIterator groupItr;
    elementType label = RECT;

    rectListItr = createIterator(image->rectangles);

    while (rectListItr.current)
    {
        insertBack(rectList, (Rectangle *)rectListItr.current->data);
        rectListItr.current = rectListItr.current->next;
    }

    if (getLength(image->groups) > 0)
    {
        groupItr = createIterator(image->groups);
        while (groupItr.current)
        {
            searchGroupsInGroups(groupItr.current->data, rectList, label);
            groupItr.current = groupItr.current->next;
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void getAllPaths(SVGimage *image)
{
    List *pathList = image->allPaths;
    if (!image) return;

    ListIterator imagePathItr;
    ListIterator groupItr;

    elementType label = PATH;

    imagePathItr = createIterator(image->paths);
    while (imagePathItr.current )
    {
        insertBack(pathList, (Path *)imagePathItr.current->data);
        imagePathItr.current = imagePathItr.current->next;
    }

    if (getLength(image->groups) > 0)
    {
        groupItr = createIterator(image->groups);
        while (groupItr.current )
        {
            searchGroupsInGroups(groupItr.current->data, pathList, label);
            groupItr.current = groupItr.current->next;
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void getAllGroups(SVGimage *image)
{
    List *groupList = image->allGroups;

    if (!image) return;

    ListIterator groupItr;

    elementType label = GROUP; 

    if (getLength(image->groups) > 0)
    {
        groupItr = createIterator(image->groups);
        while (groupItr.current)
        {
            searchGroupsInGroups(groupItr.current->data, groupList, label);
            groupItr.current = groupItr.current->next;
        }
    }

    return;

    groupItr = createIterator(image->allGroups);
    
}

/////////////////////////////////////////////////////////////////////////////////////////

void searchGroupsInGroups(Group *parentGroup, List *list, elementType label){

    ListIterator itr;
    List *lptr = NULL;
    Group *group = NULL;

    // insert shape to appropriate list
    if (label == 1) searchCirclesInGroup(parentGroup, list);
    else if (label == 2) searchRectanglesInGroup(parentGroup, list);
    else if (label == 3) searchPathsInGroup(parentGroup, list);
    else if (label == 4) insertBack(list, parentGroup);

    // determine if the parent group has more groups.
    lptr = ((List *)(parentGroup->groups));

    if (getLength(lptr) > 0){

        //if so, iterate through them.
        itr = createIterator(lptr);

        // recursively call the function again for each group.
        while (itr.current){
            group = (Group *) itr.current->data;
            searchGroupsInGroups(group, list, label);
            itr.current = itr.current->next;
        }

    }

    return;

}

/////////////////////////////////////////////////////////////////////////////////////////

void searchCirclesInGroup(Group *group, List *circList)
{
    ListIterator circItr;
    List *lptr;
    if (getLength((List *)(group->circles)) > 0)
    {
        lptr = (List *)(group->circles);
        circItr = createIterator(lptr);
        while (circItr.current)
        {
            insertBack(circList, (Circle *)circItr.current->data);
            circItr.current = circItr.current->next;
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void searchRectanglesInGroup(Group *group, List *rectList)
{
    ListIterator rectItr;
    List *lptr;
    if (getLength((List *)(group->rectangles)) > 0)
    {
        lptr = (List *)(group->rectangles);
        rectItr = createIterator(lptr);
        while (rectItr.current)
        {
            insertBack(rectList, (Rectangle *)rectItr.current->data);
            rectItr.current = rectItr.current->next;
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void searchPathsInGroup(Group *group, List *pathList)
{
    ListIterator pathItr;
    List *lptr;
    if (getLength((List *)(group->paths)) > 0)
    {
        lptr = (List *)(group->paths);
        pathItr = createIterator(lptr);
        while (pathItr.current)
        {
            insertBack(pathList, (Path *)pathItr.current->data);
            pathItr.current = pathItr.current->next;
        }
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void freeShapesList(List *shapes){

    if (!shapes)
        return;
    ListIterator itr1, itr2;
    itr1 = createIterator(shapes);
    itr2 = createIterator(shapes);
    while (itr1.current )
    {
        itr2.current = itr1.current;
        itr1.current = itr1.current->next;
        free(itr2.current);
    }
    free(shapes);
}

/////////////////////////////////////////////////////////////////////////////////////////

bool validateSVGNode(SVGimage *image){
    
    bool check = true;
    List *lptr = NULL;
    
    // validate the SVG node.
    if (!image){
        check = false;
        return check;
    }

    if (!image->rectangles || !image->circles || !image->paths  ||
        !image->groups || !image->otherAttributes || !image->namespace ||
        strcmp(image->namespace, "\0") == 0  || strlen(image->namespace) > 255 ||
        strlen(image->title) > 255 || strlen(image->description) > 255){

        check = false;
        return check;
    }

    //get all other attributes from the paths in the image
    lptr = image->otherAttributes;
    
    if (!validateOtherAttributes(lptr)){
        check = false;
        return check;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool validateCircles(SVGimage *image){

    bool check = true;
    List *lptr = NULL;
    ListIterator circItr;
    Circle *circle = NULL;

    //validate the circles in the image
    circItr = createIterator(image->allCircles);

    while (circItr.current){

        circle = (Circle *)circItr.current->data;

        if (!circle || circle->r < 0 || !circle->otherAttributes){

            check = false;
            return check;
        }

       //get all other attributes from the circles in the image
        lptr = circle->otherAttributes;
        
        if (!validateOtherAttributes(lptr)){
            check = false;
            return check;
        }

        circItr.current = circItr.current->next;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool validateRectangles(SVGimage *image){

    bool check = true;
    List *lptr = NULL;
    ListIterator rectItr;
    Rectangle *rectangle = NULL;

    //validate the rectangles in the image
    rectItr = createIterator(image->allRectangles);

    while (rectItr.current){

        rectangle = (Rectangle *)rectItr.current->data;

        if (!rectangle || rectangle->height < 0 || rectangle->width < 0 || 
            !rectangle->otherAttributes){

            check = false;
            return check;
        }

        //get all other attributes from the rectangles in the image
        lptr = rectangle->otherAttributes;
        
        if (!validateOtherAttributes(lptr)){
            check = false;
            return check;
        }

        rectItr.current = rectItr.current->next;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool validatePaths(SVGimage *image){

    bool check = true;
    List *lptr = NULL;
    ListIterator pathItr;
    Path *path = NULL;

    //validate the paths in the image
    pathItr = createIterator(image->allPaths);

    while (pathItr.current){

        path = (Path *)pathItr.current->data;

        if (!path || !path->data || !path->otherAttributes || 
            strcmp(path->data, "\0") == 0){

            check = false;
            return check;
        }

        //get all other attributes from the paths in the image
        lptr = path->otherAttributes;
        
        if (!validateOtherAttributes(lptr)){
            check = false;
            return check;
        }

        pathItr.current = pathItr.current->next;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool validateGroups(SVGimage *image){

    bool check = true;
    List *lptr = NULL;
    ListIterator groupItr;
    Group * group = NULL;

    //validate the paths in the image
    groupItr = createIterator(image->allGroups);

    while (groupItr.current){

        group = (Group *)groupItr.current->data;

        if (!group || !group->rectangles || !group->circles || !group->paths ||
            !group->groups || !group->otherAttributes){

            check = false;
            return check;
        }

        //get all other attributes from the paths in the image
        lptr = group->otherAttributes;

        if (!validateOtherAttributes(lptr)){
            check = false;
            return check;
        }

        groupItr.current = groupItr.current->next;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool validateOtherAttributes(List *lptr){

    ListIterator attItr = createIterator(lptr);
    Attribute *attribute = NULL;
    bool check = true;

    while(attItr.current){
        attribute = (Attribute *) attItr.current->data;
        if (!attribute || !attribute->name || !attribute->value){

            check = false;
            return check;
        }

        attItr.current = attItr.current->next;
    }

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool validateSVGimage(SVGimage *image, char *schemaFile){

    if (!image) return false;
    if (!schemaFile) return false;

    bool check = true;

    check = validateSVGNode(image);
    if (!check) return check;

    check = validateCircles(image);
    if (!check) return check;

    check = validateRectangles(image);
    if (!check) return check;

    check = validatePaths(image);
    if (!check) return check;

    check = validateGroups(image);
    if (!check) return check;

    xmlDocPtr doc = NULL;
    xmlNsPtr nsPtr = NULL;

    doc = buildTree(image, nsPtr);
    
    if (!doc){
        check = false;
        return check;
    }

    check = checkAgainstSchemaFile(schemaFile, doc);
    cleanXML(doc);
    xmlFreeNs(nsPtr);

    return check;
     
}

/////////////////////////////////////////////////////////////////////////////////////////

xmlDocPtr buildTree(SVGimage *image, xmlNsPtr nsPtr){

    if (!image || !image->namespace || strcmp(image->namespace, "\0") == 0 || 
        !image->title || !image->description){

        return NULL;
    }

    xmlDocPtr doc = NULL;
    xmlNodePtr root_node = NULL;

    doc = xmlNewDoc(BAD_CAST "1.0");//creating the document with version 1.0

    //namespace
    root_node = xmlNewNode(nsPtr, BAD_CAST "svg");//creating the root node
    nsPtr = xmlNewNs(root_node, BAD_CAST image->namespace, NULL);
    xmlSetNs(root_node, nsPtr);

    //setting root node
    xmlDocSetRootElement(doc, root_node);

    //title
    if (strcmp(image->title, "\0") == 0) xmlNewChild(root_node, NULL, BAD_CAST "title",
                                         BAD_CAST "\0");

    else xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST image->title);

    //description
    if (strcmp(image->description, "\0") == 0) xmlNewChild(root_node, NULL,
                                               BAD_CAST "desc", BAD_CAST "\0");

    else xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST image->description);
    
    //adding attributes to svg node in loop
    ListIterator itr = createIterator(image->otherAttributes);
    Attribute *attribute = NULL;

    for (itr.current = itr.current; itr.current ; 
         itr.current = itr.current->next){

        attribute = (Attribute*)itr.current->data;
        xmlNewProp(root_node, BAD_CAST attribute->name, BAD_CAST attribute->value);
    }

    //add all rectangles, circles paths and groups for svg node every group (in order).
    addListsToTree(root_node, image);
    return doc;
}

/////////////////////////////////////////////////////////////////////////////////////////

void addRectangleChild(xmlNodePtr curNode, List *rectangles){

    xmlNodePtr rect = NULL;
    char *buff = calloc (1000, sizeof(char));
    Rectangle *rectPtr = NULL;
    Attribute *attrPtr = NULL;
    ListIterator rectItr = createIterator(rectangles);
    ListIterator attItr;

    /*first loop itterates through list elements*/
    while(rectItr.current){
        rect = xmlNewChild(curNode, NULL, BAD_CAST "rect", BAD_CAST "\0");
        rectPtr = (Rectangle *)rectItr.current->data;

        //x property
        sprintf(buff, "%f", rectPtr->x);
        if (rectPtr->units && strcmp(rectPtr->units, "\0") != 0)
            strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "x", BAD_CAST buff);

        //y property
        sprintf(buff, "%f", rectPtr->y);
        if (rectPtr->units && strcmp(rectPtr->units, "\0") != 0) 
            strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "y", BAD_CAST buff);

        //width property
        sprintf(buff, "%f", rectPtr->width);
        if (rectPtr->units && strcmp(rectPtr->units, "\0") != 0)
            strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "width", BAD_CAST buff);

        //height property
        sprintf(buff, "%f", rectPtr->height);
        if (rectPtr->units && strcmp(rectPtr->units, "\0") != 0) 
            strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "height", BAD_CAST buff);

        //otherAttributes
        attItr = createIterator(rectPtr->otherAttributes);
        while (attItr.current ){
            attrPtr = (Attribute *) attItr.current->data; 
            xmlNewProp(rect, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attItr.current = attItr.current->next;
        }

        rectItr.current = rectItr.current->next;
    }

    free(buff);
}

/////////////////////////////////////////////////////////////////////////////////////////

void addCircleChild(xmlNodePtr curNode, List *circles){

    xmlNodePtr circle = NULL;
    char *buff = calloc (1000, sizeof(char));
    Circle *circlePtr = NULL;
    Attribute *attrPtr = NULL;
    ListIterator circleItr = createIterator(circles);
    ListIterator attItr;

    /*first loop itterates through list elements*/
    while(circleItr.current){
        circle = xmlNewChild(curNode, NULL, BAD_CAST "circle", BAD_CAST "\0");
        circlePtr = (Circle *)circleItr.current->data;

        //cx property
        sprintf(buff, "%f", circlePtr->cx);
        if (circlePtr->units  && strcmp(circlePtr->units, "\0") != 0) 
            strcat(buff, circlePtr->units);
        xmlNewProp(circle, BAD_CAST "cx", BAD_CAST buff);

        //cy property
        sprintf(buff, "%f", circlePtr->cy);
        if (circlePtr->units  && strcmp(circlePtr->units, "\0") != 0) 
            strcat(buff, circlePtr->units);
        xmlNewProp(circle, BAD_CAST "cy", BAD_CAST buff);

        //r property
        sprintf(buff, "%f", circlePtr->r);
        if (circlePtr->units  && strcmp(circlePtr->units, "\0") != 0) 
            strcat(buff, circlePtr->units);
        xmlNewProp(circle, BAD_CAST "r", BAD_CAST buff);

        //otherAttributes
        attItr = createIterator(circlePtr->otherAttributes);
        while (attItr.current){
            attrPtr = (Attribute *) attItr.current->data; 
            xmlNewProp(circle, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attItr.current = attItr.current->next;
        }

        circleItr.current = circleItr.current->next;
    }

    free(buff);
}

/////////////////////////////////////////////////////////////////////////////////////////

void addPathChild(xmlNodePtr curNode, List *paths){

    xmlNodePtr path = NULL;
    Path *pathPtr = NULL;
    Attribute *attrPtr = NULL;
    ListIterator pathItr = createIterator(paths);
    ListIterator attItr;

    /*first loop itterates through list elements*/
    while(pathItr.current){

        path = xmlNewChild(curNode, NULL, BAD_CAST "path", BAD_CAST "\0");
        pathPtr = (Path *)pathItr.current->data;

        //d property
        xmlNewProp(path, BAD_CAST "d", BAD_CAST pathPtr->data);

        //otherAttributes
        attItr = createIterator(pathPtr->otherAttributes);

        while (attItr.current){
            attrPtr = (Attribute *) attItr.current->data; 
            xmlNewProp(path, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attItr.current = attItr.current->next;
        }

        pathItr.current = pathItr.current->next;
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

void addGroupChild(xmlNodePtr curNode, List *groups){

    xmlNodePtr group = NULL;
    Group *groupPtr = NULL;
    Attribute *attrPtr;
    ListIterator groupItr = createIterator(groups);
    ListIterator attItr;

    List* lptr = NULL;
    while(groupItr.current ){
        //add new child
        group = xmlNewChild(curNode, NULL, BAD_CAST "g", BAD_CAST "\0");
        //set group ptr
        groupPtr = (Group*) groupItr.current->data;
        //add rectangles
        lptr = groupPtr->rectangles;
        addRectangleChild(group, lptr);
        //add circles
        lptr = groupPtr->circles;
        addCircleChild(group, lptr);
        //add paths
        lptr = groupPtr->paths;
        addPathChild(group, lptr);
        //add other Attributes
        attItr = createIterator(groupPtr->otherAttributes);
        while(attItr.current ){
            attrPtr = (Attribute *) attItr.current->data; 
            xmlNewProp(group, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attItr.current = attItr.current->next;
        }
        //recursively call addGroupsChild
        lptr = groupPtr->groups;
        addGroupChild(group, lptr);
        //update list pointer
        groupItr.current = groupItr.current->next;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////

void addListsToTree(xmlNodePtr curNode, SVGimage *image){

    List *lptr = NULL;

    //add rectangles to svg
    lptr = image->rectangles;
    addRectangleChild(curNode, lptr);

    //add circles to svg
    lptr = image->circles;
    addCircleChild(curNode, lptr);

    //add paths to svg
    lptr = image->paths;
    addPathChild(curNode, lptr);

    // enter the addGroupChild function with svg groups list, which will do recursive
    // calls to build the rest of the tree.
    lptr = image->groups;
    addGroupChild(curNode, lptr);
}

/////////////////////////////////////////////////////////////////////////////////////////

SVGimage *createValidSVGimage(char *fileName, char *schemaFile){

    xmlDocPtr doc = NULL;
    xmlNode *rootElement = NULL;
    SVGimage *image;
    bool valid = true;

    // validate parameters.
    if (!fileName || strcmp(fileName, "\0") == 0 || !schemaFile || 
        strcmp(schemaFile, "\0") == 0){

        xmlCleanupParser();
        return NULL;
    }

    // check doc pointer.
    doc = xmlReadFile(fileName, NULL, 0);
    if (!doc){

        cleanXML(doc);
        return NULL;
    }

    // check the file against the schema file.
    if (!checkAgainstSchemaFile(schemaFile, doc)){

        cleanXML(doc);
        return NULL;
    }

    // Allocate an SVGimage.
    image = calloc(1, sizeof(SVGimage));
    initSVGimage(image);

    // build/validate an SVG image.
    rootElement = xmlDocGetRootElement(doc);
    valid = buildSVGimage(rootElement, image);
    getAllRects(image);
    getAllCircles(image);
    getAllPaths(image);
    getAllGroups(image);
    if (valid) valid = validateSVGimage(image, schemaFile);

    if (!valid){
        deleteSVGimage(image);
        cleanXML(doc);
        return NULL;
    }

    cleanXML(doc);
    return image;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool writeSVGimage(SVGimage *image, char *fileName, char *schemaFile){ 

    if (!schemaFile) return false;
    if (!validateSVGimage(image, schemaFile)) return false;

    xmlNsPtr nsPtr = NULL;
    xmlDocPtr doc = NULL;

    doc = buildTree(image, nsPtr);

    if (!doc){
        xmlFreeDoc(doc);
        xmlFreeNs(nsPtr);
        return false;
    }

    xmlSaveFile(fileName, doc);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlFreeNs(nsPtr);

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

int numRectsWithArea(SVGimage *image, float area)
{
    if (!image) return 0;
    if (area < 0) return 0;

    List *rectangles = image->allRectangles;
    Rectangle *rectangle;
    ListIterator itr = createIterator(rectangles);
    int intArea = ceil(area);
    int calcArea = 0;
    int num = 0;

    while (itr.current)
    {
        rectangle = (Rectangle *)itr.current->data;
        if (rectangle->height != 0 && rectangle->width != 0)
        {
            calcArea = ceil(rectangle->height * rectangle->width);
            if (calcArea == intArea)
                num++;
        }
        itr.current = itr.current->next;
    }

    return num;
}

/////////////////////////////////////////////////////////////////////////////////////////

int numCirclesWithArea(SVGimage *image, float area)
{
    if (!image) return 0;
    if (area < 0) return 0;

    List *circles = image->allCircles;
    Circle *circle;
    ListIterator itr = createIterator(circles);
    int intArea = ceil(area);
    int calcArea = 0;
    int num = 0;

    while (itr.current){

        circle = (Circle *)itr.current->data;
        if (circle->r > 0)
        {
            calcArea = ceil(3.14159265358979 * (circle->r) * circle->r);
            if (calcArea == intArea)
                num++;
        }
        itr.current = itr.current->next;
    }

    return num;
}

/////////////////////////////////////////////////////////////////////////////////////////

int numPathsWithdata(SVGimage *image, char *data)
{
    if (image == NULL) return 0;
    if (data == NULL) return 0;

    List *paths = image->allPaths;
    Path *path;
    ListIterator itr = createIterator(paths);
    int num = 0;

    while (itr.current != NULL){

        path = (Path *)itr.current->data;
        if (strcmp(data, path->data) == 0)
        {
            num++;
        }
        itr.current = itr.current->next;
    }

    return num;
}

/////////////////////////////////////////////////////////////////////////////////////////

int numGroupsWithLen(SVGimage *image, int len)
{
    if (!image) return 0;
    if (len < 0) return 0;

    List *groups = image->allGroups;
    Group *group;
    ListIterator itr = createIterator(groups);
    int num = 0;
    int calcLength = 0;

    while (itr.current != NULL){

        group = (Group *)itr.current->data;
        calcLength = getLength(group->rectangles) + getLength(group->circles) + 
                     getLength(group->paths) + getLength(group->groups);
        if (calcLength == len)
            num++;
        itr.current = itr.current->next;
    }

    return num;
}

/////////////////////////////////////////////////////////////////////////////////////////

int numAttr(SVGimage *image)
{
    if (image == NULL) return 0;

    Group *group;
    Rectangle *rectangle;
    Circle *circle;
    Path *path;

    List *groups = image->allGroups;
    List *rectangles = image->allRectangles;
    List *circles = image->allCircles;
    List *paths = image->allPaths;

    ListIterator itr1 = createIterator(groups);
    ListIterator itr2 = createIterator(rectangles);
    ListIterator itr3 = createIterator(circles); 
    ListIterator itr4 = createIterator(paths);

    int num = 0;

    while (itr1.current != NULL)
    {
        group = (Group *)itr1.current->data;
        num += getLength(group->otherAttributes);
        itr1.current = itr1.current->next;
    }

    while (itr2.current != NULL)
    {
        rectangle = (Rectangle *)itr2.current->data;
        num += getLength(rectangle->otherAttributes);
        itr2.current = itr2.current->next;
    }

    while (itr3.current != NULL)
    {
        circle = (Circle *)itr3.current->data;
        num += getLength(circle->otherAttributes);
        itr3.current = itr3.current->next;
    }

    while (itr4.current != NULL)
    {
        path = (Path *)itr4.current->data;
        num += getLength(path->otherAttributes);
        itr4.current = itr4.current->next;
    }

    num += getLength(image->otherAttributes);

    return num;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* attrToJSON(const Attribute *a){
    
    if (!a){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "{}");
        return ret;
    }

    char *attrName = calloc(strlen(a->name) + 1, sizeof(char));
    strcpy(attrName, (char*)a->name);
    char *attrValue = calloc(strlen(a->value) + 1, sizeof(char));
    strcpy(attrValue, (char*)a->value);
    char *json = calloc(strlen(a->name) + strlen(a->value) + 100, sizeof(char));

    strcpy(json, "{\"name\":\"");
    strcat(json, attrName);
    strcat(json, "\",\"value\":\"");
    strcat(json, attrValue);
    strcat(json, "\"}");

    free(attrName);
    free(attrValue);

    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* circleToJSON(const Circle *c){

    if (!c){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "{}");
        return ret;
    }
    float cx = c->cx, cy = c->cy, r = c->r;
    int numAtt = getLength(c->otherAttributes);
    char *units = calloc(strlen(c->units) + 1, sizeof(char));
    char *buffer = calloc(100, sizeof(char));
    strcpy(units, c->units);
    char *json = calloc(strlen(units) + 250, sizeof(char));

    strcpy(json, "{\"cx\":");
    sprintf(buffer, "%.2f", cx);
    strcat(json, buffer);
    strcat(json, ",\"cy\":");
    sprintf(buffer, "%.2f", cy);
    strcat(json, buffer);
    strcat(json, ",\"r\":");
    sprintf(buffer, "%.2f", r);
    strcat(json, buffer);
    strcat(json, ",\"numAttr\":");
    sprintf(buffer, "%d", numAtt);
    strcat(json, buffer);
    strcat(json, ",\"units\":\"");
    strcat(json, units);
    strcat(json, "\"}");

    free(buffer);
    free(units);

    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* rectToJSON(const Rectangle *r){

    if (!r){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "{}");
        return ret;
    }

    float x = r->x, y = r->y, width = r->width, height = r->height;
    int numAtt = getLength(r->otherAttributes);
    char *units = calloc(strlen(r->units) + 1, sizeof(char));
    char *buffer = calloc(100, sizeof(char));
    strcpy(units, r->units);
    char *json = calloc(strlen(units) + 250, sizeof(char));

    strcpy(json, "{\"x\":");
    sprintf(buffer, "%.2f", x);
    strcat(json, buffer);
    strcat(json, ",\"y\":");
    sprintf(buffer, "%.2f", y);
    strcat(json, buffer);
    strcat(json, ",\"w\":");
    sprintf(buffer, "%.2f", width);
    strcat(json, buffer);
    strcat(json, ",\"h\":");
    sprintf(buffer, "%.2f", height);
    strcat(json, buffer);
    strcat(json, ",\"numAttr\":");
    sprintf(buffer, "%d", numAtt);
    strcat(json, buffer);
    strcat(json, ",\"units\":\"");
    strcat(json, units);
    strcat(json, "\"}");

    free(buffer);
    free(units);

    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* pathToJSON(const Path *p){

    if (!p){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "{}");
        return ret;
    }

    int numAtt = getLength(p->otherAttributes);
    char *data = calloc(65, sizeof(char));
    strncpy(data, p->data, 64);
    char *json = calloc(strlen(data) + 250, sizeof(char));
    char *buffer = calloc(100, sizeof(char));
    strcpy(json, "{\"d\":\"");
    strcat(json, data);
    strcat(json, "\",\"numAttr\":");
    sprintf(buffer, "%d", numAtt);
    strcat(json, buffer);
    strcat(json, "}");
    free(buffer);
    free(data);
    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* groupToJSON(const Group *g){

    if (!g || !g->rectangles || !g->circles || !g->paths || !g->groups ||
        !g->otherAttributes){

        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "{}");
        return ret;
    }

    int numAtt = getLength(g->otherAttributes);
    int cVal = getLength(g->rectangles) + getLength(g->circles) + getLength(g->paths) + getLength(g->groups);
    char *json = calloc(250, sizeof(char));
    char *buffer = calloc(100, sizeof(char));

    strcpy(json, "{\"children\":");
    sprintf(buffer, "%d", cVal);
    strcat(json, buffer);
    strcat(json, ",\"numAttr\":");
    sprintf(buffer, "%d", numAtt);
    strcat(json, buffer);
    strcat(json, "}");

    free(buffer);

    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* SVGtoJSON(const SVGimage* image){

    if (image == NULL){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "{}");
        return ret;
    }

    int numRect = 0, numCircles = 0, numPaths = 0, numGroups = 0;

    List *lptr = image->allRectangles;
    numRect = getLength(lptr);

    lptr = image->allCircles;
    numCircles = getLength(lptr);

    lptr = image->allPaths;
    numPaths = getLength(lptr);

    lptr = image->allGroups;
    numGroups = getLength(lptr);

    lptr = NULL;

    char *json = calloc(350, sizeof(char));
    char *buffer = calloc(100, sizeof(char));

    strcpy(json, "{\"numRect\":");
    sprintf(buffer, "%d", numRect);
    strcat(json, buffer);
    strcat(json, ",\"numCirc\":");
    sprintf(buffer, "%d", numCircles);
    strcat(json, buffer);
    strcat(json, ",\"numPaths\":");
    sprintf(buffer, "%d", numPaths);
    strcat(json, buffer);
    strcat(json, ",\"numGroups\":");
    sprintf(buffer, "%d", numGroups);
    strcat(json, buffer);
    strcat(json, "}");

    free(buffer);

    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

//Attribute list to JSON string
char* attrListToJSON(const List *list){

    List *lptr = (List*) list;

    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }

    char *json = listToString(lptr, 0);
    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* circListToJSON(const List *list){
    
    List *lptr = (List*) list;
    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }

    char *json = listToString(lptr, CIRC);
    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* rectListToJSON(const List *list){

    List *lptr = (List*) list;
    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }

    char *json = listToString(lptr, RECT);
    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////


char* pathListToJSON(const List *list){

    List *lptr = (List*) list;
    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }

    char *json = listToString(lptr, PATH);
    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

char* groupListToJSON(const List *list){

    List *lptr = (List*) list;
    if (list == NULL ||getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }

    char *json = listToString(lptr, GROUP);
    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

//helper function to convert a sequence of JSON strings into a list of JSON strings
char *listToString(List *lptr, elementType type){

    void *ptr = NULL;
    char **stringsToJSON = calloc(1, sizeof(char*));
    char *json;
    int i = 0, j = 0, length = 0;

    ListIterator itr = createIterator(lptr);
    while(itr.current != NULL){
        if (type == 0){
            ptr = (Attribute *)itr.current->data;
            stringsToJSON[i] = attrToJSON(ptr);
        }
        else if (type == 1){ 
            ptr = (Circle *)itr.current->data;
            stringsToJSON[i] = circleToJSON(ptr);
        }
        else if (type == 2){
            ptr = (Rectangle *)itr.current->data;
            stringsToJSON[i] = rectToJSON(ptr);
        }
        else if (type == 3) {
            ptr = (Path *)itr.current->data;
            stringsToJSON[i] = pathToJSON(ptr);
        }
        else if (type == 4) {
            ptr = (Group *)itr.current->data;
            stringsToJSON[i] = groupToJSON(ptr);
        }
        length += (strlen(stringsToJSON[i]) + 250);
        i ++;
        stringsToJSON = realloc(stringsToJSON, (i+1)*sizeof(char *));
        itr.current = itr.current->next;
    }
    if (i > 0) json = calloc(length, sizeof(char));
    else json = calloc(250, sizeof(char));
    strcpy(json, "[");
    for (j = 0; j < i; j ++){
        strcat(json, stringsToJSON[j]);
        if (j != i - 1) strcat(json, ",");
        else strcat(json, "]");
        free(stringsToJSON[j]);
    }
    if (j == 0) strcat(json, "]");
    free(stringsToJSON);
    return json;
}

/////////////////////////////////////////////////////////////////////////////////////////

void JSONtoSVG(const char* svgString, char *name, char *schemaFile){

    if (!svgString) return;
    if (!name) return;

    int i = 0, j = 0;
    char title [255], description [255];
    SVGimage *image = NULL;

    while (j < 3 && i < strlen(svgString)){
        if (svgString[i] == '\"') j ++;
        i ++;
    }

    j = 0;

    while (svgString[i] != '\"'){
        title[j] = svgString[i];
        i ++;
        j ++;
    }

    i = 0;
    j = 0;

    while (j < 7 && i < strlen(svgString)){
        if (svgString[i] == '\"') j ++;
        i ++;
    }

    j = 0;

    while (svgString[i] != '\"'){
        description[j] = svgString[i];
        i ++;
        j ++;
    }

    image = calloc(1, sizeof(SVGimage));
    initSVGimage(image);

    strncpy(image->namespace, "http://www.w3.org/2000/svg", 255);
    strncpy(image->title, title, 255);
    strncpy(image->description, description, 255);

    if (validateSVGimage(image, schemaFile)){
        writeSVGimage(image, name, schemaFile);
    }

    deleteSVGimage(image);
    return;
}
/////////////////////////////////////////////////////////////////////////////////////////

Rectangle* JSONtoRect(const char* svgString){

    if (svgString == NULL) return NULL;

    Rectangle *rectangle = calloc(1, sizeof(Rectangle));
    rectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute,
                                                &compareAttributes);
    char *buffer = calloc(1000, sizeof(char));
    int i = 0;
    int j = 0;
    char *pend;

    while (svgString[i] != ':'){
        i ++;
    }
    i ++;
    while (svgString[i] != ','){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }

    rectangle->x = strtof(buffer, &pend);

    i +=5;
    j = 0;
    while (svgString[i] != ','){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }

    rectangle->y = strtof(buffer, &pend);

    i +=5;
    j = 0;
    while (svgString[i] != ','){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }

    rectangle->width = strtof(buffer, &pend);

    i +=5;
    j = 0;
    while (svgString[i] != ','){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }

    rectangle->height = strtof(buffer, &pend);

    i +=10;
    j = 0;
    while (svgString[i] != '\"'){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }
    if (j == 0){
        buffer[0] = ' ';
        buffer[1] = '\0';
    }

    strcpy(rectangle->units, buffer);
    free(buffer);

    return rectangle;
}

/////////////////////////////////////////////////////////////////////////////////////////

Circle* JSONtoCircle(const char* svgString){

    if (svgString == NULL) return NULL;

    Circle *circle = calloc(1, sizeof(Circle));
    circle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, 
                                             &compareAttributes);

    char *buffer = calloc(1000, sizeof(char));
    int i = 0;
    int j = 0;
    char *pend;

    while (svgString[i] != ':'){
        i ++;
    }
    i ++;

    while (svgString[i] != ','){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }

    circle->cx = strtof(buffer, &pend);

    i +=6;
    j = 0;
    while (svgString[i] != ','){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }

    circle->cy = strtof(buffer, &pend);

    i +=5;
    j = 0;
    while (svgString[i] != ','){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }

    circle->r = strtof(buffer, &pend);

    i +=10;
    j = 0;
    while (svgString[i] != '\"'){
        buffer[j] = svgString[i];
        i ++;
        j ++;
    }
    if (j == 0){
        buffer[0] = ' ';
        buffer[1] = '\0';
    }

    strcpy(circle->units, buffer);
    free(buffer);

    return circle;
}

/////////////////////////////////////////////////////////////////////////////////////////

void addComponent(SVGimage* image, elementType elemType, void* newComponent){

    if (!image || elemType < 1 || elemType > 4 || !newComponent) return;

    Circle *circlePtr = NULL;
    Rectangle *rectanglePtr = NULL;
    Path *pathPtr = NULL;
    List *lptr = NULL;
    char units[3];

    if (elemType == 1 || elemType == 2){

       if (elemType == 1) circlePtr = (Circle *) newComponent;
       if (elemType == 2) rectanglePtr = (Rectangle *) newComponent;

        if (elemType == 1){

            if (circlePtr->otherAttributes == NULL || circlePtr->r < 0) return;
            strncpy(units, circlePtr->units, 3);
            
        }

        else if (elemType == 2){

            if (!rectanglePtr->otherAttributes || rectanglePtr->width < 0 || 
                rectanglePtr->height < 0) return;
            strncpy(units, rectanglePtr->units, 3);    
        }  

        if (strcmp(units, "cm") != 0 && strcmp(units, "em") != 0 && 
            strcmp(units, "px") != 0 && strcmp(units, "pt") != 0 && 
            strcmp(units, "pc") != 0 && strcmp(units, "ex") != 0 && 
            strcmp(units, "mm") != 0 && strcmp(units, "in") != 0 && 
            strcmp(units, "\0") != 0) return;

        if (elemType == 1) {
            lptr = image->circles;
            insertBack(lptr, circlePtr);
        }

        else if (elemType == 2){
            lptr = image->rectangles;
            insertBack(lptr, rectanglePtr);
        }
    }

    else if (elemType == 3){

        pathPtr = (Path *)newComponent;

        if (!pathPtr->data || strcmp(pathPtr->data, "\0") == 0 ||
            !pathPtr->otherAttributes) return;

        lptr = image->paths;
        insertBack(lptr, pathPtr);
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool modifyCircleAttributes(Circle *circlePtr, Attribute *newAttribute, List *lptr){

    bool check = false;

    if (strcmp(newAttribute->name, "cx") == 0){
        check = changeAttributeFloatValue(&circlePtr->cx, newAttribute, NULL);
    }

    else if (strcmp(newAttribute->name, "cy") == 0){
        check = changeAttributeFloatValue(&circlePtr->cy, newAttribute, NULL);
    }

    else if (strcmp(newAttribute->name, "r") == 0){
        check = changeAttributeFloatValue(&circlePtr->r, newAttribute, "r");
    }

    else lptr = circlePtr->otherAttributes;

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool modifyRectangleAttributes(Rectangle *rectanglePtr, Attribute *newAttribute, 
                               List *lptr){
    bool check = false;

    if (strcmp(newAttribute->name, "x") == 0){
        check = changeAttributeFloatValue(&rectanglePtr->x, newAttribute, NULL);
    }

    else if (strcmp(newAttribute->name, "y") == 0){
        check = changeAttributeFloatValue(&rectanglePtr->y, newAttribute, NULL);
    }

    else if (strcmp(newAttribute->name, "width") == 0){
        check = changeAttributeFloatValue(&rectanglePtr->width, newAttribute, "width");
    }

    else if (strcmp(newAttribute->name, "height") == 0){
        check = changeAttributeFloatValue(&rectanglePtr->height, newAttribute, "height");
    }

    else lptr = rectanglePtr->otherAttributes;

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool modifyPathAttributes(Path *pathPtr, Attribute *newAttribute, List *lptr){
    bool check = false;

    if (strcmp(newAttribute->name, "d") == 0){
        check = changeAttributeString(pathPtr->data, newAttribute);
    }

    else lptr = pathPtr->otherAttributes;

    return check;
}

/////////////////////////////////////////////////////////////////////////////////////////

void setAttribute(SVGimage* image, elementType elemType, int elemIndex, 
                  Attribute* newAttribute){

    if (!image || elemType > 4 || elemType < 0 || !newAttribute || !newAttribute->name||
        !newAttribute->value|| strcmp(newAttribute->value, "\0") == 0 ||
        strcmp(newAttribute->name, "\0") == 0) return;

    Circle *circlePtr = NULL;
    Rectangle *rectanglePtr = NULL;
    Path *pathPtr = NULL;
    Group *groupPtr = NULL;
    List *lptr = NULL;
    ListIterator itr;
    int i = 0;
    int length;
    int check = 0;

    if (elemType == 0){
        lptr = image->otherAttributes;
        modifyOtherAttributes(lptr, newAttribute);
    }

    else if (elemType >= 1 && elemType <= 4){

        if (elemType == 1) lptr = image->circles;
        if (elemType == 2) lptr = image->rectangles;
        if (elemType == 3) lptr = image->paths;
        if (elemType == 4) lptr = image->groups;

        length = getLength(lptr);

        if (elemIndex >= length) return;

        else{

            itr = createIterator(lptr);

            for (i = 0; i < elemIndex; i ++){
                itr.current = itr.current->next;
            }

            if (elemType == 1) {

                circlePtr = (Circle *) itr.current->data;
                check = modifyCircleAttributes(circlePtr, newAttribute, lptr);
                if (check == 0) return;
            }

            else if (elemType == 2) {
                rectanglePtr = (Rectangle *) itr.current->data;
                check = modifyRectangleAttributes(rectanglePtr, newAttribute, lptr);
                if (check == 0) return;
            }

            else if (elemType == 3) {
                pathPtr = (Path *) itr.current->data;
                check = modifyPathAttributes(pathPtr, newAttribute, lptr);
            }
            else if (elemType == 4) {
                groupPtr = (Group *) itr.current->data;
                lptr = groupPtr->otherAttributes;
                check = 0;
            }
            if (check == 0) check = modifyOtherAttributes(lptr, newAttribute);
        }
    }
    
    if (check == 1) {
        deleteAttribute(newAttribute);
        newAttribute = NULL;
    }

    return;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool changeAttributeFloatValue(float *value, Attribute *newAtt, char *label){

    float test;
    char *pend;
    test = strtof(newAtt->value, &pend);
    if (label != NULL){
        if (strcmp(label, "r") == 0 || strcmp(label, "width") == 0 ||
            strcmp(label, "height") == 0){

            if (test < 0) return true;
        }
    }
    *value = test;
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool changeAttributeString(char *string, Attribute *newAtt){

    if (newAtt->name == NULL || newAtt->value == NULL || 
        strcmp(newAtt->name, "\0") == 0 || strcmp(newAtt->value, "\0") == 0){
        return false;
    }

    if (strlen(newAtt->value) > strlen(string)){
        string = realloc(string, (strlen(newAtt->value) + 1)*sizeof(char));
    }

    strcpy(string, newAtt->value);

    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool modifyOtherAttributes(List *lptr, Attribute *attributeToAdd){

    if (lptr == NULL || attributeToAdd == NULL){
        return 0;
    }
    ListIterator itr = createIterator(lptr);
    Attribute *attPtr;
    bool check = false;

    while (itr.current != NULL){

        attPtr = (Attribute *)itr.current->data;

        if (strcmp(attPtr->name, attributeToAdd->name) == 0){
            check = true;
            break;
        }

        itr.current = itr.current->next;
    }

    if (check && strlen(attributeToAdd->value) > strlen(attPtr->value)){
        attPtr->value = realloc(attPtr->value, 
                               (strlen(attributeToAdd->value) + 250)*sizeof(char));
    }

    if (check) strcpy(attPtr->value, attributeToAdd->value);

    if (!check) insertBack(lptr, attributeToAdd);

    return check;
}
/////////////////////////////////////////////////////////////////////////////////////////

char *groupToString(void *data)
{
    if (data == NULL)
        return NULL;
    Group *group = (Group *)data;
    char *description = calloc(1000000, sizeof(char));
    int mem = 1000000;
    char *string;

    ListIterator itr1 = createIterator(group->otherAttributes);
    while (itr1.current != NULL)
    {
        strcat(description, "\n\nother attributes in group:\n");
        string = attributeToString(itr1.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr1.current = itr1.current->next;
    }

    ListIterator itr2 = createIterator(group->rectangles);
    while (itr2.current != NULL)
    {
        strcat(description, "\n\nrectangle in group:\n");
        string = rectangleToString(itr2.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr2.current = itr2.current->next;
    }

    ListIterator itr3 = createIterator(group->circles);
    while (itr3.current != NULL)
    {
        strcat(description, "\n\nCircle in group:\n");
        string = circleToString(itr3.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr3.current = itr3.current->next;
    }

    ListIterator itr4 = createIterator(group->paths);
    while (itr4.current != NULL)
    {
        strcat(description, "\n\npath in group:\n");
        string = pathToString(itr4.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr4.current = itr4.current->next;
    }

    if (getLength(group->groups) > 0)
    {
        strcat(description, "\n\nGROUP WITHIN GROUP DESCRIPTION\n");
        strcat(description, "*********************************************************\n");
    }
    else
    {
        strcat(description, "*********************************************************\n");
    }

    ListIterator itr5 = createIterator(group->groups);
    while (itr5.current != NULL)
    {
        string = groupToString(itr5.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr5.current = itr5.current->next;
    }

    strcat(description, "\0");

    return description;
}

/////////////////////////////////////////////////////////////////////////////////////////

char *attributeToString(void *data)
{
    if (data == NULL)
        return NULL;
    Attribute *att = (Attribute *)data;
    char *description = calloc(strlen(att->name) + strlen(att->value) + 40, sizeof(char));  
    strcpy(description, "\0");
    if (strcmp(att->name, "\0") == 0 && strcmp(att->value, "\0") == 0)
        return description;
    strcpy(description, "Attribute name: ");
    strcat(description, att->name);
    strcat(description, "\nAttribute value: ");
    strcat(description, att->value);
    strcat(description, "\n\0");
    return description;
}

/////////////////////////////////////////////////////////////////////////////////////////

char *circleToString(void *data)
{
    if (data == NULL) return NULL;

    Circle *circle = (Circle *)data;
    int mem = 10000;
    char *description = calloc(mem, sizeof(char));
    char buffer[100];

    if (strcmp(circle->units, "\0") != 0)
    {
        strcpy(description, "\nAttribute name: units\nValue: ");
        strcat(description, circle->units);
    }
    strcat(description, "\nAttribute name: cx\nValue: ");
    sprintf(buffer, "%f", circle->cx);
    strcat(description, buffer);
    if (strcmp(circle->units, "\0") != 0)
        strcat(description, circle->units);
    strcat(description, "\nAttribute name: cy\nValue: ");
    sprintf(buffer, "%f", circle->cy);
    strcat(description, buffer);
    if (strcmp(circle->units, "\0") != 0)
        strcat(description, circle->units);
    strcat(description, "\nAttribute name: r\nValue: ");
    sprintf(buffer, "%f", circle->r);
    strcat(description, buffer);
    if (strcmp(circle->units, "\0") != 0)
        strcat(description, circle->units);
    if (getLength(circle->otherAttributes) > 0)
        strcat(description, "\n\nOther Attributes:\n");

    ListIterator itr = createIterator(circle->otherAttributes);
    char *string;
    while (itr.current != NULL)
    {
        string = attributeToString(itr.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr.current = itr.current->next;
    }

    strcat(description, "\0");

    return description;
}

/////////////////////////////////////////////////////////////////////////////////////////

char *rectangleToString(void *data)
{
    if (data == NULL)
        return NULL;
    Rectangle *rectangle = (Rectangle *)data;
    int mem = 10000;
    char *description = calloc(mem, sizeof(char));
    char buffer[100];

    if (strcmp(rectangle->units, "\0") != 0)
    {
        strcpy(description, "\nAttribute name: units\nValue: ");
        strcat(description, rectangle->units);
    }

    strcat(description, "\nAttribute name: x\nValue: ");
    sprintf(buffer, "%f", rectangle->x);
    strcat(description, buffer);
    if (strcmp(rectangle->units, "\0") != 0)
        strcat(description, rectangle->units);
    strcat(description, "\nAttribute name: y\nValue: ");
    sprintf(buffer, "%f", rectangle->y);
    strcat(description, buffer);
    if (strcmp(rectangle->units, "\0") != 0)
        strcat(description, rectangle->units);
    strcat(description, "\nAttribute name: width\nValue: ");
    sprintf(buffer, "%f", rectangle->width);
    strcat(description, buffer);
    if (strcmp(rectangle->units, "\0") != 0)
        strcat(description, rectangle->units);
    strcat(description, "\nAttribute name: height\nValue: ");
    sprintf(buffer, "%f", rectangle->height);
    strcat(description, buffer);
    if (strcmp(rectangle->units, "\0") != 0)
        strcat(description, rectangle->units);
    if (getLength(rectangle->otherAttributes) > 0)
        strcat(description, "\n\nOther Attributes:\n");

    ListIterator itr = createIterator(rectangle->otherAttributes);
    char *string;
    while (itr.current != NULL)
    {
        string = attributeToString(itr.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr.current = itr.current->next;
    }

    strcat(description, "\0");

    return description;
}

/////////////////////////////////////////////////////////////////////////////////////////

char *pathToString(void *data)
{
    if (data == NULL) return NULL;
    Path *path = (Path *)data;
    if (path->data == NULL) return NULL;

    char *description;
    int mem = 10000;
    description = calloc(mem, sizeof(char));
    ListIterator itr;
    char *string;

    strcpy(description, "\nPath:\n");
    strcat(description, "\nAttribute name: data\nValue: ");
    strcat(description, path->data);
    strcat(description, "\n");

    itr = createIterator(path->otherAttributes);
    
    if (getLength(path->otherAttributes) > 0)
        strcat(description, "\nOther Attributes:\n");
    while (itr.current != NULL)
    {
        string = attributeToString(itr.current->data);
        if (strlen(string) + strlen(description) + 2 >= mem)
        {
            mem = mem * 2;
            description = realloc(description, mem * sizeof(char));
        }
        strcat(description, string);
        free(string);
        itr.current = itr.current->next;
    }

    strcat(description, "\0");

    return description;
}

/////////////////////////////////////////////////////////////////////////////////////////

int compareRectangles(const void *first, const void *second)
{
    return 0;
}
int compareAttributes(const void *first, const void *second)
{
    return 0;
}
int compareGroups(const void *first, const void *second)
{
    return 0;
}
int compareCircles(const void *first, const void *second)
{
    return 0;
}
int comparePaths(const void *first, const void *second)
{
    return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////