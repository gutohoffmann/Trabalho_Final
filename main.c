#include <stdio.h>
#include <stdlib.h>

typedef struct header Header;
typedef struct serie Serie;
typedef struct resistor Resistor;

struct resistor
{
    int value;
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
Resistor* createResistor (Header* hd, int val, float pow, int amt);
Serie* createSerieAndResistor (Header* hd,int ser, int val, float pow, int amt);
Header* createHeader ();
int loadFile(Header* hd);
int saveFile(Header* hd);
Header* addResistor (Header* hd,int ser, int val, float pow, int amt);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void)
{
    Header* h = createHeader();
    loadFile(h);
    printf("tamanho do cabecalho: %d\n", h->sizeH);
    printf("primeira serie: %d\n", h->firstS->serie); // problema com ponteiro de serie
    printf("primeiro res: %d\n", h->firstS->firstR->value);
    printf("ultimo res: %d\n", h->lastS->lastR->value);
    printf("primeiro res ultima serie: %d\n", h->lastS->firstR->value);
    //saveFile(h);

    return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Resistor* createResistor (Header* hd, int val, float pow, int amt)
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
Serie* createSerieAndResistor (Header* hd, int ser, int val, float pow, int amt)
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
    fp = fopen ("lista_de_resistores.txt", "r");
    if (fp == NULL)
    {
        printf ("Erro ao abrir o arquivo.\n");
        return 0;
    }
    Resistor* auxR = (Resistor*) malloc(sizeof(Resistor));
    Serie* auxS = (Serie*) malloc(sizeof(Serie));


    do
    {
        n = fscanf(fp, "%d %d %f %d\n", &(auxS->serie), &(auxR->value), &(auxR->power), &(auxR->amount));
        if (n == EOF) break;
        addResistor(hd, auxS->serie, auxR->value, auxR->power, auxR->amount);
        while (hd->firstS->prevS != NULL)
        {
            while (hd->firstS->firstR->prevR != NULL)
            {
                hd->firstS->firstR = hd->firstS->firstR->prevR;
            }
            hd->firstS = hd->firstS->prevS;
        }
    }
    while(n != EOF);
    fclose (fp);
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int saveFile(Header* hd)
{
    FILE *fp;
    fp = fopen ("lista_de_resistores2.txt", "w");
    if (fp == NULL)
    {
        printf ("Erro ao abrir o arquivo para escrever.\n");
        return 0;
    }

        while (hd->sizeH > 0)
        {
            while (hd->firstS->sizeS > 0)
            {
                fprintf(fp, "%d %d %f %d\n", hd->firstS->serie, hd->firstS->firstR->value, hd->firstS->firstR->power, hd->firstS->firstR->amount);
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
    fclose (fp);
    return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Header* addResistor (Header* hd, int ser, int val, float pow, int amt)
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


    return hd;
}
