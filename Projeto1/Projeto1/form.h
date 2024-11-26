#ifndef __FORM_H__
#define __FORM_H__

#define RECTANGLE 0
#define SQUARE 1
#define TRIANGLE_ISO 2
#define LINE 3
#define TRIANGLE_EQUI 4
#define HEXAGON 5
#define CIRCUMFERENCE 6
#define STAR 7

/*
*  This interface provides clients with handles to form objects,
*  but does not give any information about the internal representation.
*  (The struct is specified only by its tag name.)
*/


// The tag name "Form" represents a pointer to a Graphical Form object
typedef struct form* Form;

Form newForm(int type, float x, float y, float xSize, float ySize);
Form newRectangle(float x, float y, float xSize, float ySize);
Form newSquare(float x, float y, float side);
Form newTriangleIso(float x, float y, float xSize, float ySize);
Form newLine(float x, float y, float xSide, float ySize);
Form newTriangleEqui(float x, float y, float xSide, float ySize);
Form newHexagon(float x, float y, float xSide, float ySize);
Form newCircumference(float x, float y, float xSide, float ySize);

Form newRectangle2Point(float xi, float yi, float xf, float yf);

Form createRandomForm(int maxSize);

// check if XY is inside form
int pick(Form f, float x, float y);

// delete form
void deleteForm(Form f);


// update a form position - moving by (dx,dy)
void updateForm(Form f, float dx, float dy);
void change2Point(Form f, float x, float y);

void setBackgroundColor(Form f, float r, float g, float b);
void getBGColor(Form f, float* c);
void getBGLineColor(Form f, float* c);
void setLineColor(Form f, float lr, float lg, float lb);

// print the content of the graphical form to console
void printfForm(Form f);

// draw a graphical form
void drawForm(Form f);
void updateFormY(Form f, float y);

// apanha o tipo da forma
int getFormType(Form f);

#endif

