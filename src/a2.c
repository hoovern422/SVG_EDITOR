
/*******Module 3 A2*****************************************************************************************************************/

//attribute to JSON string
char* attrToJSON(const Attribute *a){
    //printf("Calling: attrToJson\n");
    if (a == NULL){
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

//circle to JSON string
char* circleToJSON(const Circle *c){
    //printf("Calling: circleToJson\n");
    if (c == NULL){
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

//rectangle to JSON string
char* rectToJSON(const Rectangle *r){
    //printf("Calling: rectToJson\n");
    if (r == NULL){
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

//path to JSON string
char* pathToJSON(const Path *p){
    //printf("Calling: pathToJson\n");
    if (p == NULL){
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

//group to JSON string
char* groupToJSON(const Group *g){
    //printf("Calling: groupToJson\n");
    if (g == NULL || g->rectangles == NULL || g->circles == NULL || g->paths == NULL || g->groups == NULL || g->otherAttributes == NULL){
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

// SVG to JSON string
char* SVGtoJSON(const SVGimage* image){
    //printf("Calling: SVGToJson\n");
    if (image == NULL){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "{}");
        return ret;
    }
    SVGimage *img = (SVGimage *)image;
    int numRect = 0, numCircles = 0, numPaths = 0, numGroups = 0;
    List *lptr = getRects(img);
    numRect = getLength(lptr);
    freeShapesList(lptr);
    lptr = NULL;
    lptr = getCircles(img);
    numCircles = getLength(lptr);
    freeShapesList(lptr);
    lptr = NULL;
    lptr = getPaths(img);
    numPaths = getLength(lptr);
    freeShapesList(lptr);
    lptr = NULL;
    lptr = getGroups(img);
    numGroups = getLength(lptr);
    freeShapesList(lptr);
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

//Attribute list to JSON string
char* attrListToJSON(const List *list){
    //printf("Calling: attrListToJSON\n");
    List *lptr = (List*) list;
    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }
    char *json = listToString(lptr, 0);
    return json;
}

//circle list to JSON string
char* circListToJSON(const List *list){
    //printf("Calling: circListToJSON\n");
    List *lptr = (List*) list;
    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }
    char *json = listToString(lptr, CIRC);
    return json;
}

//rectangle list to JSON string
char* rectListToJSON(const List *list){
    //printf("Calling: rectListToJSON\n");
    List *lptr = (List*) list;
    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }
    char *json = listToString(lptr, RECT);
    return json;
}

//path list to JSON string
char* pathListToJSON(const List *list){
    //printf("Calling: pathListToJSON\n");
    List *lptr = (List*) list;
    if (list == NULL || getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }
    char *json = listToString(lptr, PATH);
    return json;
}

//group list to JSON string
char* groupListToJSON(const List *list){
    //printf("Calling: groupListToJSON\n");
    List *lptr = (List*) list;
    if (list == NULL ||getLength(lptr) == 0){
        char *ret = calloc(3, sizeof(char));
        strcpy(ret, "[]");
        return ret;
    }
    char *json = listToString(lptr, GROUP);
    return json;
}


//helper function to convert a sequence of JSON strings into a list of JSON strings
char *listToString(List *lptr, elementType type){
    //printf("Calling: listToString\n");
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

/********Module 2 A2******************************************************************************************************************/

//function to add a component to an SVG image
void addComponent(SVGimage* image, elementType elemType, void* newComponent){
    //printf("Calling: addComponent\n");
    if (image == NULL || elemType < 1 || elemType > 4 || newComponent == NULL){
        return;
    }
    Circle *circlePtr = NULL;
    Rectangle *rectanglePtr = NULL;
    Path *pathPtr = NULL;
    List *lptr = NULL;
    if (elemType == 1){
        circlePtr = (Circle *)newComponent;
        if (circlePtr->otherAttributes == NULL || circlePtr->r < 0){
            return;
        }
        if (strcmp(circlePtr->units, "cm") != 0 && strcmp(circlePtr->units, "em") != 0 && strcmp(circlePtr->units, "px") != 0 && strcmp(circlePtr->units, "pt") != 0 && strcmp(circlePtr->units, "pc") != 0 &&
                strcmp(circlePtr->units, "ex") != 0 && strcmp(circlePtr->units, "mm") != 0 && strcmp(circlePtr->units, "in") != 0 && strcmp(circlePtr->units, "\0") != 0){
            return;
        }
        lptr = image->circles;
        insertBack(lptr, circlePtr);
    }
    else if (elemType == 2){
        rectanglePtr = (Rectangle *)newComponent;
        if (rectanglePtr->otherAttributes == NULL || rectanglePtr->width < 0 || rectanglePtr->height < 0){
            return;
        }
        if (strcmp(rectanglePtr->units, "cm") != 0 && strcmp(rectanglePtr->units, "em") != 0 && strcmp(rectanglePtr->units, "px") != 0 && strcmp(rectanglePtr->units, "pt") != 0 && strcmp(rectanglePtr->units, "pc") != 0 &&
                strcmp(rectanglePtr->units, "ex") != 0 && strcmp(rectanglePtr->units, "mm") != 0 && strcmp(rectanglePtr->units, "in") != 0 && strcmp(rectanglePtr->units, "\0") != 0){
            return;
        }
        lptr = image->rectangles;
        insertBack(lptr, rectanglePtr);
    }
    else if (elemType == 3){
        pathPtr = (Path *)newComponent;
        if (pathPtr->data == NULL || strcmp(pathPtr->data, "\0") == 0 || pathPtr->otherAttributes == NULL){
            return;
        }
        lptr = image->paths;
        insertBack(lptr, pathPtr);
    }
    return;
}

/*********************************************************************************************************************************************/
//function to set Attributes in an SVG image
void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute){
    //printf("Calling: setAttribute\n");
    if (image == NULL || elemType > 4 || elemType < 0 || newAttribute == NULL || newAttribute->name == NULL || newAttribute->value == NULL  || strcmp(newAttribute->value, "\0") == 0 || strcmp(newAttribute->name, "\0") == 0){
        return;
    }
    List *lptr = NULL;
    ListIterator *itr = calloc(1, sizeof(ListIterator));
    Circle *circlePtr = NULL;
    Rectangle *rectanglePtr = NULL;
    Path *pathPtr = NULL;
    Group *groupPtr = NULL;
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
        if (elemIndex >= length){
            free(itr);
            //deleteAttribute(newAttribute);//REMOVE THIS!!!!!!!
            return;
        }
        else{
            *itr = createIterator(lptr);
            for (i = 0; i < elemIndex; i ++){
                itr->current = itr->current->next;
            }
            if (elemType == 1) {
                circlePtr = (Circle *) itr->current->data;
                if (strcmp(newAttribute->name, "cx") == 0){
                    check = changeAttributeFloatValue(&circlePtr->cx, newAttribute, NULL);
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else if (strcmp(newAttribute->name, "cy") == 0){
                    check = changeAttributeFloatValue(&circlePtr->cy, newAttribute, NULL);
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else if (strcmp(newAttribute->name, "r") == 0){
                    check = changeAttributeFloatValue(&circlePtr->r, newAttribute, "r");
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else{
                    lptr = circlePtr->otherAttributes;
                    check = 0;
                }
            }
            else if (elemType == 2) {
                rectanglePtr = (Rectangle *) itr->current->data;
                if (strcmp(newAttribute->name, "x") == 0){
                    check = changeAttributeFloatValue(&rectanglePtr->x, newAttribute, NULL);
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else if (strcmp(newAttribute->name, "y") == 0){
                    check = changeAttributeFloatValue(&rectanglePtr->y, newAttribute, NULL);
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else if (strcmp(newAttribute->name, "width") == 0){
                    check = changeAttributeFloatValue(&rectanglePtr->width, newAttribute, "width");
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else if (strcmp(newAttribute->name, "height") == 0){
                    check = changeAttributeFloatValue(&rectanglePtr->height, newAttribute, "height");
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else{
                    lptr = rectanglePtr->otherAttributes;
                    check = 0;
                }
            }
            else if (elemType == 3) {
                pathPtr = (Path *) itr->current->data;
                if (strcmp(newAttribute->name, "d") == 0){
                    check = changeAttributeString(pathPtr->data, newAttribute);
                    if (check == 0){
                        free (itr);
                        return;
                    }
                }
                else{
                    lptr = pathPtr->otherAttributes;
                    check = 0;
                }
            }
            else if (elemType == 4) {
                groupPtr = (Group *) itr->current->data;
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
    free(itr);
    return;
}

//helper function to change a floating point value in an attribute
int changeAttributeFloatValue(float *value, Attribute *newAtt, char *label){
    //printf("Calling: changeAttributeFloatValue\n");
    float test;
    char *pend;
    test = strtof(newAtt->value, &pend);
    if (label != NULL){
        if (strcmp(label, "r") == 0 || strcmp(label, "width") == 0 ||strcmp(label, "height") == 0){
            if (test < 0){
                return 0;
            }
        }
    }
    *value = test;
    return 1;
}

//helper function to change a string in an attribute
int changeAttributeString(char *string, Attribute *newAtt){
    //printf("Calling: changeAttributeStringValue\n");
    if (newAtt->name == NULL || newAtt->value == NULL || strcmp(newAtt->name, "\0") == 0 || strcmp(newAtt->value, "\0") == 0){
        return 0;
    }
    if (strlen(newAtt->value) > strlen(string)){
        string = realloc(string, (strlen(newAtt->value) + 1)*sizeof(char));
    }
    strcpy(string, newAtt->value);
    return 1;
}

//helper function modify or add an other attribute for any element in the SVG node
int modifyOtherAttributes(List *lptr, Attribute *attributeToAdd){
    //printf("Calling: modifyOtherAttributes\n");
    if (lptr == NULL || attributeToAdd == NULL){
        return 0;
    }
    ListIterator itr = createIterator(lptr);
    Attribute *attPtr;
    int check = 0;
    while (itr.current != NULL){
        attPtr = (Attribute *)itr.current->data;
        if (strcmp(attPtr->name, attributeToAdd->name) == 0){
            check = 1;
            break;
        }
        itr.current = itr.current->next;
    }
    if (check == 1 && strlen(attributeToAdd->value) > strlen(attPtr->value)){
        attPtr->value = realloc(attPtr->value, (strlen(attributeToAdd->value) + 250)*sizeof(char));
    }
    if (check == 1) {
        strcpy(attPtr->value, attributeToAdd->value);
    }
    if (check == 0) insertBack(lptr, attributeToAdd);
    return check;
}

/*********Module 1 A2*************************************************************************************************************************************************/
bool writeSVGimage(SVGimage* image, char* fileName){
    //printf("Calling: writeSVGimage\n");
    if (image == NULL || fileName == NULL || strcmp(fileName, "\0") == 0 || image->rectangles == NULL || image->circles == NULL || image->paths == NULL || image->groups == NULL || image->otherAttributes == NULL){
        return false;
    }
    if (validateConstraints(image) == false) return false;
    xmlNsPtr nsPtr = NULL;
    xmlDocPtr doc = NULL;
    doc = buildTree(image, nsPtr);
    if (doc == NULL){
        xmlFreeDoc(doc);
        xmlFreeNs(nsPtr);
        return false;
    }
    xmlSaveFile(fileName, doc);
    xmlFreeDoc(doc);
    xmlFreeNs(nsPtr);
    xmlCleanupParser();
    return true;
}


bool validateSVGimage(SVGimage* image, char* schemaFile){
    //printf("Calling: validateSVGimage\n");
    if (image == NULL || schemaFile == NULL || strcmp(schemaFile, "\0") == 0) return false;

    //validate the constraints of the svg image
    bool constraintsAreMet = false;
    constraintsAreMet = validateConstraints(image);
    if (constraintsAreMet == false) return false;

    xmlDocPtr doc = NULL;
    xmlNsPtr nsPtr = NULL;

    /*Must have a namespace and non-null title, description*/
    if (strcmp(image->namespace, "\0") == 0 || image->namespace == NULL || image->title == NULL || image->description == NULL){
        return false;
    }

    //build the tree
    doc = buildTree(image, nsPtr);
    if (doc == NULL){
        xmlFreeDoc(doc);
        xmlFreeNs(nsPtr);
        //xmlCleanupParser();
        //xmlMemoryDump();
        return false;
    }

    //verifying the validity of the newly generated tree against a schema file
    int valid = checkAgainstSchemaFile(schemaFile, doc);

    if (valid != 0){
        xmlFreeDoc(doc);
        xmlFreeNs(nsPtr);
        return false;
    }
    xmlFreeDoc(doc);
    xmlFreeNs(nsPtr);
    //xmlCleanupParser();
    //xmlMemoryDump();

    return true;
}
/******************************************************************************************************************************************************/

//Helper function to build the XML tree from a SVGimage
xmlDocPtr buildTree(SVGimage *image, xmlNsPtr nsPtr){
    //printf("Calling: buildTree\n");
    if (image == NULL || image->namespace == NULL || strcmp(image->namespace, "\0") == 0 || image->title == NULL || image->description == NULL){
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
    if (strcmp(image->title, "\0") == 0) xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST "\0"); //new child to the root node for title
    else xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST image->title);

    //description
    if (strcmp(image->description, "\0") == 0) xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST "\0"); //new child to the root node for description
    else xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST image->description);
    
    //adding attributes to svg node in loop
    ListIterator itr = createIterator(image->otherAttributes);
    Attribute *attribute = NULL;

    for (itr.current = itr.current; itr.current != NULL; itr.current = itr.current->next){
        attribute = (Attribute*)itr.current->data;
        xmlNewProp(root_node, BAD_CAST attribute->name, BAD_CAST attribute->value);
    }

    //add all rectangles, circles paths and groups for svg node every group node (in order)
    addListsToTree(root_node, image);
    //xmlCleanupParser();
    return doc;
}
/***************************************************************************************************************************************/
//helper function for adding lists to trees
void addListsToTree(xmlNodePtr cur_node, SVGimage *image){
    //printf("Calling: addListsToTree\n");
    List *lptr = NULL;
    //add rectangles to svg
    lptr = image->rectangles;
    addRectangleChild(cur_node, lptr);
    //add circles to svg
    lptr = image->circles;
    addCircleChild(cur_node, lptr);
    //add paths to svg
    lptr = image->paths;
    addPathChild(cur_node, lptr);
    //enter the addGroupChild function with svg groups list, which will do recursive calls to build the rest of the tree
    lptr = image->groups;
    addGroupChild(cur_node, lptr);
}

void addGroupChild(xmlNodePtr cur_node, List *groups){
    //printf("Calling: addGroupChild\n");
    xmlNodePtr group = NULL;
    Group *groupPtr = NULL;
    Attribute *attrPtr;
    ListIterator groupItr = createIterator(groups);
    ListIterator attrItr;

    List* lptr = NULL;
    while(groupItr.current != NULL){
        //add new child
        group = xmlNewChild(cur_node, NULL, BAD_CAST "g", BAD_CAST "\0");
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
        attrItr = createIterator(groupPtr->otherAttributes);
        while(attrItr.current != NULL){
            attrPtr = (Attribute *) attrItr.current->data; 
            xmlNewProp(group, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attrItr.current = attrItr.current->next;
        }
        //recursively call addGroupsChild
        lptr = groupPtr->groups;
        addGroupChild(group, lptr);
        //update list pointer
        groupItr.current = groupItr.current->next;
    }
}

void addRectangleChild(xmlNodePtr cur_node, List *rectangles){
    //printf("Calling: addRectangleChild\n");
    xmlNodePtr rect = NULL;
    char *buff = calloc (1000, sizeof(char));
    Rectangle *rectPtr = NULL;
    Attribute *attrPtr = NULL;
    ListIterator rectItr = createIterator(rectangles);
    ListIterator *attrItr = calloc (1, sizeof(ListIterator));

    /*first loop itterates through list elements*/
    while(rectItr.current != NULL){
        rect = xmlNewChild(cur_node, NULL, BAD_CAST "rect", BAD_CAST "\0");
        rectPtr = (Rectangle *)rectItr.current->data;

        //x property
        sprintf(buff, "%f", rectPtr->x);
        if (rectPtr->units != NULL && strcmp(rectPtr->units, "\0") != 0) strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "x", BAD_CAST buff);

        //y property
        sprintf(buff, "%f", rectPtr->y);
        if (rectPtr->units != NULL && strcmp(rectPtr->units, "\0") != 0) strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "y", BAD_CAST buff);

        //width property
        sprintf(buff, "%f", rectPtr->width);
        if (rectPtr->units != NULL && strcmp(rectPtr->units, "\0") != 0) strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "width", BAD_CAST buff);

        //height property
        sprintf(buff, "%f", rectPtr->height);
        if (rectPtr->units != NULL && strcmp(rectPtr->units, "\0") != 0) strcat(buff, rectPtr->units);
        xmlNewProp(rect, BAD_CAST "height", BAD_CAST buff);

        //otherAttributes
        *attrItr = createIterator(rectPtr->otherAttributes);
        while (attrItr->current != NULL){
            attrPtr = (Attribute *) attrItr->current->data; 
            xmlNewProp(rect, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attrItr->current = attrItr->current->next;
        }

        rectItr.current = rectItr.current->next;
    }

    free(buff);
    free(attrItr);
}

void addCircleChild(xmlNodePtr cur_node, List *circles){
    //printf("Calling: addCircleChild\n");
    xmlNodePtr circle = NULL;
    char *buff = calloc (1000, sizeof(char));
    Circle *circlePtr = NULL;
    Attribute *attrPtr = NULL;
    ListIterator circleItr = createIterator(circles);
    ListIterator *attrItr = calloc (1, sizeof(ListIterator));

    /*first loop itterates through list elements*/
    while(circleItr.current != NULL){
        circle = xmlNewChild(cur_node, NULL, BAD_CAST "circle", BAD_CAST "\0");
        circlePtr = (Circle *)circleItr.current->data;

        //cx property
        sprintf(buff, "%f", circlePtr->cx);
        if (circlePtr->units != NULL && strcmp(circlePtr->units, "\0") != 0) strcat(buff, circlePtr->units);
        xmlNewProp(circle, BAD_CAST "cx", BAD_CAST buff);

        //cy property
        sprintf(buff, "%f", circlePtr->cy);
        if (circlePtr->units != NULL && strcmp(circlePtr->units, "\0") != 0) strcat(buff, circlePtr->units);
        xmlNewProp(circle, BAD_CAST "cy", BAD_CAST buff);

        //r property
        sprintf(buff, "%f", circlePtr->r);
        if (circlePtr->units != NULL && strcmp(circlePtr->units, "\0") != 0) strcat(buff, circlePtr->units);
        xmlNewProp(circle, BAD_CAST "r", BAD_CAST buff);

        //otherAttributes
        *attrItr = createIterator(circlePtr->otherAttributes);
        while (attrItr->current != NULL){
            attrPtr = (Attribute *) attrItr->current->data; 
            xmlNewProp(circle, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attrItr->current = attrItr->current->next;
        }

        circleItr.current = circleItr.current->next;
    }

    free(buff);
    free(attrItr);
}

void addPathChild(xmlNodePtr cur_node, List *paths){
    //printf("Calling: addPathChild\n");
    xmlNodePtr path = NULL;
    Path *pathPtr = NULL;
    Attribute *attrPtr = NULL;
    ListIterator pathItr = createIterator(paths);
    ListIterator *attrItr = calloc (1, sizeof(ListIterator));

    /*first loop itterates through list elements*/
    while(pathItr.current != NULL){
        path = xmlNewChild(cur_node, NULL, BAD_CAST "path", BAD_CAST "\0");
        pathPtr = (Path *)pathItr.current->data;

        //d property
        xmlNewProp(path, BAD_CAST "d", BAD_CAST pathPtr->data);

        //otherAttributes
        *attrItr = createIterator(pathPtr->otherAttributes);
        while (attrItr->current != NULL){
            attrPtr = (Attribute *) attrItr->current->data; 
            xmlNewProp(path, BAD_CAST attrPtr->name, BAD_CAST attrPtr->value);
            attrItr->current = attrItr->current->next;
        }

        pathItr.current = pathItr.current->next;
    }
    free(attrItr);
}
/********************************************************************************************************************************************/

//creating a valid SVG image
SVGimage* createValidSVGimage(char* fileName, char* schemaFile)
{
    //printf("Calling: createValidSVGimage\n");
    if (fileName == NULL || strcmp(fileName, "\0") == 0 || schemaFile == NULL || strcmp(schemaFile, "\0") == 0)
    {
        xmlCleanupParser();
        return NULL;
    }

    xmlDocPtr doc = NULL;
    doc = xmlReadFile(fileName, NULL, 0);
    if (doc == NULL)
    {
        xmlFreeDoc(doc);
        xmlSchemaCleanupTypes();
        xmlCleanupParser();
        xmlMemoryDump();
        return NULL;
    }

    int valid = checkAgainstSchemaFile(schemaFile, doc);

    if (valid != 0){
        xmlFreeDoc(doc);
        xmlSchemaCleanupTypes();
        xmlCleanupParser();
        xmlMemoryDump();
        return NULL;
    }

    SVGimage *image = createSVGimage(fileName);

    if (image == NULL){
        xmlFreeDoc(doc);
        xmlSchemaCleanupTypes();
        xmlCleanupParser();
        xmlMemoryDump();
        return NULL;
    }

    bool truth = validateSVGimage(image, schemaFile);

    xmlFreeDoc(doc);
    xmlSchemaCleanupTypes();
    xmlCleanupParser();
    xmlMemoryDump();

    if (truth == true) return image;
    else return NULL;
}

//helper function for checking against schema file
int checkAgainstSchemaFile(char *schemaFile, xmlDocPtr doc){
    //printf("Calling: checkAgainstSchemaFile\n");
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    ctxt = xmlSchemaNewParserCtxt(schemaFile);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    xmlSchemaValidCtxtPtr validCtxt;
    int valid;

    validCtxt = xmlSchemaNewValidCtxt(schema);
    valid = xmlSchemaValidateDoc(validCtxt, doc);

    xmlSchemaFreeValidCtxt(validCtxt);
    if (schema != NULL) xmlSchemaFree(schema);
    //xmlCleanupParser();
    return valid;
}
/**********************************************************************************************************************************************/

//validating the constraints of an svg image according to SVGParser.h and the libxml2 documentation
bool validateConstraints(SVGimage *image){
    //printf("Calling: validateConstraints\n");
    if (image == NULL) return false;
    List *lptr = NULL;
    List *lptrAtt = NULL;
    ListIterator *itr = calloc(1, sizeof(ListIterator));
    ListIterator *attItr = calloc(1, sizeof(ListIterator));
    Rectangle *rectangle = NULL;
    Circle *circle = NULL;      
    Path *path = NULL;
    Group *group = NULL;
    Attribute *attribute = NULL;
    bool valid = true;

    //validate the SVG Node
    if (image->rectangles == NULL || image->circles == NULL || image->paths == NULL || image->groups == NULL || image->otherAttributes == NULL || image->namespace == NULL
    || strcmp(image->namespace, "\0") == 0  || strlen(image->namespace) > 255 || strlen(image->title) > 255 || strlen(image->description) > 255){
        valid = false;
        free(itr);
        free(attItr);
        return valid;
    }
    //get all other attributes from the svg node
    lptrAtt = image->otherAttributes;
    *attItr = createIterator(lptrAtt);
    while(attItr->current != NULL){
            attribute = (Attribute *)attItr->current->data;
            if (attribute == NULL || attribute->name == NULL || attribute->value == NULL){
                valid = false;
                freeShapesList(lptr);
                free(itr);
                free(attItr);
                return valid;
            }
            attItr->current = attItr->current->next;
        }

    //validate the rectangles in the image
    lptr = getRects(image);
    *itr = createIterator(lptr);
    while (itr->current != NULL){
        rectangle = (Rectangle *)itr->current->data;
        if (rectangle == NULL || rectangle->height < 0 || rectangle->width < 0 || rectangle->otherAttributes == NULL){
            valid = false;
            freeShapesList(lptr);
            free(itr);
            free(attItr);
            return valid;
        }
        //get all other attributes from the rectangles in the image
        lptrAtt = rectangle->otherAttributes;
        *attItr = createIterator(lptrAtt);
        while(attItr->current != NULL){
            attribute = (Attribute *)attItr->current->data;
            if (attribute == NULL || attribute->name == NULL || attribute->value == NULL){
                valid = false;
                freeShapesList(lptr);
                free(itr);
                free(attItr);
                return valid;
            }
            attItr->current = attItr->current->next;
        }
        itr->current = itr->current->next;
    }
    freeShapesList(lptr);

    //validate the circles in the image
    lptr = getCircles(image);
    *itr = createIterator(lptr);
    while(itr->current != NULL){
        circle = (Circle *)itr->current->data;
        if (circle == NULL || circle->r < 0 || circle->otherAttributes == NULL){
            valid = false;
            freeShapesList(lptr);
            free(itr);
            free(attItr);
            return valid;
        }
        //get all other attributes from the circles in the image
        lptrAtt = circle->otherAttributes;
        *attItr = createIterator(lptrAtt);
        while(attItr->current != NULL){
            attribute = (Attribute *)attItr->current->data;
            if (attribute == NULL || attribute->name == NULL || attribute->value == NULL){
                valid = false;
                freeShapesList(lptr);
                free(itr);
                free(attItr);
                return valid;
            }
            attItr->current = attItr->current->next;
        }
        itr->current = itr->current->next;
    }
    freeShapesList(lptr);

    //validate paths in the image
    lptr = getPaths(image);
    *itr = createIterator(lptr);
    while(itr->current != NULL){
        path = (Path *)itr->current->data;
        if (path == NULL || path->data == NULL || strcmp(path->data, "\0") == 0 || path->otherAttributes == NULL){
            valid = false;
        }
        //get all other attributes from the paths in the image
       lptrAtt = path->otherAttributes;
        *attItr = createIterator(lptrAtt);
        while(attItr->current != NULL){
            attribute = (Attribute *)attItr->current->data;
            if (attribute == NULL || attribute->name == NULL || attribute->value == NULL){
                valid = false;
                freeShapesList(lptr);
                free(itr);
                free(attItr);
                return valid;
            }
            attItr->current = attItr->current->next;
        }
        itr->current = itr->current->next;
    }
    freeShapesList(lptr);

    //validate the groups in the image
    lptr = getGroups(image);
    *itr= createIterator(lptr);
    while(itr->current != NULL){
        group = (Group *)itr->current->data;
        if (group == NULL || group->rectangles == NULL || group->circles == NULL || group->paths == NULL || group->groups == NULL || group->otherAttributes == NULL){
            valid = false;
        }
        //get all other attributes from the groups in the image
        lptrAtt = group->otherAttributes;
        *attItr = createIterator(lptrAtt);
        while(attItr->current != NULL){
            attribute = (Attribute *)attItr->current->data;
            if (attribute == NULL || attribute->name == NULL || attribute->value == NULL){
                valid = false;
                freeShapesList(lptr);
                free(itr);
                free(attItr);
                return valid;
            }
            attItr->current = attItr->current->next;
        }
        itr->current = itr->current->next;
    }
    freeShapesList(lptr);

    //free statements
    free(itr);
    free(attItr);
    return valid;
     
}


/**********************************************************************************************************************************************/

//helper function for printing the newly generated tree
void printer(xmlNodePtr root_node){
  //printf("Calling: printer\n");
  xmlNodePtr cur_node = NULL;

  for (cur_node = root_node; cur_node != NULL; cur_node = cur_node->next){
        if (cur_node->type == XML_ELEMENT_NODE){
            if (cur_node->ns != NULL) printf("********************NEW NODE*********************\nnamespace for %s: %s\n",(char*)cur_node->name, cur_node->ns->href);
            if (strcmp((char*)cur_node->name, "svg") != 0){
                printf("parent node: %s\n", (char*)cur_node->parent->name);
            }
            if(strcmp((char*)cur_node->name, "title") == 0){
                if (cur_node->children != NULL && cur_node->children->content != NULL) printf("title: %s\n\n", cur_node->children->content);
            }
            if(strcmp((char*)cur_node->name, "desc") == 0){
               if (cur_node->children != NULL && cur_node->children->content != NULL) printf("description: %s\n\n", cur_node->children->content);
            }

            xmlAttrPtr attr;
            for (attr = cur_node->properties; attr != NULL; attr = attr->next)
            {
                xmlNode *value = attr->children;
                char *attrName = (char*)attr->name;
                char *cont = (char*)value->content;
                printf("name of attribute for %s: %s\n",(char*)cur_node->name, attrName);
                printf("content of attribute for %s: %s\n\n",(char*)cur_node->name, cont);
            }

        }
        printer(cur_node->children);
    }
}
/*********************************************************************************************************************************************************************/
SVGimage* JSONtoGPX(const char* svgString){
    return NULL;
}
Rectangle* JSONtoRect(const char* svgString){
    return NULL;
}
Circle* JSONtoCircle(const char* svgString){
    return NULL;
}
/*************************************************************************************************************************************************************************/ 