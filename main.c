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
int saveFile(Header* hd); // também libera tudo
Header* addResistor (Header* hd, int ser, float val, float pow, int amt);
Header* removeResistor (Header* hd, int ser, float val, float pow);
Header* searchResistor (Header* hd, int ser, float val, float pow);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    Header* h = createHeader();
    loadFile(h);
    addResistor(h, 3, 330, 2, 10);
    removeResistor(h, 23, 1000, 0.25);
    removeResistor(h, 24, 1000, 0.3);
    removeResistor(h, 24, 1000, 0.25);
    saveFile(h);

    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Resistor* createResistor (Header* hd, float val, float pow, int amt)
{
    Resistor* r = (Resistor*) malloc(sizeof(Resistor));
    if (r == NULL) exit(EXIT_FAILURE);
    int resistorCreated = 0; // flag

    r->value = val;
    r->power = pow;
    r->amount = amt;

    if (hd->firstS->sizeS == 0)
    {
        hd->firstS->firstR = r;
        hd->firstS->lastR = r;
        r->nextR = NULL;
        r->prevR = NULL;
    }
    else
    {
        while (hd->firstS->firstR->value > val) // enquanto o valor passado for menor que o existente
        {
            if (hd->firstS->firstR != hd->firstS->lastR) // se não está no ultimo nó de resistor
            {
                hd->firstS->firstR = hd->firstS->firstR->nextR; // vai pro proximo e verifica
            }
            else // se estiver no último nó, insere no fim
            {
                r->nextR = NULL;
                r->prevR = hd->firstS->firstR;
                hd->firstS->firstR->nextR = r;
                hd->firstS->lastR = r;
                while (hd->firstS->firstR->prevR != NULL)
                {
                    hd->firstS->firstR = hd->firstS->firstR->prevR; // coloca o ponteiro hd->firstS->firstR no começo
                }
                resistorCreated = 1;
            }
        }
        // se chegou aqui, valor passado maior que valor existente
        //if (resistorCreated == 1) break; // sai do else se já criou resistor

        // aqui cria no inicio da lista
        if (hd->firstS->firstR->prevR == NULL && resistorCreated == 0) // se o anterior é nulo, tem que inserir no inicio da lista de resistores
        {
            r->nextR = hd->firstS->firstR;
            r->prevR = NULL;
            hd->firstS->firstR->prevR = r;
            hd->firstS->firstR = r;
            while (hd->firstS->firstR->prevR != NULL)
            {
                hd->firstS->firstR = hd->firstS->firstR->prevR; // coloca o ponteiro hd->firstS->firstR no começo
            }
            resistorCreated = 1;
        }
        //if (resistorCreated == 1) break; // sai do else se já criou resistor


        //a partir daqui, cria no meio da lista
        if (resistorCreated == 0)
        {
            r->nextR = hd->firstS->firstR;
            r->prevR = hd->firstS->firstR->prevR;
            hd->firstS->firstR->prevR->nextR = r;
            hd->firstS->firstR->prevR = r;
            while (hd->firstS->firstR->prevR != NULL)
            {
                hd->firstS->firstR = hd->firstS->firstR->prevR; // coloca o ponteiro hd->firstS->firstR no começo
            }
            resistorCreated = 1;
        }

    }

    hd->firstS->sizeS++;
    return r;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Serie* createSerieAndResistor (Header* hd, int ser, float val, float pow, int amt)
{
    Serie* s = (Serie*) malloc(sizeof(Serie));
    if (s == NULL) exit(EXIT_FAILURE);
    int serieCreated = 0; // flag

    s->serie = ser;
    s->sizeS = 0;

    if (hd->sizeH == 0)
    {
        hd->firstS = s;
        hd->lastS = s;
        s->nextS = NULL;
        s->prevS = NULL;
        createResistor(hd, val, pow, amt);
    }
    else
    {
        while (hd->firstS->serie > ser && serieCreated == 0) // enquanto o valor passado for menor que o existente
        {
            if (hd->firstS != hd->lastS) // se não está no ultimo nó de serie
            {
                hd->firstS = hd->firstS->nextS; // vai pro proximo e verifica
            }
            else // se estiver no último nó, insere no fim
            {
                s->nextS = NULL;
                s->prevS = hd->firstS;
                hd->firstS->nextS = s;
                hd->lastS = s;
                hd->firstS = s; // aponto firstS para s para criar o resistor
                createResistor(hd, val, pow, amt);
                while (hd->firstS->prevS != NULL)
                {
                    hd->firstS = hd->firstS->prevS; // coloca o ponteiro hd->firstS no começo
                }
                serieCreated = 1;
            }
        }
        // se chegou aqui, valor passado maior que valor existente
        //if (resistorCreated == 1) break; // sai do else se já criou resistor

        // aqui cria no inicio da lista
        if (hd->firstS->prevS == NULL && serieCreated == 0) // se o anterior é nulo, tem que inserir no inicio da lista de resistores
        {
            s->nextS = hd->firstS;
            s->prevS = NULL;
            hd->firstS->prevS = s;
            hd->firstS = s;
            createResistor(hd, val, pow, amt);
            while (hd->firstS->prevS != NULL)
            {
                hd->firstS = hd->firstS->prevS; // coloca o ponteiro hd->firstS no começo
            }
            serieCreated = 1;
        }
        //if (resistorCreated == 1) break; // sai do else se já criou resistor


        //a partir daqui, cria no meio da lista
        if (serieCreated == 0)
        {
            s->nextS = hd->firstS;
            s->prevS = hd->firstS->prevS;
            hd->firstS->prevS->nextS = s;
            hd->firstS->prevS = s;
            hd->firstS = s; // para criar o resistor
            createResistor(hd, val, pow, amt);
            while (hd->firstS->prevS != NULL)
            {
                hd->firstS = hd->firstS->prevS; // coloca o ponteiro hd->firstS no começo
            }
            serieCreated = 1;
        }

    }

    hd->sizeH++;
    return s;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Header* createHeader ()
{
    Header* hd = (Header*) malloc(sizeof(Header)); //inicialização do cabeçalho
    hd->sizeH = 0;
    hd->firstS = NULL;
    hd->lastS = NULL;
    return hd;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int loadFile(Header* hd)
{
    int n;
    FILE *fp;
    fp = fopen ("lista_de_resistores_desorganizada.txt", "r");
    if (fp == NULL)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return 0;
    }
    Resistor* auxR = (Resistor*) malloc(sizeof(Resistor));
    Serie* auxS = (Serie*) malloc(sizeof(Serie));


    do
    {
        n = fscanf(fp, "%d %f %f %d\n", &(auxS->serie), &(auxR->value), &(auxR->power), &(auxR->amount));
        if (n == EOF) break;
        addResistor(hd, auxS->serie, auxR->value, auxR->power, auxR->amount);
    }
    while(n != EOF);
    fclose (fp);
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int saveFile(Header* hd)
{
    FILE *fp;
    fp = fopen ("lista_de_resistores.txt", "w");
    if (fp == NULL)
    {
        printf ("Erro ao abrir o arquivo para escrever.\n");
        return 0;
    }

    while (hd->sizeH > 0)
    {
        while (hd->firstS->sizeS > 0)
        {
            fprintf(fp, "%d %.2f %.3f %d\n", hd->firstS->serie, hd->firstS->firstR->value, hd->firstS->firstR->power, hd->firstS->firstR->amount);
            if (hd->firstS->sizeS > 1)
            {
                hd->firstS->firstR = hd->firstS->firstR->nextR; // vai para o próximo
                free(hd->firstS->firstR->prevR); // libera o que foi impresso
            }
            else
            {
                free(hd->firstS->firstR);
            }
            hd->firstS->sizeS--; // decrementa tamanho da serie
        }
        // chegou aqui apagou uma série e todos seus resistores
        if (hd->sizeH > 1)
        {
            hd->firstS = hd->firstS->nextS;
            free(hd->firstS->prevS);
        }
        else
        {
            free(hd->firstS);
        }
        hd->sizeH--;
    }
    free(hd);
    fclose (fp);
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Header* addResistor (Header* hd, int ser, float val, float pow, int amt)
{
    //Resistor* auxR;
    int serieAndResistorCreated = 0; // flag
    int resistorCreated = 0; // flag

    if (hd->sizeH == 0)
    {
        createSerieAndResistor(hd, ser, val, pow, amt);
    }
    else
    {
        while (ser != hd->firstS->serie)
        {
            if (hd->firstS != hd->lastS)
            {
                hd->firstS = hd->firstS->nextS;
            }
            else
            {
                while (hd->firstS->prevS != NULL) hd->firstS = hd->firstS->prevS; // quando chamo para criar a serie, tenho que ter certeza que aponta para o inicio
                createSerieAndResistor(hd, ser, val, pow, amt); // dentro da função já ordena por serie e retorna os ponteiros certos
                serieAndResistorCreated = 1; // criou
            }
        }
        if (serieAndResistorCreated == 0)
        {
            //se chegou aqui, achou a serie igual, agora verifica se tem resistor igual
            //se tiver, incrementa o tamanho, se não, cria resistor
            //o ponteiro que aponta para o resistor é hd->firstS->firstR
            while (val != hd->firstS->firstR->value || pow != hd->firstS->firstR->power) // se valor ou potencia forem diferentes
            {
                if (hd->firstS->firstR != hd->firstS->lastR)
                {
                    hd->firstS->firstR = hd->firstS->firstR->nextR;
                }
                else
                {
                    while (hd->firstS->firstR->prevR != NULL) // foi andando na lista de resistores para encontrar se tinha algum igual, se não tem, deve voltar
                    {
                        hd->firstS->firstR = hd->firstS->firstR->prevR;
                    }
                    createResistor(hd, val, pow, amt); // dentro da função já ordena por valor
                    resistorCreated = 1; // criou
                }
            }
            if (resistorCreated == 0)
            {
                //se chegou aqui, achou serie, valor e potencia iguais, deve incrementar a quantidade em auxR->amount
                hd->firstS->firstR->amount = (hd->firstS->firstR->amount + amt); // incrementa a quantidade
            }
        }
    }
// organiza ponteiros
    while (hd->firstS->firstR->prevR != NULL) // faz ponteiro ir para começo
    {
        hd->firstS->firstR = hd->firstS->firstR->prevR;
    }
    while (hd->firstS->lastR->nextR != NULL) // faz ponteiro ir para final
    {
        hd->firstS->lastR = hd->firstS->lastR->nextR;
    }
    while (hd->firstS->prevS != NULL) // faz ponteiro ir para inicio
    {
        while (hd->firstS->firstR->prevR != NULL) // faz ponteiro ir para começo
        {
            hd->firstS->firstR = hd->firstS->firstR->prevR;
        }
        while (hd->firstS->lastR->nextR != NULL) // faz ponteiro ir para final
        {
            hd->firstS->lastR = hd->firstS->lastR->nextR;
        }
        hd->firstS = hd->firstS->prevS;
    }
    while (hd->lastS->nextS != NULL) // faz ponteiro ir para final
    {
        while (hd->firstS->firstR->prevR != NULL) // faz ponteiro ir para começo
        {
            hd->firstS->firstR = hd->firstS->firstR->prevR;
        }
        while (hd->firstS->lastR->nextR != NULL) // faz ponteiro ir para final
        {
            hd->firstS->lastR = hd->firstS->lastR->nextR;
        }
        hd->lastS = hd->lastS->nextS;
    }

    return hd;
}


Header* removeResistor (Header* hd, int ser, float val, float pow)
{
    while (hd->firstS->serie != ser) // enquanto a série for diferente
    {
        if (hd->firstS->nextS == NULL) // se o proximo é nulo, não tem mais pra consultar
        {
            printf("\nResistor inexistente.\n");
            while (hd->firstS->prevS != NULL)
            {
                hd->firstS = hd->firstS->prevS;
            }
            return hd;
        }
        else
        {
            hd->firstS = hd->firstS->nextS; // anda uma série
        }
    }
    // se chegar aqui, achou a série

    while (hd->firstS->firstR->value != val || hd->firstS->firstR->power != pow) // achou a serie, mas valor epotencia não batem
    {
        if (hd->firstS->firstR->nextR == NULL) // se o proximo é nulo, não tem mais o que consultar
        {
            printf("\nResistor não encontrado.\n");
            while (hd->firstS->firstR->prevR != NULL)
            {
                hd->firstS->firstR = hd->firstS->firstR->prevR;
            }
            while (hd->firstS->prevS != NULL)
            {
                hd->firstS = hd->firstS->prevS;
            }
            return hd;
        }
        else
        {
            hd->firstS->firstR = hd->firstS->firstR->nextR;
        }
    }

    // se chegou aqui, encontrou pra remover
    if (hd->firstS->sizeS > 1)
    {
        if (hd->firstS->firstR->prevR != NULL) // se não estã no começo
        {
            if (hd->firstS->firstR->nextR != NULL) // se não está no final
            {
                hd->firstS->firstR->prevR->nextR = hd->firstS->firstR->nextR;
                hd->firstS->firstR->nextR->prevR = hd->firstS->firstR->prevR;
                free(hd->firstS->firstR);
                hd->firstS->firstR = hd->firstS->lastR;
                while (hd->firstS->firstR->prevR != NULL)
                {
                    hd->firstS->firstR = hd->firstS->firstR->prevR;
                }
                hd->firstS->sizeS--;
                while (hd->firstS->prevS != NULL)
                {
                    hd->firstS = hd->firstS->prevS;
                }
                return hd; // removeu do meio
            }
            // se chegou aqui, está no final
            while (hd->firstS->firstR->prevR != NULL)
            {
                hd->firstS->firstR = hd->firstS->firstR->prevR;
            }
            hd->firstS->lastR = hd->firstS->lastR->prevR;
            free(hd->firstS->lastR->nextR);
            hd->firstS->lastR->nextR = NULL;
            hd->firstS->sizeS--;
            while (hd->firstS->prevS != NULL)
            {
                hd->firstS = hd->firstS->prevS;
            }
            return hd;
        }
        // se chegou aqui, está no começo
        hd->firstS->firstR = hd->firstS->firstR->nextR;
        free(hd->firstS->firstR->prevR);
        hd->firstS->firstR->prevR = NULL;
        return hd;
    }
    else if (hd->firstS->sizeS == 1)
    {
        free(hd->firstS->firstR);
        if (hd->sizeH > 1)
        {
            if (hd->firstS->prevS != NULL) // se não estã no começo
            {
                if (hd->firstS->nextS != NULL) // se não está no final
                {
                    hd->firstS->prevS->nextS = hd->firstS->nextS;
                    hd->firstS->nextS->prevS = hd->firstS->prevS;
                    free(hd->firstS);
                    hd->firstS = hd->lastS;
                    while (hd->firstS->prevS != NULL)
                    {
                        hd->firstS = hd->firstS->prevS;
                    }
                    hd->sizeH--;
                    return hd; // removeu do meio
                }
                // se chegou aqui, está no final
                while (hd->firstS->prevS != NULL)
                {
                    hd->firstS = hd->firstS->prevS;
                }
                hd->firstS = hd->firstS->prevS;
                free(hd->firstS->nextS);
                hd->firstS->nextS = NULL;
                hd->sizeH--;
                return hd;
            }
            // se chegou aqui, está no começo
            hd->firstS = hd->firstS->nextS;
            free(hd->firstS->prevS);
            hd->firstS->prevS = NULL;
            return hd;
        }
        else if (hd->sizeH == 1)
        {
            free (hd->firstS);
            //free(hd);
        }
    }
    return hd;
}
