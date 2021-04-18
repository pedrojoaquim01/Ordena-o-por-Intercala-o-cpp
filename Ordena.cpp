#include <stdio.h>
#include <cstring>
#include <iostream>
#include <math.h>
#include <cmath>
using namespace std;

typedef struct _Endereco Endereco;

struct _Endereco
{
    char logradouro[72];
    char bairro[72];
    char cidade[72];
    char uf[72];
    char sigla[2];
    char cep[8];
    char lixo[2];
};

void intercala(char *parte1, char *parte2, char *saida)
{
    FILE      *p1, *p2;
    Endereco  e1, e2;
    FILE      *s;

    p1 = fopen(parte1, "r");                p2 = fopen(parte2, "r");
    fread(&e1, sizeof(Endereco), 1, p1);    fread(&e2, sizeof(Endereco), 1, p2);

    s = fopen(saida, "w");


    while (!feof(p1) && !feof(p2))
    {   
        
        if (strncmp((&e1)->cep, (&e2)->cep, 8) > 0)
        {
            fwrite(&e2, sizeof(Endereco), 1, s);
            fread(&e2, sizeof(Endereco), 1, p2);
        }
        else
        {
            fwrite(&e1, sizeof(Endereco), 1, s);
            fread(&e1, sizeof(Endereco), 1, p1);
        }
    }   
        while (!feof(p2))
        {
            fwrite(&e2, sizeof(Endereco), 1, s);
            fread(&e2, sizeof(Endereco), 1, p2);
        }
        while (!feof(p1))
        {
            fwrite(&e1, sizeof(Endereco), 1, s);
            fread(&e1, sizeof(Endereco), 1, p1);
        }
    fclose(p1); fclose(p2); fclose(s);
}

int comp(const void *f1, const void *f2)
{
    return strncmp(((Endereco *)f1)->cep, ((Endereco *)f2)->cep, 8);
    
}

int main(int argc, char**argv)
{
    FILE *f, *g;
    Endereco *e;
    long primeiro, meio, ultimo, tamanho, posicao;

    f = fopen("cep.dat", "r");
    fseek(f, 0, SEEK_END);
    tamanho = ftell(f);
    rewind(f); // fseek(f,0,SEEK_SET);
    printf("\nTamanho do Arquivo: %ld\n", tamanho);
    printf("Tamanho do Registro: %ld\n", sizeof(Endereco));
    printf("Tamanho do Arquivo em Registros: %ld\n \n", tamanho / sizeof(Endereco));
    primeiro = 0;
    ultimo = (tamanho / sizeof(Endereco)) - 1;
    meio = (primeiro + ultimo) / 2;
    posicao = -1;

    int count = 0;
    int blocos = 16;

    long regBloco = ceil(ultimo / blocos);

    rewind(f);
    e = (Endereco *)malloc(regBloco * sizeof(Endereco));
    for (int i = 0; i < blocos; i++)
    { 
        qsort(e, regBloco, sizeof(Endereco), comp);
        char nome[16];
        sprintf(nome, "CepDisc_%d.dat", i);
        g = fopen(nome, "w"); 

        fwrite(e, sizeof(Endereco), regBloco, g);
        fclose(g);
        count++;
    }
    free(e);
    fclose(f);

    int aux;
    int index = 0;
    for (int i = log2(blocos); i > 0; i--)
    {
        
	    printf("Junta %d blocos em ", blocos);
        aux = count; blocos = blocos / 2;
        printf("%d \n", blocos);
        for (int i = 0; i < blocos; i++)
        {
            char parte1[25];
            sprintf(parte1, "CepDisc_%d.dat", index++); 

             printf("Intercala o bloco %d com ", index);

            char parte2[25];
            sprintf(parte2, "CepDisc_%d.dat", index++);

             printf("%d \n", index);

            char saida[25];
            sprintf(saida , "CepDisc_%d.dat", count++);

            intercala(parte1, parte2, saida);
            remove(parte1); remove(parte2);
            if (i+1 >= blocos)
            {
                index = aux;
            }
        }
        printf("\n\n");
    }

    return 0;
}
