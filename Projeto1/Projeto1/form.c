#include "form.h"
#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

#ifndef  M_PI
#define  M_PI  3.1415926535897932384626433
#endif


// this struct is encapsulated
struct form {
    int type;
    float x, y;             // initial point of the form
    float xSize, ySize;     // sides
    float r, g, b;
    float lr, lg, lb;
};

Form newForm(int type, float x, float y, float xSize, float ySize) {
    Form f = (Form)malloc(sizeof(struct form));

    f->x = x;
    f->y = y;
    f->xSize = xSize;
    f->ySize = ySize;

    f->r = 1.0;
    f->g = 1.0;
    f->b = 1.0;

    f->lr = 0.0;
    f->lg = 0.0;
    f->lb = 0.0;

    f->type = type;

    return f;
}

Form newRectangle(float x, float y, float xSize, float ySize) {
    return newForm(RECTANGLE, x, y, xSize, ySize);
}

Form newRectangle2Point(float xi, float yi, float xf, float yf) {
    return newForm(RECTANGLE, xi, yi, xf - xi, yf - yi);
}

Form newSquare(float x, float y, float side) {
    return newForm(SQUARE, x, y, side, side);
}

Form newTriangleIso(float x, float y, float xSize, float ySize) {
    return newForm(TRIANGLE_ISO, x, y, xSize, ySize);
}

Form newLine(float x, float y, float xSide, float ySize) {
    return newForm(LINE, x, y, xSide, ySize);
}

Form newTriangleEqui(float x, float y, float xSize, float ySize) {
    return newForm(TRIANGLE_EQUI, x, y, xSize, ySize);
}

//Foi modificado para ficar centrado ao criar a figura
Form newHexagon(float x, float y, float xSide, float ySize) {
    return newForm(HEXAGON, x - (xSide / 2), y - (ySize / 2), xSide, ySize);
}

//Foi modificado para ficar centrado ao criar a figura
Form newCircumference(float x, float y, float xSide, float ySize) {
    return newForm(CIRCUMFERENCE, x - (xSide / 2), y - (ySize / 2), xSide, ySize);
}

int pick(Form f, float x, float y) {
    float menorX = f->x;
    float maiorX = f->x + f->xSize;
    float menorY = f->y;
    float maiorY = f->y + f->ySize;

    if (f->x > f->x + f->xSize) {
        menorX = f->x + f->xSize;
        maiorX = f->x;
    }

    if (f->y > f->y + f->ySize) {
        menorY = f->y + f->ySize;
        maiorY = f->y;
    }

    return x >= menorX && x <= maiorX &&
           y >= menorY && y <= maiorY;
}

int getFormType(Form f) {
    return f->type;
}

Form createRandomForm(int maxSize) {
    Form f;
    switch (rand() % 7) {
        case 0:
            f = newRectangle(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4), rand() % (maxSize / 4));
            break;
        case 1:
            f = newSquare(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4));
            break;
        case 2:
            f = newTriangleIso(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4), rand() % (maxSize / 4));
            break;
        case 3:
            f = newLine(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4), rand() % (maxSize / 4));
            break;
        case 4:
            f = newTriangleEqui(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4), rand() % (maxSize / 4));
            break;
        case 5:
            f = newHexagon(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4), rand() % (maxSize / 4));
            break;
        case 6:
            f = newCircumference(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4), rand() % (maxSize / 4));
            break;

        default:
            f = newRectangle(rand() % maxSize, rand() % maxSize, rand() % (maxSize / 4), rand() % (maxSize / 4));
            break;
    }

    return f;
}

void deleteForm(Form f) {
    free(f);
}

void updateForm(Form f, float dx, float dy) {
    f->x += dx;
    f->y += dy;
}


void change2Point(Form f, float x, float y) {
    if (f->type == SQUARE) {
        float size;
        float dx = x - f->x;
        float dy = y - f->y;

        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        if (dx > dy) {
            size = dx;
        } else {
            size = dy;
        }
        if (x < f->x) {
            f->xSize = -size;
        } else {
            f->xSize = size;
        }
        if (y < f->y) {
            f->ySize = -size;
        } else {
            f->ySize = size;
        }
    } else {
        f->xSize = x - f->x;
        f->ySize = y - f->y;
    }
}

void setBackgroundColor(Form f, float r, float g, float b) {
    f->r = r;
    f->g = g;
    f->b = b;
}

void getBGColor(Form f, float* c) {
    c[0] = f->r;
    c[1] = f->g;
    c[2] = f->b;
}

void getBGLineColor(Form f, float* c) {
    c[0] = f->lr;
    c[1] = f->lg;
    c[2] = f->lb;
}

void setLineColor(Form f, float lr, float lg, float lb) {
    f->lr = lr;
    f->lg = lg;
    f->lb = lb;
}

void updateFormY(Form f, float y) {
    f->y = y;
}

void printfForm(Form f) {
    printf("Graphical Form BBox: (%.2f,%.2f), (%.2f, %.2f), (%.2f, %.2f), (%.2f, %.2f)\t",
           f->x, f->y,
           f->x, f->y + f->ySize,
           f->x + f->xSize, f->y + f->ySize,
           f->x + f->xSize, f->y);


    printf("\nCenter(%.2f,%.2f) Size{%.2f,%.2f}\n",
           f->x + (f->xSize / 2), f->y + (f->ySize / 2),
           f->xSize, f->ySize);

}

void drawTriangleIso(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + (f->xSize / 2.0), f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glColor3f(f->lr, f->lg, f->lb);
    glBegin(GL_LINE_LOOP);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + (f->xSize / 2.0), f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
}

void drawLine(Form f) {
    //glColor3f(0.0, 0.0, 0.0); // black
    glColor3f(f->lr, f->lg, f->lb); //para mudar o contorno da linha/cor
    glBegin(GL_LINES);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glEnd();
}

void drawTriangleEqui(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + (sqrt(3) / 2) * f->xSize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glColor3f(f->lr, f->lg, f->lb);
    glBegin(GL_LINE_LOOP);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x + f->xSize / 2, f->y + (sqrt(3) / 2) * f->xSize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
}

void drawHexagon(Form f) {
    float centerX = f->x + f->xSize / 2;
    float centerY = f->y + f->ySize / 2;
    float radius = f->xSize / 2;

    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        glVertex2f(centerX + radius * cos(i * M_PI / 3), centerY + radius * sin(i * M_PI / 3));
    }
    glEnd();

    glColor3f(f->lr, f->lg, f->lb);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 6; ++i) {
        glVertex2f(centerX + radius * cos(i * M_PI / 3), centerY + radius * sin(i * M_PI / 3));
    }
    glEnd();
}

void drawnCircumference(Form f) {
    float centerX = f->x + f->xSize / 2;
    float centerY = f->y + f->ySize / 2;
    float radius = f->xSize / 2;

    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; ++i) {
        glVertex2f(centerX + radius * cos(i * M_PI / 180), centerY + radius * sin(i * M_PI / 180));
    }
    glEnd();

    glColor3f(f->lr, f->lg, f->lb);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 360; ++i) {
        glVertex2f(centerX + radius * cos(i * M_PI / 180), centerY + radius * sin(i * M_PI / 180));
    }
    glEnd();
}

void drawBox(Form f) {
    glColor3f(f->r, f->g, f->b);
    glBegin(GL_POLYGON);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();

    glColor3f(f->lr, f->lg, f->lb);
    glBegin(GL_LINE_LOOP);
    glVertex2f(f->x, f->y);
    glVertex2f(f->x, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y + f->ySize);
    glVertex2f(f->x + f->xSize, f->y);
    glEnd();
}

void drawForm(Form f) {
    switch (f->type) {
        case TRIANGLE_ISO:
            drawTriangleIso(f); break;
        case RECTANGLE:
            drawBox(f); break;
        case SQUARE:
            drawBox(f); break;
        case LINE:
            drawLine(f); break;
        case TRIANGLE_EQUI:
            drawTriangleEqui(f); break;
        case HEXAGON:
            drawHexagon(f); break;
        case CIRCUMFERENCE:
            drawnCircumference(f); break;
    }
}


