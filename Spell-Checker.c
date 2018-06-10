
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/timeb.h>
#define MAX_ARRAYSIZE 50
#define MICRO_PER_SECOND 1000000 
// Os 1000000 e so porque quero que os milisegundos aparecam como decimal dos segundos
// o membro tv_use da struct timeval vem em microsegundos

/* =========== COMO FUNCIONA A GERAÇÃO DA CHAVE NA FUNÇÃO HASH ==============
A   -   M   -   O   -   R
65      77      79      82 --->h(481) = 1
R   -   O   -   M   -   A
82      79      77      65  ---->h() = 3
A     -   B   -   C   -   D
65        66      67      68 ---->h() = 404
D     -   C   -   B   -   A
68        67      66      65 ---->h() = 404
*/

//============================================================================
//======================= FUNÇÃO DO NÓ E INICIALIZAÇÃO =======================
//============================================================================


struct No
{
	char palavraNo[30];
	int chaveNo;
	struct No *next;
};

typedef struct No *HashTable [513]; // 513 é o número de buckets

void InicioTabela(HashTable h) // Resetando a tabela para NULL
{
	int i;

	for(i = 0; i < 513; i++)
	{	
		h[i] = NULL;
	}
}

//============================================================================
//======================= FUNÇÃO PARA GRAVAR NO ARQUIVO ======================
//============================================================================

void GravaArquivo(char *pointer2string)
{
   FILE *txt;
   char linhacriada [100];
   int resultGrav;

   txt = fopen("Relatorio.txt", "a"); // o comando 'a' ordena que a linha seja escrito no final do arquivo

   if(txt == NULL) //Se não conseguiu criar retorna NULL
   {
       char ErroCriacao[50] = "Problemas na criação do arquivo";
       puts(ErroCriacao);

        exit(0);
   }

   strcpy(linhacriada, pointer2string);
//    strcat(linhacriada, "\n");

   resultGrav = fputs(linhacriada,txt);


   if(resultGrav == EOF) // Caso não seja executada com sucesso, retorna a constante EOF
    {
        char ErroGrav [50] = "Erro na Gravação";
        puts(ErroGrav);

        exit(0);
    }
    fclose(txt);
}

//============================================================================
//============================ FUNÇÃO DE HASH ================================
//============================================================================

int Hashfunction(char *palavra)
{
    int k = 0, i = 0, sizestring;
    char *pointer2int;
    char integer2str[12]; //string que vai receber o inteiro

    while (palavra[i] != '\n')
    {
        if(palavra[i] - 65 == 0 || palavra[i] - 69 == 0 || palavra[i] - 73 == 0 || palavra[i] - 79 == 0 || palavra[i] - 85 == 0)
	    {
		    k = (i * 7) * palavra[i] + k;
		    i++;
            
	    }

        else
	    {   
		    k = (i * 3)  * palavra[i] + k;		
		    i++;  
        }	
    }

    //printf("\n");

    // sizestring = strlen(palavra) - 2;

    //sprintf(integer2str, "%d", k); // função que transforma int->string (k era sizestring)

    //strcat(integer2str, " ");

     //pointer2int = integer2str;

      //GravaArquivo(pointer2int);

    //k = k % (strlen(palavra)*17);

    //sprintf(integer2str, "%d", k); 

    //strcat(integer2str, "\n");

     //pointer2int = integer2str;

      //GravaArquivo(pointer2int);

return k;
}

//============================================================================
//============== FUNÇÃO INSERIR PALAVRA NA TABELA ============================
//============================================================================

void InserirPalavraTabela(HashTable h,char *palavraCopia)
{
	char word[30];
	
	strcpy(word, palavraCopia);	

	int hash = Hashfunction(word);

	if( h[hash] == NULL)
	{
		h[hash] = (struct No*) malloc ( sizeof(struct No) );
		strcpy( h[hash]->palavraNo, palavraCopia);
		h[hash]->chaveNo = hash; 
		h[hash]->next = NULL;
	}

	else
	{
		struct No* n = h[hash];

		while (n->next != NULL)
			n = n->next;

		n->next = (struct No*) malloc ( sizeof(struct No) );
		strcpy( n->next->palavraNo, palavraCopia);
		h[hash]->chaveNo = hash;
		n->next->next = NULL;

	}
}



//============================================================================
//============================ FUNÇÃO DE LEIADICTIONARY ======================
//============================================================================

void LeiaDictionary()
{
    FILE *txt;
    char linhas [100];
    char *result;
    int cont = 1, key;

    txt = fopen("ascii_noaccent_noduplicates_FIXED_v2.txt", "rt");

    if(txt == NULL) // Se não conseguir abrir o arquivo, retorna NULL
    {
        char ErroLeitura[50] = "Problemas na abertura do arquivo";
        puts(ErroLeitura);
        
        exit(0);
    }

    while(!feof(txt)) // Se não chegar ao final do texto, feof retorna zero.
    {
        result = fgets(linhas, 100, txt);

        //key = Hashfunction(linhas);  //verificar na FUNÇÃO INSERIR PALAVRA NA TABELA

        if(result) // Se não conseguir ler o arquivo, fgets retorna NULL
        {
            //printf("Linha[%d] : %s", cont, linhas);
            //printf("O valor de key = %d\n", key);
            cont++;
        }
    }
    fclose(txt);
}

//============================================================================
//============================ FUNÇÃO DE LEIATEXT ============================
//============================================================================

void LeiaTextoCriado(HashTable h)
{
    FILE *txt;
    char linhas [100];
    char *result;
    int cont = 1, key;

    txt = fopen("TextoCriado.txt", "rt");

    if(txt == NULL) // Se não conseguir abrir o arquivo, retorna NULL
    {
        char ErroLeitura[50] = "Problemas na abertura do arquivo";
        puts(ErroLeitura);
        
        exit(0);
    }

    while(!feof(txt)) // Se não chegar ao final do texto, feof retorna zero.
    {
        result = fgets(linhas, 100, txt);

        ConsultaFrase(h, linhas); 

        if(result) // Se não conseguir ler o arquivo, fgets retorna NULL
        {
            //printf("Linha[%d] : %s", cont, linhas);
            //printf("O valor de key = %d\n", key);
            cont++;
        }
    }
    fclose(txt);
}


//============================================================================
//============================ FUNÇÃO DE CONSULTA ============================
//============================================================================

void ConsultaFrase (HashTable h, char frase){
	int b = 0, falhas = 0;
	b = Hashfunction (frase);

	while (b == h[b]->chaveNo &&  (strcmp(h[b]->palavraNo, frase) != 0) ) 
		h[b] = h[b]->next;

	if(b == h[b]->chaveNo && (strcmp(h[b]->palavraNo, frase) == 0) ) // Caso Sucesso
		return 1;	
	else
	{
		falhas++;
		return 0;

	}
}

//============================================================================
//============================ FUNÇÃO TIMER ==================================
//============================================================================


float Timer(int argc, char* argv[])
{
    struct timeval start_time;
    struct timeval stop_time;
    float time_diff, x;

    gettimeofday( &start_time, NULL );
    //LeiaDictionary(); // qualquer coisa que queiram medir    
    gettimeofday( &stop_time, NULL );

     time_diff = (float)(stop_time.tv_sec - start_time.tv_sec);
     time_diff += (stop_time.tv_usec - start_time.tv_usec)/(float)MICRO_PER_SECOND; 
     printf("\n Funcao executada em %f ms\n", time_diff);
	x = time_diff;

return x;
}

//============================================================================
//====================== FUNÇÃO ImprimeTexto =================================
//============================================================================

//void ImprimeTexto (*s


//===========================================================================
//========================  FUNÇÃO PRINCIPAL ================================
//===========================================================================

void main () {

    char string [MAX_ARRAYSIZE];
    char *ps;

    LeiaDictionary();
	

}
