#ifndef __DBFORM_H__
#define __DBFORM_H__

#include "form.h"

void initDBForms(int num);
void populateDBForms(int maxSize);
int insertBDForm(Form f);
void deleteRandomDBForm();
void drawDBForms();
void printDBForms();
int deleteFormDB(Form f);

// fun��o para ver se est� alguma forma dentro do XY e retorna
Form pickDBForm(float x, float y);

#endif
