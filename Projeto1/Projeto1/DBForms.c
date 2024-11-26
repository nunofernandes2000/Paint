#include "form.h"
#include "DBForms.h"
#include <stdlib.h>

// Array of pointers to form objects (Recall, Form is a pointer!)
Form* a;
int n;

void initDBForms(int num) {
    int i;

    n = num;

    a = malloc(sizeof(Form) * n);

    // initialize array of forms (pointers to forms) to NULL
    for (i = 0; i < n; i++)
        a[i] = NULL;
}

void populateDBForms(int maxSize) {
    int i;

    for (i = 0; i < n; i++)
        if (a[i] == NULL) {
            a[i] = createRandomForm(maxSize);
            setBackgroundColor(a[i], rand() / (RAND_MAX * 1.0), rand() / (RAND_MAX * 1.0), rand() / (RAND_MAX * 1.0));
        }


}

int insertBDForm(Form f) {
    int i;

    for (i = 0; i < n; i++)
        if (a[i] == NULL) {
            a[i] = f;
            return 1;
        }

    return 0;
}

void deleteRandomDBForm() {
    int i = rand() % n;
    if (a[i] != NULL) {
        deleteForm(a[i]);
        a[i] = NULL;
    }
}

// Quando apagar uma forma quero "puxar" as outras para a esquerda
int deleteFormDB(Form f) {
    int i;

    // Se o array estiver cheio e tentar apagar uma figura que n�o est�, este vai apagar a �ltima figura
    // Assim, tenho que verificar se encontrei a figura
    int formDeleted = 0;

    for (i = 0; i < n && a[i] != NULL; ++i) {
        if (a[i] == f) {           // Encontra a posi��o onde a figura est�
            deleteForm(a[i]);      // Libera a mem�ria da figura
            formDeleted = 1;
            break;
        }
    }
    // Encontrou a forma e apagou-a

    if (formDeleted) {
        // A pr�xima forma vai come�ar na posi��o seguinte at� ao final do array ou at� encontrar uma posi��o vazia
        for (i = i + 1; i < n && a[i] != NULL; ++i) {
            a[i - 1] = a[i];
        }
        // Como a �ltima forma foi copiada para a posi��o anterior, vou colocar a �ltima a NULL
        a[i - 1] = NULL;
    }

    // Assim consigo ver se a forma foi efetivamente apagada
    return formDeleted;
}



void drawDBForms() {
    int i;
    for (i = 0; i < n; i++)
        if (a[i] != NULL)
            drawForm(a[i]);
}

void printDBForms() {
    int i;

    // print the content of the array
    for (i = 0; i < n; i++)
        if (a[i] != NULL)
            printfForm(a[i]);
}

Form pickDBForm(float x, float y) {
    int i;

    for (i = n - 1; i >= 0; i--)
        if (a[i] != NULL && pick(a[i], x, y)) {
            return a[i];
        }

    return NULL;
}