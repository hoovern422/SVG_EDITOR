/*******************************************************
 * Name: Nickolas Hoover
 * Student number: 1024486
 * email: hoovern@uoguelph.ca
 * *****************************************************
 * This code was created using the sample file: tree1.c
 * Author: author: Dodji Seketeli
 * url : http://www.xmlsoft.org/examples/tree1.c
 * type: source code
 *******************************************************/

#include "SVGParser.h"
#include "LinkedListAPI.h"
#include <math.h>
#include "header.h" 
//#include "a2.c"

/*int main(int argc, char **argv)         
{
    char string [1000];
    strcpy(string, "{\"title\":\"titleVal\",\"descr\":\"descrVal\"}");
    JSONtoSVG(string, "testFile.svg");
    SVGimage *image = createValidSVGimage("./testFile.svg", "./include/testFiles/svg.xsd");
    if (image == NULL) printf("error creating SVG file\n");
    else printf("image is valid");
    deleteSVGimage(image);
    Rectangle *rectangle = JSONtoRect("{\"x\":1,\"y\":2,\"w\":19,\"h\":15,\"units\":\"cm\"}");
    printf("%s\n", rectangleToString(rectangle));
    Circle *circle = JSONtoCircle("{\"cx\":32,\"cy\":32,\"r\":30,\"units\":\"\"}");
    printf("%s\n", circleToString(circle));
    return 0;
}*/


int buildImage(xmlNode *a_node, SVGimage *image)
{
    int check = 0;
    if (image == NULL)
        check = -1;
    if (a_node == NULL)
        check = -1;
    xmlNode *cur_node = NULL;
    xmlNode *temp_node = NULL;

    Rectangle **rects = calloc(1, sizeof(Rectangle *));
    Circle **circs = calloc(1, sizeof(Circle *));
    Path **paths = calloc(1, sizeof(Path *));
    Group **groups = calloc(1, sizeof(Group *));
    Attribute **attributes = calloc(1, sizeof(Attribute *));
    int rectangleCounter = 0;
    int circleCounter = 0;
    int pathCounter = 0;
    int groupCounter = 0;
    int groupInGroupCounter = 0;
    int attCounter = 0;
    List *lptr;
    int i = 0;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if (i == 0)
            {
                if (cur_node->ns == NULL)
                {
                    check = -1;
                    break;
                }
                else if (cur_node->ns->href == NULL || strcmp((char *)cur_node->ns->href, "\0") == 0)
                {
                    check = -1;
                    break;
                }
                strncpy(image->namespace, (char *)cur_node->ns->href, 255);
            }

            if (strcmp((char *)cur_node->name, "svg") != 0 && strcmp((char *)cur_node->name, "g") == 0 && strcmp((char *)cur_node->parent->name, "g") == 0)
            {
                int i = 0;
                int j = 0;
                groupInGroupCounter = 0;
                groupCounter = 0;
                groupInGroupCounter++;
                temp_node = cur_node->parent;
                ListIterator **itr = calloc(1, sizeof(ListIterator *));
                groupCounter++;
                groups[groupCounter - 1] = createGroup(cur_node);
                if (groups[groupCounter - 1] == NULL)
                {
                    check = -1;
                    break;
                }

                while (strcmp((char *)temp_node->parent->name, "g") == 0)
                {
                    groupInGroupCounter++;
                    temp_node = temp_node->parent;
                }

                itr[0] = calloc(1, sizeof(ListIterator));
                *itr[0] = createIterator(image->groups);

                while (itr[0]->current->next != NULL)
                {
                    itr[0]->current = itr[0]->current->next;
                }

                lptr = (List *)(((Group *)itr[0]->current->data)->groups);
                itr = realloc(itr, (i + 2) * sizeof(ListIterator *));
  
                for (i = 1; i < groupInGroupCounter; i++)
                {
                    itr[i] = calloc(1, sizeof(ListIterator));
                    *itr[i] = createIterator(lptr);
                    while (itr[i]->current->next != NULL)
                    {
                        itr[i]->current = itr[i]->current->next;
                    }
                    lptr = (List *)(((Group *)itr[i]->current->data)->groups);
                    itr = realloc(itr, (i + 2) * sizeof(ListIterator *));
                }

                while (itr[i - 1]->current->next != NULL)
                {
                    itr[i - 1]->current = itr[i - 1]->current->next;
                }

                lptr = (List *)(((Group *)itr[i - 1]->current->data)->groups);
                insertBack(lptr, groups[groupCounter - 1]);

                for (j = 0; j < i; j++)
                {
                    free(itr[j]);
                }
                free(itr);
            }

            else if (strcmp((char *)cur_node->name, "svg") != 0 && strcmp((char *)cur_node->parent->name, "g") == 0 && strcmp((char *)cur_node->name, "g") != 0)
            {
                int i = 0;
                int j = 0;
                groupInGroupCounter = 0;
                rectangleCounter = 0;
                circleCounter = 0;
                pathCounter = 0;
                groupInGroupCounter++;
                temp_node = cur_node->parent;
                ListIterator **itr = calloc(1, sizeof(ListIterator *));

                while (strcmp((char *)temp_node->parent->name, "g") == 0)
                {
                    groupInGroupCounter++;
                    temp_node = temp_node->parent;
                }

                itr[0] = calloc(1, sizeof(ListIterator));
                *itr[0] = createIterator(image->groups);

                while (itr[0]->current->next != NULL)
                {
                    itr[0]->current = itr[0]->current->next;
                }

                lptr = (List *)(((Group *)itr[0]->current->data)->groups);
                itr = realloc(itr, (i + 2) * sizeof(ListIterator *));

                for (i = 1; i < groupInGroupCounter; i++)
                {
                    itr[i] = calloc(1, sizeof(ListIterator));
                    *itr[i] = createIterator(lptr);
                    while (itr[i]->current->next != NULL)
                    {
                        itr[i]->current = itr[i]->current->next;
                    }
                    lptr = (List *)(((Group *)itr[i]->current->data)->groups);
                    itr = realloc(itr, (i + 2) * sizeof(ListIterator *));
                }

                while (itr[i - 1]->current->next != NULL)
                {
                    itr[i - 1]->current = itr[i - 1]->current->next;
                }

                if (strcmp((char *)cur_node->name, "rect") == 0)
                {
                    rects[0] = createRectangle(cur_node);
                    lptr = (List *)(((Group *)itr[i - 1]->current->data)->rectangles);
                    if (rects[0] != NULL)
                        insertBack(lptr, rects[0]);
                }
                if (strcmp((char *)cur_node->name, "circle") == 0)
                {
                    circs[0] = createCircle(cur_node);
                    lptr = (List *)(((Group *)itr[i - 1]->current->data)->circles);
                    if (circs[0] != NULL)
                        insertBack(lptr, circs[0]);
                }
                if (strcmp((char *)cur_node->name, "path") == 0)
                {
                    paths[0] = createPath(cur_node);
                    lptr = (List *)(((Group *)itr[i - 1]->current->data)->paths);
                    if (paths[0] != NULL)
                        insertBack(lptr, paths[0]);
                }
                for (j = 0; j < i; j++)
                {
                    free(itr[j]);
                }

                free(itr);
            }

            else if (strcmp((char *)cur_node->name, "title") == 0)
            {
                if (cur_node->children != NULL)
                {
                    //if (cur_node->children->name != NULL) printf("child name is %s\n", (char*)cur_node->children->name);
                    if (cur_node->children->content != NULL)
                        strncpy(image->title, (char *)cur_node->children->content, 255);
                }
            }
            else if (strcmp((char *)cur_node->name, "desc") == 0)
            {
                if (cur_node->children != NULL)
                {
                    if (cur_node->children->content != NULL)
                        strncpy(image->description, (char *)cur_node->children->content, 255);
                }
            }
            else if (strcmp((char *)cur_node->name, "rect") == 0)
            {
                rectangleCounter = 0;
                if (rectangleCounter > 0)
                    rects = realloc(rects, (rectangleCounter + 1) * sizeof(Rectangle *));
                rectangleCounter++;
                rects[rectangleCounter - 1] = createRectangle(cur_node);
                if (rects[rectangleCounter - 1] != NULL)
                    insertBack(image->rectangles, rects[rectangleCounter - 1]);
            }
            else if (strcmp((char *)cur_node->name, "circle") == 0)
            {
                circleCounter = 0;
                if (circleCounter > 0)
                    circs = realloc(circs, (circleCounter + 1) * sizeof(Circle *));
                circleCounter++;
                circs[circleCounter - 1] = createCircle(cur_node);
                if (circs[circleCounter - 1] != NULL)
                    insertBack(image->circles, circs[circleCounter - 1]);
            }
            else if (strcmp((char *)cur_node->name, "path") == 0)
            {
                pathCounter = 0;
                if (pathCounter > 0)
                    paths = realloc(paths, (pathCounter + 1) * sizeof(Path *));
                pathCounter++; 
                paths[pathCounter - 1] = createPath(cur_node);
                if (paths[pathCounter - 1] != NULL)
                    insertBack(image->paths, paths[pathCounter - 1]);
            }
            else if (strcmp((char *)cur_node->name, "g") == 0)
            {
                groupCounter = 0;
                if (groupCounter > 0)
                    groups = realloc(groups, (groupCounter + 1) * sizeof(Group *));
                groupCounter++;
                groups[groupCounter - 1] = createGroup(cur_node);
                if (groups[groupCounter - 1] != NULL)
                    insertBack(image->groups, groups[groupCounter - 1]);
            }

            xmlAttr *attr; 
            attCounter = 0;
            for (attr = cur_node->properties; attr != NULL; attr = attr->next)
            {
                xmlNode *value = attr->children;
                char *attrName = (char *)attr->name;
                char *cont = (char *)(value->content);
                if (strcmp((char *)cur_node->name, "svg") == 0)
                { 
                    if (attCounter != 0)
                        attributes = realloc(attributes, (attCounter + 1) * sizeof(Attribute *));
                    attributes[attCounter] = calloc(1, sizeof(Attribute));
                    if (attrName != NULL)
                        attributes[attCounter]->name = calloc(strlen(attrName) + 1, sizeof(char));
                    else
                        attributes[attCounter]->name = calloc(1, sizeof(char));
                    if (attrName != NULL)
                        strcpy(attributes[attCounter]->name, attrName);
                    else
                        strcpy(attributes[attCounter]->name, "\0");
                    if (cont != NULL)
                        attributes[attCounter]->value = calloc(strlen(cont) + 1, sizeof(char));
                    else
                        attributes[attCounter]->value = calloc(1, sizeof(char));
                    if (cont != NULL)
                        strcpy(attributes[attCounter]->value, cont);
                    else
                        strcpy(attributes[attCounter]->value, "\0");
                    if (attributes[attCounter] != NULL)
                        insertBack(image->otherAttributes, attributes[attCounter]);
                    attCounter++;
                }
            }
 
            i++;
            buildImage(cur_node->children, image);
        } 
    }
    free(rects);
    free(circs);
    free(paths);
    free(groups);
    free(attributes);
    return check;
}
   
SVGimage *createSVGimage(char *fileName)
{    
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    int check;

    if (fileName == NULL || strcmp(fileName, "\0") == 0)
    {
        return NULL;  
    }

    doc = xmlReadFile(fileName, NULL, 0);
    if (doc == NULL)
    {
        xmlFreeDoc(doc);
        return NULL;
    }
    
    root_element = xmlDocGetRootElement(doc);

    SVGimage *image = calloc(sizeof(SVGimage), 1);
    image->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles); 
    image->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    image->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
    image->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    check = buildImage(root_element, image);
    if (check != 0)
    {
        deleteSVGimage(image);
        xmlFreeDoc(doc);
        return NULL;
    }

    xmlFreeDoc(doc);

    return image;
}
    
void deleteSVGimage(SVGimage *img)
{
    if (img == NULL)
        return;
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->paths);
    freeList(img->groups);
    freeList(img->otherAttributes);
    free(img);
    img = NULL;
    return;
}

char *SVGimageToString(SVGimage *img)
{
    if (img == NULL)
        return NULL;
    char *svgDescription = calloc(1000000, sizeof(char));
    int mem = 1000000;
    char *buffer;
    int i = 0;
    char counter[200];

    strcpy(svgDescription, "\nSVG NODE DESCRIPTION\n");
    strcat(svgDescription, "*********************************************************************************************************************\n");
    strcat(svgDescription, "namespace: ");
    strcat(svgDescription, img->namespace);
    strcat(svgDescription, "\n");
    if (strlen(img->title) > 0)
    {
        strcat(svgDescription, "title: ");
        strcat(svgDescription, img->title);
        strcat(svgDescription, "\n");
    }
    if (strlen(img->description))
    {
        strcat(svgDescription, "description: ");
        strcat(svgDescription, img->description);
        strcat(svgDescription, "\n");
    }

    if (getLength(img->rectangles) > 0)
        strcat(svgDescription, "\nRectangle in svg: \n");
    ListIterator itr1 = createIterator(img->rectangles);
    while (itr1.current != NULL)
    {
        buffer = rectangleToString(itr1.current->data);
        if (strlen(buffer) + strlen(svgDescription) + 2 >= mem)
        {
            mem = mem * 2;
            svgDescription = realloc(svgDescription, mem * sizeof(char));
        }
        strcat(svgDescription, buffer);
        if (buffer != NULL){
            free(buffer); 
            buffer = NULL;
        }
        itr1.current = itr1.current->next;
    }

    if (getLength(img->circles) > 0)
        strcat(svgDescription, "\n\nCircle in svg: \n");
    ListIterator itr2 = createIterator(img->circles);
    while (itr2.current != NULL)
    {
        buffer = circleToString(itr2.current->data);
        if (strlen(buffer) + strlen(svgDescription) + 2 >= mem)
        {
            mem = mem * 2;
            svgDescription = realloc(svgDescription, mem * sizeof(char));
        }
        strcat(svgDescription, buffer);
        if (buffer != NULL){
            free(buffer);   
            buffer = NULL;
        } 
        itr2.current = itr2.current->next;
    }

    if (getLength(img->paths) > 0)
        strcat(svgDescription, "\nPath in svg: \n");
    ListIterator itr3 = createIterator(img->paths);
    while (itr3.current != NULL)
    {
        buffer = pathToString(itr3.current->data);
        if (strlen(buffer) + strlen(svgDescription) + 2 >= mem)
        {
            mem = mem * 2;
            svgDescription = realloc(svgDescription, mem * sizeof(char));
        }
        strcat(svgDescription, buffer);
        if (buffer != NULL){
            free(buffer); 
            buffer = NULL;
        }
        itr3.current = itr3.current->next;
    }

    if (getLength(img->otherAttributes) > 0)
        strcat(svgDescription, "\nOther Attributes in svg:\n");
    ListIterator itr = createIterator(img->otherAttributes);
    while (itr.current != NULL)
    {
        buffer = attributeToString(itr.current->data);
        if (strlen(buffer) + strlen(svgDescription) + 2 >= mem)
        {
            mem = mem * 2;
            svgDescription = realloc(svgDescription, mem * sizeof(char));
        }
        strcat(svgDescription, buffer);
        if (buffer != NULL){
            free(buffer); 
            buffer = NULL;
        }
        itr.current = itr.current->next;
    }

    strcat(svgDescription, "\nSVG GROUPS DESCRIPTION\n");
    strcat(svgDescription, "*********************************************************************************************************************\n");
    ListIterator itr4 = createIterator(img->groups);
    while (itr4.current != NULL)
    {
        if (i > 0)
        {
            if (strlen(buffer) + strlen(svgDescription) + 2 >= mem)
            {
                mem = mem * 2;
                svgDescription = realloc(svgDescription, mem * sizeof(char));
            }
        }
        if (i > 0){
            if (buffer!= NULL){
                free(buffer);
                buffer = NULL;
            }
        }
        strcat(svgDescription, "\nGROUP #");
        sprintf(counter, "%d", i + 1);
        strcat(svgDescription, counter);
        strcat(svgDescription, " IN SVG:\n");
        buffer = groupToString(itr4.current->data);
        strcat(svgDescription, buffer);
        itr4.current = itr4.current->next;
        i++;
    }

    strcat(svgDescription, "\0");
    if (buffer != NULL) free(buffer);
    return svgDescription;
}

void deleteRectangle(void *data)
{
    if (data == NULL)
        return;
    Rectangle *rectangle = (Rectangle *)data;
    freeList(rectangle->otherAttributes);
    free(rectangle);
    return;
}

/*#######################compare function stubs#################*/
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
/*############################################################*/

char *rectangleToString(void *data)
{
    if (data == NULL)
        return NULL;
    Rectangle *rectangle = (Rectangle *)data;
    char *description = calloc(1000000, sizeof(char));
    char buffer[100];
    int mem = 1000000;

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

Rectangle *createRectangle(xmlNode *node)
{
    if (node == NULL)
        return NULL;
    Rectangle *rect = calloc(1, sizeof(Rectangle));
    xmlAttr *attr;
    xmlNode *value;
    Attribute **attributes = calloc(1, sizeof(Attribute *));
    char *attrName;
    char *cont;
    int i = 0; 
    int units = 0;
    float test = 0.0;
    char *pend;
    rect->x = 0.0;
    rect->y = 0.0;
    rect->height = 0.0;
    rect->width = 0.0;
    rect->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    for (attr = node->properties; attr != NULL; attr = attr->next)
    {
        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);

        test = strtof(cont, &pend);
        if (strcmp(pend, "\0") != 0 && pend != NULL && units == 0)
        {
            if (strcmp(pend, "cm") == 0 || strcmp(pend, "em") == 0 || strcmp(pend, "px") == 0 || strcmp(pend, "pt") == 0 || strcmp(pend, "pc") == 0 ||
                strcmp(pend, "ex") == 0 || strcmp(pend, "mm") == 0 || strcmp(pend, "in") == 0)
            {
                strncpy(rect->units, pend, 50);
                units++;
            }
        }

        if (strcmp(attrName, "x") == 0)
        {
            sscanf(cont, "%f", &rect->x);
        }
        else if (strcmp(attrName, "y") == 0)
        {
            sscanf(cont, "%f", &rect->y);
        }
        else if (strcmp(attrName, "width") == 0)
        {
            sscanf(cont, "%f", &test);
            if (test >= 0)
            {
                sscanf(cont, "%f", &rect->width);
            }
            else
            {
                free(rect);
                freeList(rect->otherAttributes);
                free(attributes);
                return NULL;
            }
        }
        else if (strcmp(attrName, "height") == 0)
        {
            sscanf(cont, "%f", &test);
            if (test >= 0)
            {
                sscanf(cont, "%f", &rect->height);
            }
            else
            {
                free(rect);
                free(attributes);
                freeList(rect->otherAttributes);
                return NULL;
            }
        }
        else
        {
            if (i != 0)
                attributes = realloc(attributes, (i + 1) * sizeof(Attribute *));
            attributes[i] = calloc(1, sizeof(Attribute));
            if (attrName != NULL)
                attributes[i]->name = calloc(strlen(attrName) + 1, sizeof(char));
            else
                attributes[i]->name = calloc(1, sizeof(char));
            if (attrName != NULL)
                strcpy(attributes[i]->name, attrName);
            else
                strcpy(attributes[i]->name, "\0");
            if (cont != NULL)
                attributes[i]->value = calloc(strlen(cont) + 1, sizeof(char));
            else
                attributes[i]->value = calloc(1, sizeof(char));
            if (cont != NULL)
                strcpy(attributes[i]->value, cont);
            else
                strcpy(attributes[i]->value, "\0");
            if (attributes[i] != NULL)
                insertBack(rect->otherAttributes, attributes[i]);
            i++;
        }
    }

    free(attributes);

    return rect;
}

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

Circle *createCircle(xmlNode *curNode)
{
    if (curNode == NULL)
        return NULL;
    Circle *circ = calloc(1, sizeof(Circle));
    xmlAttr *attr;
    xmlNode *value;
    Attribute **attributes = calloc(1, sizeof(Attribute *));

    char *attrName;
    char *cont;
    char *pend;
    int i = 0;
    int units = 0;
    float test = 0.0;

    circ->cx = 0.0;
    circ->cy = 0.0;
    circ->r = 0.0;
    circ->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

    for (attr = curNode->properties; attr != NULL; attr = attr->next)
    {
        value = attr->children;
        attrName = (char *)(attr->name);
        cont = (char *)(value->content);

        test = strtof(cont, &pend);
        if (strcmp(pend, "\0") != 0 && pend != NULL && units == 0)
        {
            if (strcmp(pend, "cm") == 0 || strcmp(pend, "em") == 0 ||
                strcmp(pend, "px") == 0 || strcmp(pend, "pt") == 0 || 
                strcmp(pend, "pc") == 0 || strcmp(pend, "ex") == 0 ||
                strcmp(pend, "mm") == 0 || strcmp(pend, "in") == 0)
            {
                strncpy(circ->units, pend, 50);
                units++;
            }
        }

        if (strcmp(attrName, "cx") == 0)
        {
            sscanf(cont, "%f", &circ->cx);
        }
        else if (strcmp(attrName, "cy") == 0)
        {
            sscanf(cont, "%f", &circ->cy);
        }
        else if (strcmp(attrName, "r") == 0)
        {
            sscanf(cont, "%f", &test);
            if (test >= 0)
            {
                sscanf(cont, "%f", &circ->r);
            }
            else
            {
                freeList(circ->otherAttributes);
                free(circ);
                free(attributes);
                return NULL;
            }
        }

        else
        {
            if (i != 0){
                attributes = realloc(attributes, (i + 2) * sizeof(Attribute *));
            }

            attributes[i] = calloc(1, sizeof(Attribute));

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

            if (attributes[i] != NULL)
                insertBack(circ->otherAttributes, attributes[i]);
            i++;
        }
    }

    free(attributes);

    return circ;
}

void deleteCircle(void *data)
{
    if (data == NULL)
        return;
    Circle *circle = (Circle *)data;
    freeList(circle->otherAttributes);
    free(circle);
    return;
}

char *circleToString(void *data)
{
    if (data == NULL)
        return NULL;
    Circle *circle = (Circle *)data;
    char *description = calloc(1000000, sizeof(char));
    char buffer[100];
    int mem = 1000000;

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

Path *createPath(xmlNode *node)
{
    if (node == NULL)
        return NULL;
    Path *path = calloc(1, sizeof(Path));
    xmlAttr *attr;
    xmlNode *value;
    Attribute **attributes = calloc(1, sizeof(Attribute *));
    char *attrName;
    char *cont;
    int i = 0;
    path->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    path->data = NULL;
    for (attr = node->properties; attr != NULL; attr = attr->next)
    {
        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);
        if (strcmp(attrName, "d") == 0)
        {
            if (attr == NULL || attrName == NULL || cont == NULL)
            {
                deletePath(path);
                free(attributes);
                return NULL;
            }
            path->data = calloc((strlen(cont) + 1), sizeof(char));
            strcpy(path->data, cont);
        }
        else
        {
            if (i != 0)
                attributes = realloc(attributes, (i + 1) * sizeof(Attribute *));
            attributes[i] = calloc(1, sizeof(Attribute));
            if (attrName != NULL)
                attributes[i]->name = calloc(strlen(attrName) + 1, sizeof(char));
            else
                attributes[i]->name = calloc(1, sizeof(char));
            if (attrName != NULL)
                strcpy(attributes[i]->name, attrName);
            else
                strcpy(attributes[i]->name, "\0");
            if (cont != NULL)
                attributes[i]->value = calloc(strlen(cont) + 1, sizeof(char));
            else
                attributes[i]->value = calloc(1, sizeof(char));
            if (cont != NULL)
                strcpy(attributes[i]->value, cont);
            else
                strcpy(attributes[i]->value, "\0");
            if (attributes[i] != NULL)
                insertBack(path->otherAttributes, attributes[i]);
            i++;
        }
    }

    free(attributes);
    if (path->data == NULL)
    {
        deletePath(path);
        return NULL;
    }
    return path;
}

char *pathToString(void *data)
{
    if (data == NULL)
        return NULL;
    Path *path = (Path *)data;
    char *description;
    if (path->data == NULL)
        return NULL;
    description = calloc(1000000, sizeof(char));
    int mem = 1000000;
    strcpy(description, "\nPath:\n");
    strcat(description, "\nAttribute name: data\nValue: ");
    strcat(description, path->data);
    strcat(description, "\n");
    ListIterator itr = createIterator(path->otherAttributes);
    char *string;
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

Group *createGroup(xmlNode *curNode){

    if (curNode == NULL)
        return NULL;

    Group *group = calloc(1, sizeof(Group));
    xmlAttr *attr;
    xmlNode *value;
    char *attrName;
    char *cont;
    int i = 0;
    group->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    group->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    group->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
    group->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    group->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
    Attribute **attributes = calloc(1, sizeof(Attribute *));
    for (attr = curNode->properties; attr != NULL; attr = attr->next)
    {
        value = attr->children;
        attrName = (char *)attr->name;
        cont = (char *)(value->content);
        if (i != 0)
            attributes = realloc(attributes, (i + 1) * sizeof(Attribute *));
        attributes[i] = calloc(1, sizeof(Attribute));
        if (attrName != NULL)
            attributes[i]->name = calloc(strlen(attrName) + 1, sizeof(char));
        else
            attributes[i]->name = calloc(1, sizeof(char));
        if (attrName != NULL)
            strcpy(attributes[i]->name, attrName);
        else
            strcpy(attributes[i]->name, "\0");
        if (cont != NULL)
            attributes[i]->value = calloc(strlen(cont) + 1, sizeof(char));
        else
            attributes[i]->value = calloc(1, sizeof(char));
        if (cont != NULL)
            strcpy(attributes[i]->value, cont);
        else
            strcpy(attributes[i]->value, "\0");
        if (attributes[i] != NULL)
            insertBack(group->otherAttributes, attributes[i]);
        i++;
    }
    free(attributes);
    return group;
}

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
        strcat(description, "*********************************************************************************************************************\n");
    }
    else
    {
        strcat(description, "*********************************************************************************************************************\n");
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

List *getRects(SVGimage *img)
{
    List *rectList = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    if (img == NULL)
        return rectList;
    ListIterator imageRectangleItr;
    ListIterator *headItr = headItr = calloc(1, sizeof(ListIterator));
    char *label = calloc(20, sizeof(char));
    strcpy(label, "rectangle");

    imageRectangleItr = createIterator(img->rectangles);
    while (imageRectangleItr.current != NULL)
    {
        insertBack(rectList, (Rectangle *)imageRectangleItr.current->data);
        imageRectangleItr.current = imageRectangleItr.current->next;
    }

    if (getLength(img->groups) > 0)
    {
        *headItr = createIterator(img->groups);
        while (headItr->current != NULL)
        {
            searchGroupsInGroups(headItr->current->data, rectList, label);
            headItr->current = headItr->current->next;
        }
    }

    free(headItr);
    free(label);

    return rectList;
}

List *getCircles(SVGimage *img)
{
    List *circleList = initializeList(&circleToString, &deleteCircle, &compareCircles);
    if (img == NULL)
        return circleList;
    ListIterator imageCircleItr;
    ListIterator *headItr = calloc(1, sizeof(ListIterator));
    char *label = calloc(20, sizeof(char));
    strcpy(label, "circle");

    imageCircleItr = createIterator(img->circles);
    while (imageCircleItr.current != NULL)
    {
        insertBack(circleList, (Circle *)imageCircleItr.current->data);
        imageCircleItr.current = imageCircleItr.current->next;
    }

    if (getLength(img->groups) > 0)
    {
        *headItr = createIterator(img->groups);
        while (headItr->current != NULL)
        {
            searchGroupsInGroups(headItr->current->data, circleList, label);
            headItr->current = headItr->current->next;
        }
    }

    free(headItr);
    free(label);

    return circleList;
}

List *getPaths(SVGimage *img)
{
    List *pathList = initializeList(&pathToString, &deletePath, &comparePaths);
    if (img == NULL)
        return pathList;
    ListIterator imagePathItr;
    ListIterator *headItr = calloc(1, sizeof(ListIterator));
    char *label = calloc(20, sizeof(char));
    strcpy(label, "path");

    imagePathItr = createIterator(img->paths);
    while (imagePathItr.current != NULL)
    {
        insertBack(pathList, (Path *)imagePathItr.current->data);
        imagePathItr.current = imagePathItr.current->next;
    }

    if (getLength(img->groups) > 0)
    {
        *headItr = createIterator(img->groups);
        while (headItr->current != NULL)
        {
            searchGroupsInGroups(headItr->current->data, pathList, label);
            headItr->current = headItr->current->next;
        }
    }

    free(headItr);
    free(label);

    return pathList;
}

List *getGroups(SVGimage *img)
{
    List *groupList = initializeList(&groupToString, &deleteGroup, &compareGroups);
    if (img == NULL)
        return groupList;
    ListIterator *headItr = calloc(1, sizeof(ListIterator));
    char *label = calloc(20, sizeof(char));
    strcpy(label, "group");

    if (getLength(img->groups) > 0)
    {
        *headItr = createIterator(img->groups);
        while (headItr->current != NULL)
        {
            searchGroupsInGroups(headItr->current->data, groupList, label);
            headItr->current = headItr->current->next;
        }
    }

    free(headItr);
    free(label);

    return groupList;
}

void searchRectanglesInGroup(Group *group, List *rectList)
{
    ListIterator rectItr;
    List *lptr;
    if (getLength((List *)(group->rectangles)) > 0)
    {
        lptr = (List *)(group->rectangles);
        rectItr = createIterator(lptr);
        while (rectItr.current != NULL)
        {
            insertBack(rectList, (Rectangle *)rectItr.current->data);
            rectItr.current = rectItr.current->next;
        }
    }
}

void searchCirclesInGroup(Group *group, List *circList)
{
    ListIterator circItr;
    List *lptr;
    if (getLength((List *)(group->circles)) > 0)
    {
        lptr = (List *)(group->circles);
        circItr = createIterator(lptr);
        while (circItr.current != NULL)
        {
            insertBack(circList, (Circle *)circItr.current->data);
            circItr.current = circItr.current->next;
        }
    }
}

void searchPathsInGroup(Group *group, List *pathList)
{
    ListIterator pathItr;
    List *lptr;
    if (getLength((List *)(group->paths)) > 0)
    {
        lptr = (List *)(group->paths);
        pathItr = createIterator(lptr);
        while (pathItr.current != NULL)
        {
            insertBack(pathList, (Path *)pathItr.current->data);
            pathItr.current = pathItr.current->next;
        }
    }
}

void searchGroupsInGroups(Group *parentGroup, List *list, char *label)
{
    int listLengths = 0;
    Group **groupsWithinGroups = calloc(1, sizeof(Group *));
    int length = 0;
    int i = 0;
    int j = 0;
    ListIterator itr;
    ListIterator *head = calloc(1, sizeof(ListIterator));

    if (strcmp(label, "rectangle") == 0)
        searchRectanglesInGroup(parentGroup, list);
    else if (strcmp(label, "circle") == 0)
        searchCirclesInGroup(parentGroup, list);
    else if (strcmp(label, "path") == 0)
        searchPathsInGroup(parentGroup, list);
    else if (strcmp(label, "group") == 0)
        insertBack(list, parentGroup);
    length = getLength(((List *)(parentGroup->groups)));
    if (length > 0)
    {
        itr = createIterator((List *)parentGroup->groups);
        while (itr.current != NULL)
        {
            listLengths = getLength(((List *)((Group *)(itr.current->data))->groups));
            if (listLengths > 0)
            {
                *head = createIterator(((List *)((Group *)(itr.current->data))->groups));
                while (head->current != NULL){
                   groupsWithinGroups[i] = (Group *)head->current->data;
                   i++;
                   groupsWithinGroups = realloc(groupsWithinGroups, (i + 1) * sizeof(Group *));
                   head->current = head->current->next;
                }
            }
            if (strcmp(label, "rectangle") == 0)
                searchRectanglesInGroup((Group *)(itr.current->data), list);
            else if (strcmp(label, "circle") == 0)
                searchCirclesInGroup((Group *)(itr.current->data), list);
            else if (strcmp(label, "path") == 0)
                searchPathsInGroup((Group *)(itr.current->data), list);
            else if (strcmp(label, "group") == 0)
                insertBack(list, (Group *)(itr.current->data));
            itr.current = itr.current->next;
        }
    }

    for (j = 0; j < i; j++)
    {
        searchGroupsInGroups(groupsWithinGroups[j], list, label);
    }
    free(groupsWithinGroups);
    free(head);
}

void freeShapesList(List *shapes)
{
    if (shapes == NULL)
        return;
    ListIterator itr1, itr2;
    itr1 = createIterator(shapes);
    itr2 = createIterator(shapes);
    while (itr1.current != NULL)
    {
        itr2.current = itr1.current;
        itr1.current = itr1.current->next;
        free(itr2.current);
    }
    free(shapes);
}

int numRectsWithArea(SVGimage *img, float area)
{
    if (img == NULL)
        return 0;
    if (area < 0)
        return 0;
    List *rectangles = getRects(img);
    Rectangle *rectangle;
    int intArea = ceil(area);
    int calcArea = 0;
    ListIterator itr = createIterator(rectangles);
    int i = 0;
    while (itr.current != NULL)
    {
        rectangle = (Rectangle *)itr.current->data;
        if (rectangle->height != 0 && rectangle->width != 0)
        {
            calcArea = ceil(rectangle->height * rectangle->width);
            if (calcArea == intArea)
                i++;
        }
        itr.current = itr.current->next;
    }
    freeShapesList(rectangles);
    return i;
}

int numCirclesWithArea(SVGimage *img, float area)
{
    if (img == NULL)
        return 0;
    if (area < 0)
        return 0;
    List *circles = getCircles(img);
    Circle *circle;
    int intArea = ceil(area);
    int calcArea = 0;
    ListIterator itr = createIterator(circles);
    int i = 0;
    while (itr.current != NULL)
    {
        circle = (Circle *)itr.current->data;
        if (circle->r > 0)
        {
            calcArea = ceil(3.14159265358979 * (circle->r) * circle->r);
            if (calcArea == intArea)
                i++;
        }
        itr.current = itr.current->next;
    }
    freeShapesList(circles);
    return i;
}

int numPathsWithdata(SVGimage *img, char *data)
{
    if (img == NULL)
        return 0;
    if (data == NULL)
        return 0;
    List *paths = getPaths(img);
    Path *path;
    ListIterator itr = createIterator(paths);
    int i = 0;
    while (itr.current != NULL)
    {
        path = (Path *)itr.current->data;
        if (strcmp(data, path->data) == 0)
        {
            i++;
        }
        itr.current = itr.current->next;
    }
    freeShapesList(paths);
    return i;
}

int numGroupsWithLen(SVGimage *img, int len)
{
    if (img == NULL)
        return 0;
    if (len < 0)
        return 0;
    List *groups = getGroups(img);
    Group *group;
    ListIterator itr = createIterator(groups);
    int i = 0;
    int calcLength = 0;
    while (itr.current != NULL)
    {
        group = (Group *)itr.current->data;
        calcLength = getLength(group->rectangles) + getLength(group->circles) + getLength(group->paths) + getLength(group->groups);
        if (calcLength == len)
            i++;
        itr.current = itr.current->next;
    }
    freeShapesList(groups);
    return i;
}

int numAttr(SVGimage *img)
{
    if (img == NULL)
        return 0;
    List *groups = getGroups(img);
    List *rectangles = getRects(img);
    List *circles = getCircles(img);
    List *paths = getPaths(img);
    Group *group;
    Rectangle *rectangle;
    Circle *circle;
    Path *path;
    ListIterator itr1 = createIterator(groups);
    ListIterator itr2 = createIterator(rectangles);
    ListIterator itr3 = createIterator(circles); 
    ListIterator itr4 = createIterator(paths);
    int i = 0;
    while (itr1.current != NULL)
    {
        group = (Group *)itr1.current->data;
        i += getLength(group->otherAttributes);
        itr1.current = itr1.current->next;
    }

    while (itr2.current != NULL)
    {
        rectangle = (Rectangle *)itr2.current->data;
        i += getLength(rectangle->otherAttributes);
        itr2.current = itr2.current->next;
    }

    while (itr3.current != NULL)
    {
        circle = (Circle *)itr3.current->data;
        i += getLength(circle->otherAttributes);
        itr3.current = itr3.current->next;
    }

    while (itr4.current != NULL)
    {
        path = (Path *)itr4.current->data;
        i += getLength(path->otherAttributes);
        itr4.current = itr4.current->next;
    }
    i += getLength(img->otherAttributes);

    freeShapesList(groups);
    freeShapesList(rectangles);
    freeShapesList(circles);
    freeShapesList(paths);
    return i;
}

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
void JSONtoSVG(const char* svgString, char *name){
    
    if (svgString == NULL) return;
    char *title = calloc(256, sizeof(char));
    char *description = calloc(256, sizeof(char));
    char *string = (char *) svgString;
    int i = 0, j = 0;
    //parse the JSON string
    while (j < 3 && i < strlen(string)){
        if (string[i] == '\"') j ++;
        i ++;
    }
    j = 0;
    while (string[i] != '\"'){
        title[j] = string[i];
        i ++;
        j ++;
    }
    //printf("Title: %s\n", title);
    i = 0;
    j = 0;
    while (j < 7 && i < strlen(string)){
        if (string[i] == '\"') j ++;
        i ++;
    }
    j = 0;
    while (string[i] != '\"'){
        description[j] = string[i];
        i ++;
        j ++;
    }
    //printf("Description: %s\n", description);
    //allocate all essential features of an SVGimage
    SVGimage *image = calloc(1, sizeof(SVGimage));//free
    image->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
    image->circles = initializeList(&circleToString, &deleteCircle, &compareCircles); 
    image->paths = initializeList(&pathToString, &deletePath, &comparePaths);
    image->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
    image->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    strcpy(image->namespace, "http://www.w3.org/2000/svg");
    strcpy(image->title, title);
    strcpy(image->description, description);
    free(title);
    free(description);
    bool truth = validateSVGimage(image, "./include/testFiles/svg.xsd");
    if (!truth){
        deleteSVGimage(image);
        return;
    }
    truth = writeSVGimage(image, name);
    if (!truth){
        deleteSVGimage(image);
        return;
    }
    deleteSVGimage(image);
    return;
}

Rectangle* JSONtoRect(const char* svgString){
    if (svgString == NULL) return NULL;
    Rectangle *rectangle = calloc(1, sizeof(Rectangle));
    rectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    float x = 0.0;
    float y = 0.0;
    float width = 0.0;
    float height = 0.0;
    char *string = (char *) svgString;
    char *buffer = calloc(1000, sizeof(char));
    int i = 0;
    int j = 0;
    char *pend;
    while (string[i] != ':'){
        i ++;
    }
    i ++;
    while (string[i] != ','){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    x = strtof(buffer, &pend);
    //printf("the value of x is: %f\n", x);
    i +=5;
    j = 0;
    while (string[i] != ','){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    y = strtof(buffer, &pend);
    //printf("the value of y is: %f\n", y);
    i +=5;
    j = 0;
    while (string[i] != ','){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    width = strtof(buffer, &pend);
    //printf("the value of the width is: %f\n", width);
    i +=5;
    j = 0;
    while (string[i] != ','){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    height = strtof(buffer, &pend);
    //printf("the value of the height is: %f\n", height);
    i +=10;
    j = 0;
    while (string[i] != '\"'){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    if (j == 0){
        buffer[0] = ' ';
        buffer[1] = '\0';
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    strcpy(rectangle->units, buffer);
    rectangle->x = x;
    rectangle->y = y;
    rectangle->width = width;
    rectangle->height = height;
    free(buffer);
    return rectangle;
}
Circle* JSONtoCircle(const char* svgString){
    if (svgString == NULL) return NULL;
    Circle *circle = calloc(1, sizeof(Circle));
    circle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    float cx = 0.0;
    float cy = 0.0;
    float r = 0.0;
    char *string = (char *) svgString;
    char *buffer = calloc(1000, sizeof(char));
    int i = 0;
    int j = 0;
    char *pend;
    while (string[i] != ':'){
        i ++;
    }
    i ++;
    while (string[i] != ','){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    cx = strtof(buffer, &pend);
    //printf("the value of cx is: %f\n", cx);
    i +=6;
    j = 0;
    while (string[i] != ','){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    cy = strtof(buffer, &pend);
    //printf("the value of cy is: %f\n", cy);
    i +=5;
    j = 0;
    while (string[i] != ','){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
   // printf("\nthe buffer string is: %s\n", buffer);
    r = strtof(buffer, &pend);
    //printf("the value of r is: %f\n", r);
    i +=10;
    j = 0;
    while (string[i] != '\"'){
        buffer[j] = string[i];
        i ++;
        j ++;
    }
    if (j == 0){
        buffer[0] = ' ';
        buffer[1] = '\0';
    }
    //printf("\nthe buffer string is: %s\n", buffer);
    strcpy(circle->units, buffer);
    circle->cx = cx;
    circle->cy = cy;
    circle->r = r;
    free(buffer);
    return circle;
}
/*************************************************************************************************************************************************************************/ 
