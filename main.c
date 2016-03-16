/**
*   Arquivo main.c do trabalho final da disciplina de Programa��o 2
*
*   Instituto Federal de Santa Catarina
*   Curso: Engenharia Eletr�nica
*   Disciplina: Programa��o 2
*   Semestre 2015/2
*   Professor: Daniel Lohmann
*   Aluno: Augusto Hoffmann
*
*/

#include "func_allegro.h"


int main(void)
{
    // Inicia ambiente grafico
    if (graphic_init())
        printf("\nInterface grafica inicializada.\n");
    else
    {
        printf("\nProblema na inicializacao da interface grafica.\n");
        return -1;
    }

    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
