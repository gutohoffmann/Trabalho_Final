/**
*   Arquivo func_allegro.h do trabalho final da disciplina de Programação 2
*
*   Instituto Federal de Santa Catarina
*   Curso: Engenharia Eletrônica
*   Disciplina: Programação 2
*   Semestre 2015/2
*   Professor: Daniel Lohmann
*   Aluno: Augusto Hoffmann
*
*/

#ifndef FUNC_ALLEGRO_H_INCLUDED
#define FUNC_ALLEGRO_H_INCLUDED

#define ALLEGRO_STATICLINK
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct header Header;
typedef struct serie Serie;
typedef struct resistor Resistor;

struct resistor
{
    float value;
    float power;
    int amount;
    Resistor* nextR;
    Resistor* prevR;
};

struct serie
{
    int serie;
    int sizeS;
    Serie* nextS;
    Serie* prevS;
    Resistor* firstR;
    Resistor* lastR;
};

struct header
{
    int sizeH; //tamanho
    Serie *firstS; //ponteiro apontando para o inicio da lista
    Serie *lastS; //ponteiro apontando para o fim da lista
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Resistor* createResistor (Header* hd, float val, float pow, int amt);
Serie* createSerieAndResistor (Header* hd, int ser, float val, float pow, int amt);
Header* createHeader ();
int loadFile(Header* hd);
Header* loadResistor(Header* hd, int ser, float val, float pow, int amt);
int saveFile(Header* hd); // também libera tudo
Header* addResistor (Header* hd);
Header* removeResistor (Header* hd);
Header* searchResistor (Header* hd);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**< Função que inicia a parte gráfica */
int graphic_init(void);






#endif // FUNC_ALLEGRO_H_INCLUDED


