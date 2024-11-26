/*
Nuno Fernandes nº21635
Rafael Carvalho nº21986

Funcionamento do Paint

Atalhos do teclado:
Q e q --> faz exit do programa
R e r --> type = RECTANGLE
T e t --> type = TRIANGLE_ISO
S e s --> type = SQUARE
L e l --> type = LINE
E e "e" --> type = TRIANGLE_EQUI
H e h --> type = HEXAGON
C e c --> type = CIRCUMFERENCE
1,2,3,4,5,6 --> aumenta e diminui as cores
I e i -->  currentState = INSERT
D e d --> currentState = DELETE
M e m --> currentState = MOVE
P e p --> currentState = EDIT
Z e z --> currentState = RESIZE

Barra de ferramentas:
O primeiro estado é o Inserir (cruz no meio)
O segundo é o Apagar (X)
O terceiro é o Mover (manter pressionado o botão esquerdo e mover)
O quarto é o Editar ou Pintar (Muda a cor do background ou das linhas da figura)
O quinto é o Resize, ou seja, é o que ajusta o tamanho da figura (manter pressionado o botão esquerdo e arrastar)

*/

#include "form.h"
#include "DBForms.h"
#include <stdlib.h>
#include <GL/glut.h>
//#include <time.h>
#include "stdio.h"

int counter = 0;

int windowX = 800;
int windowY = 600;
int drawArea = 70;

// Variáveis de estado
#define INSERT 0
#define DELETE 1
#define MOVE 2
#define EDIT 3
#define RESIZE 4 // Alterar o tamanho da figura
int currentState = INSERT;

// state variable
int type = RECTANGLE;
float rState = 1.0;
float gState = 1.0;
float bState = 1.0;
float lrState = 0.0;
float lgState = 0.0;
float lbState = 0.0;
float deltaColor = 0.1; // Update do RGB


int creatingForm = 0;

Form selectedForm = NULL;

// feedback
Form activeColor;

// save click last position
int xLastClick;
int yLastClick;

// color palette (Form array)
Form* palette;
Form* paletteLine;
int nPaletteColors;

// figure palette
Form* paletteFigures;
int nPaletteFigures;

// state palette (switch state)
Form* paletteState;
int nPaletteStates;


void deleteSomeForm(int x, int y) {
    Form f = pickDBForm(x, y);
    if (f != NULL) {
        printfForm(f);
        deleteFormDB(f);
        glutPostRedisplay();
    }
    else
        printf("No Form...\n");
}

void mymouseTools(GLint button, GLint state, GLint x, GLint y) {
    int i;
    float color[3];

    if (creatingForm) {
        return;
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Verificar paleta de cores
        for (i = 0; i < nPaletteColors; i++) {
            if (pick(palette[i], x, y)) {
                getBGColor(palette[i], color);
                rState = color[0];
                gState = color[1];
                bState = color[2];
                setBackgroundColor(activeColor, rState, gState, bState);
                glutPostRedisplay();
                return;
            }

            if (pick(paletteLine[i], x, y)) {
                getBGLineColor(paletteLine[i], color);
                lrState = color[0];
                lgState = color[1];
                lbState = color[2];
                setLineColor(activeColor, lrState, lgState, lbState);
                glutPostRedisplay();
                return;
            }
        }

        // Verificar paleta de formas
        for (i = 0; i < nPaletteFigures; i++) {
            if (pick(paletteFigures[i], x, y)) {
                type = getFormType(paletteFigures[i]); // Atualizar o tipo de forma selecionado
                // Recriar activeColor com o novo tipo de forma
                if (type == RECTANGLE) {
                    activeColor = newForm(type, 10, windowY - 60, 50, 25); // mudei a altura para metade da largura
                } else {
                    activeColor = newForm(type, 10, windowY - 60, 50, 50); //mudei para -60 para ajustar a altura
                }
                setBackgroundColor(activeColor, rState, gState, bState);
                setLineColor(activeColor, lrState, lgState, lbState);
                glutPostRedisplay();
                return;
            }
        }


        for (i = 0; i < nPaletteStates; i++) {
            if (pick(paletteState[i], x, y)) {
                if (currentState != i) {
                    setLineColor(paletteState[currentState], 0.0, 0.0, 0.0); // Cor do estado anterior
                    currentState = i;
                    setLineColor(paletteState[i], 1.0, 0.0, 0.0); // Cor do estado selecionado
                }
            }
            glutPostRedisplay();
        }

        for (int i = 0; i < nPaletteStates; i++) {
            if (pick(paletteState[i], x, y)) {
                if (currentState != i) {
                    setLineColor(paletteState[currentState], 0.0, 0.0, 0.0); // Cor do estado anterior
                    currentState = i;
                    setLineColor(paletteState[i], 1.0, 0.0, 0.0); // Cor do estado selecionado
                }
            }
            glutPostRedisplay();
        }
    }
}

void insertState(GLint button, GLint state, GLint x, GLint y) {
    if (currentState == INSERT) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            printf("-> LEFT button pressed!\n");

            if (creatingForm) { // segundo clique
                if (x >= 0 && x <= windowX && y >= 0 && y <= windowY - drawArea) {
                    change2Point(selectedForm, x, y);
                    creatingForm = 0;
                    selectedForm = NULL;
                    glutPostRedisplay();
                }
            }
            else { // primeiro clique
                if (x >= 0 && x <= windowX && y >= 0 && y <= windowY - drawArea) {
                    switch (type) {
                        case RECTANGLE:
                            selectedForm = newRectangle2Point(x, y, x, y);
                            break;
                        case SQUARE:
                            selectedForm = newSquare(x, y, 0);
                            break;
                        case TRIANGLE_ISO:
                            selectedForm = newTriangleIso(x, y, 0, 0);
                            break;
                        case LINE:
                            selectedForm = newLine(x, y, 0, 0);
                            break;
                        case TRIANGLE_EQUI:
                            selectedForm = newTriangleEqui(x, y, 0, 0);
                            break;
                        case HEXAGON:
                            selectedForm = newHexagon(x, y, 0, 0);
                            break;
                        case CIRCUMFERENCE:
                            selectedForm = newCircumference(x, y, 0, 0);
                            break;
                        default:
                            printf("Unknown form type!\n");
                            return;
                    }

                    if (selectedForm != NULL) {
                        setBackgroundColor(selectedForm, rState, gState, bState);
                        setLineColor(selectedForm, lrState, lgState, lbState);
                        if (!insertBDForm(selectedForm)) {
                            printf("MEMORY FULL!!!\n");
                            deleteForm(selectedForm);
                            selectedForm = NULL;
                        }
                        else {
                            creatingForm = 1;
                            glutPostRedisplay();
                        }
                    }
                    else {
                        printf("Error: Failed to create new form.\n");
                    }
                }
            }
        }
    }
}

void deleteState(GLint button, GLint state, GLint x, GLint y) {
    if (currentState == DELETE) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            printf("-> LEFT button pressed!\n");
            deleteSomeForm(x, y);
            glutPostRedisplay();
        }
    }
}

void moveState(GLint button, GLint state, GLint x, GLint y) {
    if (currentState == MOVE) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            printf("-> LEFT button pressed!\n");
            if (selectedForm == NULL) {
                selectedForm = pickDBForm(x, y);
                xLastClick = x;
                yLastClick = y;
            }
            else {
                selectedForm = NULL;
            }
            glutPostRedisplay();
        }
    }
}

void editState(GLint button, GLint state, GLint x, GLint y) {
    if (currentState == EDIT) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            printf("-> LEFT button pressed!\n");
            selectedForm = pickDBForm(x, y);

            if (selectedForm != NULL) {
                for (int i = 0; i < nPaletteColors; i++) {
                    if (pick(palette[i], x, y)) {
                        float color[3];
                        getBGColor(palette[i], color);
                        rState = color[0];
                        gState = color[1];
                        bState = color[2];
                    }

                    if (pick(paletteLine[i], x, y)) {
                        float color[3];
                        getBGLineColor(paletteLine[i], color);
                        lrState = color[0];
                        lgState = color[1];
                        lbState = color[2];
                    }
                }
                setBackgroundColor(selectedForm, rState, gState, bState);
                setLineColor(selectedForm, lrState, lgState, lbState);
                getFormType(selectedForm);

                glutPostRedisplay();
            }
        }
    }
}

void resizeState(GLint button, GLint state, GLint x, GLint y) {
    if (currentState == RESIZE) {
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            printf("-> LEFT button pressed for resizing!\n");
            if (selectedForm == NULL) {
                selectedForm = pickDBForm(x, y);
                xLastClick = x;
                yLastClick = y;
            }
        }
        else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            selectedForm = NULL;
        }
    }
}


void mymouseCanvas(GLint button, GLint state, GLint x, GLint y) {
    if (y > windowY - drawArea) {
        return; // Não permitir desenho na área da paleta
    }

    insertState(button, state, x, y);
    deleteState(button, state, x, y);
    moveState(button, state, x, y);
    editState(button, state, x, y);
    resizeState(button, state, x, y);
}


void mymouse(GLint button, GLint state, GLint x, GLint y) {


    y = windowY - y;


    if (y > windowY - drawArea) {
        mymouseTools(button, state, x, y);
    }
    else
        mymouseCanvas(button, state, x, y);

}


//Atalhos do teclado
void mykey(unsigned char key, int x, int y) {
    y = windowY - y;

    if (key == 'Q' || key == 'q') {
        exit(0);
    }

    switch (key) {
        case 'r':
        case 'R':
            type = RECTANGLE;
            break;
        case 't':
        case 'T':
            type = TRIANGLE_ISO;
            break;
        case 's':
        case 'S':
            type = SQUARE;
            break;
        case 'l':
        case 'L':
            type = LINE;
            break;
        case 'e':
        case 'E':
            type = TRIANGLE_EQUI;
            break;
        case 'h':
        case 'H':
            type = HEXAGON;
            break;
        case 'c':
        case 'C':
            type = CIRCUMFERENCE;
            break;

        case '1':
            rState += deltaColor;
            if (rState > 1.0) {
                rState = 1.0;
            }
            break;

        case '2':
            gState += deltaColor;
            if (gState > 1.0) {
                gState = 1.0;
            }
            break;

        case '3':
            bState += deltaColor;
            if (bState > 1.0) {
                bState = 1.0;
            }
            break;

        case '4':
            rState -= deltaColor;
            if (rState < 0.0) {
                rState = 0.0;
            }
            break;

        case '5':
            gState -= deltaColor;
            if (gState < 0.0) {
                gState = 0.0;
            }
            break;

        case '6':
            bState -= deltaColor;
            if (bState < 0.0) {
                bState = 0.0;
            }
            break;

        case 'i':
        case 'I':
            currentState = INSERT;
            break;

        case 'd':
        case 'D':
            currentState = DELETE;
            break;

        case 'm':
        case 'M':
            currentState = MOVE;
            break;

        case 'p':
        case 'P':
            currentState = EDIT;
            break;

        case 'z':
        case 'Z':
            currentState = RESIZE;
            break;

    }
    setBackgroundColor(activeColor, rState, gState, bState);
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    y = windowY - y;

    if (currentState == RESIZE && selectedForm != NULL) {
        if (x >= 0 && x <= windowX && y >= 0 && y <= windowY - drawArea) {
            change2Point(selectedForm, x, y);
            glutPostRedisplay();
        }
    }

    if (currentState == MOVE && selectedForm != NULL) {
        if (x >= 0 && x <= windowX && y >= 0 && y <= windowY - drawArea) {
            updateForm(selectedForm, x - xLastClick, y - yLastClick);
            xLastClick = x;
            yLastClick = y;
            glutPostRedisplay();
        }
    }
}

void mousePassiveMotion(int x, int y) {
    y = windowY - y;

    printf("Moving... (%i, %i)\n", x, y);


    if (creatingForm) { // segundo clique
        if (x >= 0 && x <= windowX && y >= 0 && y <= windowY - drawArea) {
            change2Point(selectedForm, x, y);
            glutPostRedisplay();
        }
    }
}

void drawDividingLines() {
    glColor3f(0.0, 0.0, 0.0); // Cor preta para as linhas divisórias

    // Linha divisória entre a barra de tarefas e a área de desenho
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2d(0, windowY - drawArea);
    glVertex2d(windowX, windowY - drawArea);
    glEnd();

    // Linha divisória entre a área de cores e a área de linhas
    glBegin(GL_LINES);
    glVertex2f(220, windowY - drawArea);
    glVertex2f(220, windowY);
    glEnd();

    // Linha divisória entre a área de linhas e a área de figuras
    glBegin(GL_LINES);
    glVertex2f(390, windowY - drawArea);
    glVertex2f(390, windowY);
    glEnd();

    // Linha divisória entre a área das figuras e a área de estados/modos
    glBegin(GL_LINES);
    glVertex2f(830, windowY - drawArea);
    glVertex2f(830, windowY);
    glEnd();
}

void drawStates() {
    int i = 0;

    for (i = 0; i < nPaletteStates; i++) {
        drawForm(paletteState[i]);

        if (i == INSERT) {
            if (currentState == INSERT) {
                glColor3f(1.0, 0.0, 0.0); // Cor vermelha para o sinal "+" quando selecionado
            }
            else {
                glColor3f(0.0, 0.0, 0.0); // Cor preta para o sinal "+" quando não selecionado
            }
            glBegin(GL_LINES);
            glVertex2f(875, windowY - 55); // Linha vertical do "+"
            glVertex2f(875, windowY - 15);
            glVertex2f(855, windowY - 35); // Linha horizontal do "+"
            glVertex2f(895, windowY - 35);
            glEnd();
        }
        else if (i == DELETE) {
            if (currentState == DELETE) {
                glColor3f(1.0, 0.0, 0.0); // Cor vermelha para o sinal "x" quando selecionado
            }
            else {
                glColor3f(0.0, 0.0, 0.0); // Cor preta para o sinal "x" quando não selecionado
            }
            glBegin(GL_LINES);
            glVertex2f(915, windowY - 55);
            glVertex2f(955, windowY - 15);
            glVertex2f(915, windowY - 15);
            glVertex2f(955, windowY - 55);
            glEnd();
        }
        else if (i == MOVE) {
            if (currentState == MOVE) {
                glColor3f(1.0, 0.0, 0.0); // Cor vermelha para o sinal "x" quando selecionado
            }
            else {
                glColor3f(0.0, 0.0, 0.0); // Cor preta para o sinal "x" quando não selecionado
            }
            Form s1 = newForm(SQUARE, 985, windowY - 40, 15, 15);
            setBackgroundColor(s1, 1.0, 1.0, 1.0);
            setLineColor(s1, 0.0, 0.0, 0.0);
            drawForm(s1);

            Form s2 = newForm(SQUARE, 995, windowY - 50, 15, 15);
            setBackgroundColor(s2, 0.0, 0.0, 0.0);
            setLineColor(s2, 0.0, 0.0, 0.0);
            drawForm(s2);
        }

        else if (i == EDIT) {
            if (currentState == EDIT) {
                glColor3f(1.0, 0.0, 0.0); // Cor vermelha para o ícone de redimensionamento quando selecionado
            }
            else {
                glColor3f(0.0, 0.0, 0.0); // Cor preta para o ícone de redimensionamento quando não selecionado
            }
            Form s3 = newForm(SQUARE, 1045, windowY - 45, 20, 20);
            setBackgroundColor(s3, 0.0, 0.0, 0.0);
            setLineColor(s3, 0.0, 0.0, 0.0);
            drawForm(s3);

        }

        else if (i == RESIZE) {
            if (currentState == RESIZE) {
                glColor3f(1.0, 0.0, 0.0); // Cor vermelha para o ícone de redimensionamento quando selecionado
            } else {
                glColor3f(0.0, 0.0, 0.0); // Cor preta para o ícone de redimensionamento quando não selecionado
            }
            Form s4 = newForm(SQUARE, 1100, windowY - 40, 15, 15);
            setBackgroundColor(s4, 1.0, 1.0, 1.0);
            setLineColor(s4, 0.0, 0.0, 0.0);
            drawForm(s4);

            Form s5 = newForm(SQUARE, 1110, windowY - 50, 20, 20);
            setBackgroundColor(s5, 0.0, 0.0, 0.0);
            setLineColor(s5, 0.0, 0.0, 0.0);
            drawForm(s5);
        }
    }
}

void mydisplay() {
    int i;

    glClear(GL_COLOR_BUFFER_BIT);

    // Desenhar as linhas divisórias
    drawDividingLines();


    glBegin(GL_LINES);
    glVertex2f(-4.0, 0.0);
    glVertex2f(4.0, 0.0);
    glVertex2f(0.0, -4.0);
    glVertex2f(0.0, 4.0);
    glEnd();

    drawDBForms();

    // QUADRADO GRANDE
    drawForm(activeColor);
    // QUADRADOS COM AS CORES
    for (i = 0; i < nPaletteColors; i++)
        drawForm(palette[i]);
    // QUADRADOS COM AS CORES OUTLINE
    for (i = 0; i < nPaletteColors; i++) {
        drawForm(paletteLine[i]);
    }
    // FIGURAS
    for (i = 0; i < nPaletteFigures; i++) {
        drawForm(paletteFigures[i]);
    }
    // ESTADOS
    drawStates(paletteState[i]);

    glFlush();

    printf("Counter: %i\n", ++counter);
}

//Inicialização da paleta de cores
void initPaletteColors() {
    nPaletteColors = 10;
    palette = malloc(sizeof(Form) * nPaletteColors);
    palette[0] = newSquare(70, windowY - 30, 20);
    setBackgroundColor(palette[0], 1.0, 0.0, 0.0); // red

    palette[1] = newSquare(100, windowY - 30, 20);
    setBackgroundColor(palette[1], 0.0, 1.0, 0.0); // green

    palette[2] = newSquare(130, windowY - 30, 20);
    setBackgroundColor(palette[2], 0.0, 0.0, 1.0); // blue

    palette[3] = newSquare(160, windowY - 30, 20);
    setBackgroundColor(palette[3], 1.0, 0.0, 1.0); // magenta

    palette[4] = newSquare(190, windowY - 30, 20);
    setBackgroundColor(palette[4], 1.0, 1.0, 0.0); // yellow

    palette[5] = newSquare(70, windowY - 60, 20);
    setBackgroundColor(palette[5], 0.0, 1.0, 1.0); // cyan

    palette[6] = newSquare(100, windowY - 60, 20);
    setBackgroundColor(palette[6], 1.0, 0.5, 0.0); // orange

    palette[7] = newSquare(130, windowY - 60, 20);
    setBackgroundColor(palette[7], 0.5, 0.0, 0.5); // purple

    palette[8] = newSquare(160, windowY - 60, 20);
    setBackgroundColor(palette[8], 0.5, 0.5, 0.5); // grey

    palette[9] = newSquare(190, windowY - 60, 20);
    setBackgroundColor(palette[9], 0.0, 0.0, 0.0); // black
}

//Inicialização da paleta das linhas
void initPaletteLines() {
    paletteLine = malloc(sizeof(Form) * nPaletteColors);
    paletteLine[0] = newSquare(240, windowY - 30, 20);
    setLineColor(paletteLine[0], 1.0, 0.0, 0.0);

    paletteLine[1] = newSquare(270, windowY - 30, 20);
    setLineColor(paletteLine[1], 0.0, 1.0, 0.0);

    paletteLine[2] = newSquare(300, windowY - 30, 20);
    setLineColor(paletteLine[2], 0.0, 0.0, 1.0);

    paletteLine[3] = newSquare(330, windowY - 30, 20);
    setLineColor(paletteLine[3], 1.0, 0.0, 1.0);

    paletteLine[4] = newSquare(360, windowY - 30, 20);
    setLineColor(paletteLine[4], 1.0, 1.0, 0.0);

    paletteLine[5] = newSquare(240, windowY - 60, 20);
    setLineColor(paletteLine[5], 0.0, 1.0, 1.0);

    paletteLine[6] = newSquare(270, windowY - 60, 20);
    setLineColor(paletteLine[6], 1.0, 0.5, 0.0);

    paletteLine[7] = newSquare(300, windowY - 60, 20);
    setLineColor(paletteLine[7], 0.5, 0.0, 0.5);

    paletteLine[8] = newSquare(330, windowY - 60, 20);
    setLineColor(paletteLine[8], 0.5, 0.5, 0.5);

    paletteLine[9] = newSquare(360, windowY - 60, 20);
    setLineColor(paletteLine[9], 0.0, 0.0, 0.0);

}


//Inicialização da paleta de figuras
void initPaletteFigures() {
    nPaletteFigures = 7; // Número de figuras
    paletteFigures = malloc(sizeof(Form) * nPaletteFigures);

    int startX = 410; // Posição inicial X
    int startY = windowY - 30; // Posição inicial Y
    int spacing = 60; // Espaçamento entre as figuras
    int size = 50; // Tamanho das figuras, igual ao activeColor

    // Retângulo
    paletteFigures[0] = newForm(RECTANGLE, startX, startY, size, size/2);
    setBackgroundColor(paletteFigures[0], 1.0, 1.0, 1.0);
    setLineColor(paletteFigures[0], 0.0, 0.0, 0.0);

    // Triângulo Isósceles
    paletteFigures[1] = newForm(TRIANGLE_ISO, startX + spacing, startY, size, size);
    setBackgroundColor(paletteFigures[1], 1.0, 1.0, 1.0);
    setLineColor(paletteFigures[1], 0.0, 0.0, 0.0);

    // Quadrado
    paletteFigures[2] = newForm(SQUARE, startX + 2 * spacing, startY, size, size);
    setBackgroundColor(paletteFigures[2], 1.0, 1.0, 1.0);
    setLineColor(paletteFigures[2], 0.0, 0.0, 0.0);

    // Linha
    paletteFigures[3] = newForm(LINE, startX + 3 * spacing, startY, size, size);
    setBackgroundColor(paletteFigures[3], 0.0, 0.0, 0.0);
    setLineColor(paletteFigures[3], 0.0, 0.0, 0.0);

    // Triângulo Equilátero
    paletteFigures[4] = newForm(TRIANGLE_EQUI, startX + 4 * spacing, startY, size, size);
    setBackgroundColor(paletteFigures[4], 1.0, 1.0, 1.0);
    setLineColor(paletteFigures[4], 0.0, 0.0, 0.0);

    // Hexágono
    paletteFigures[5] = newForm(HEXAGON, startX + 5 * spacing, startY, size, size);
    setBackgroundColor(paletteFigures[5], 1.0, 1.0, 1.0);
    setLineColor(paletteFigures[5], 0.0, 0.0, 0.0);

    // Circunferência
    paletteFigures[6] = newForm(CIRCUMFERENCE, startX + 6 * spacing, startY, size, size);
    setBackgroundColor(paletteFigures[6], 1.0, 1.0, 1.0);
    setLineColor(paletteFigures[6], 0.0, 0.0, 0.0);
}


//Inicialização da paleta de estados
void initPaletteStates() {
    nPaletteStates = 5;
    paletteState = malloc(sizeof(Form) * nPaletteStates);

    paletteState[0] = newSquare(850, windowY - 30, 50);
    setLineColor(paletteState[0], 1.0, 0.0, 0.0); // red (inicia o estado INSERT a vermelho)

    paletteState[1] = newSquare(910, windowY - 30, 50);

    paletteState[2] = newSquare(970, windowY - 30, 50);

    paletteState[3] = newSquare(1030, windowY - 30, 50);

    paletteState[4] = newSquare(1090, windowY - 30, 50); // Novo estado de redimensionamento

}


// Inicializar a cor ativa
void initActiveColor() {
    activeColor = newForm(type, 10, windowY - 20, 50, 50);
    setBackgroundColor(activeColor, rState, gState, bState);
    setLineColor(activeColor, lrState, lgState, lbState);
}



void init() {
    int i;

    //srand(time(NULL));

    initDBForms(20);
    //populateDBForms(windowX);

    printDBForms();

    initActiveColor(); // Inicializar a cor ativa/figura ativa/linha ativa

    initPaletteColors(); // Inicializar a paleta de cores

    initPaletteLines(); // Inicializar a paleta de linhas

    initPaletteFigures(); // Inicializar a paleta de figuras

    initPaletteStates();


    glClearColor(1.0, 1.0, 1.0, 1.0);

    glColor3f(1.0, 1.0, 1.0);

    glViewport(0, 0, windowX, windowY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowX, 0.0, windowY);
}

void myshape(int w, int h) {
    windowX = w;
    windowY = h;

    updateFormY(activeColor, windowY - 60);
    for (int i = 0; i < nPaletteColors; i++) {
        if (i < 5) {
            updateFormY(palette[i], windowY - 30);
        }
        else {
            updateFormY(palette[i], windowY - 60);
        }
    }

    for (int i = 0; i < nPaletteColors; i++) {
        if (i < 5) {
            updateFormY(paletteLine[i], windowY - 30);
        }
        else {
            updateFormY(paletteLine[i], windowY - 60);
        }
    }

    // Atualizar posição Y das formas
    for (int i = 0; i < nPaletteFigures; i++) {
        updateFormY(paletteFigures[i], windowY - 60);
    }

    for (int i = 0; i < nPaletteStates; i++) {
        updateFormY(paletteState[i], windowY - 60);
    }

    glViewport(0, 0, windowX, windowY);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, windowX, 0.0, windowY);
}

void main(int argc, char** argv) {
    glutInit(&argc, argv);        /* Freeglut needs this line!*/
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(windowX, windowY);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("simple");
    glutDisplayFunc(mydisplay);
    glutMouseFunc(mymouse);
    glutKeyboardFunc(mykey);
    glutReshapeFunc(myshape);
    glutMotionFunc(mouseMotion);
    glutPassiveMotionFunc(mousePassiveMotion);

    init();

    glutMainLoop();
}