#ifndef LEXICO_H // Evita inclusão múltipla durante a compilação
#define LEXICO_H

#include <stdio.h>

// Palavras reservadas transformadas em tokens e números
#define TOKEN_EOF          0 // Token de fim de arquivo

// Palavras reservadas
#define TOKEN_INICIO       1 // Token de início
#define TOKEN_FIM          2 // Token de fim
#define TOKEN_INTEIRO      3 // Token para tipo inteiro
#define TOKEN_SE           4 // Token para estrutura de decisão
#define TOKEN_ENTAO        5 // Token para parte verdadeira da decisão
#define TOKEN_SENAO        6 // Token para parte falsa da decisão
#define TOKEN_FIM_SE       7 // Token para fim da estrutura de decisão
#define TOKEN_PARA         8 // Token para estrutura de repetição
#define TOKEN_ATE          9 // Token para limite da repetição
#define TOKEN_PASSO        10 // Token para incremento da repetição
#define TOKEN_FIM_PARA     11 // Token para fim da estrutura de repetição
#define TOKEN_LEIA         12 // Token para leitura de dados
#define TOKEN_IMPRIMA      13 // Token para impressão de dados
#define TOKEN_ESCREVA      14 // Token para escrita de dados
#define TOKEN_E            15 // Token para operador lógico E
#define TOKEN_OU           16 // Token para operador lógico OU
#define TOKEN_NAO          17 // Token para operador lógico NÃO

// Tokens para identificadores e literais
#define TOKEN_IDENT        18   // identificador
#define TOKEN_NUM          19   // constante inteira
#define TOKEN_LITERAL      20   // string entre aspas

// Operadores e pontuação
#define TOKEN_ATRIB        21   // <-
#define TOKEN_IGUAL        22   // =
#define TOKEN_DIFERENTE    23   // <>
#define TOKEN_MENOR        24   // <
#define TOKEN_MAIOR        25   // >
#define TOKEN_MENOR_IGUAL  26   // <=
#define TOKEN_MAIOR_IGUAL  27   // >=
#define TOKEN_MAIS         28   // +
#define TOKEN_MENOS        29   // -
#define TOKEN_MULT         30   // *
#define TOKEN_DIV          31   // /
#define TOKEN_ABRE_PAR     32   // (
#define TOKEN_FECHA_PAR    33   // )
#define TOKEN_PONTO_VIRG   34   // ;
#define TOKEN_DOIS_PONTOS  35   // : 

#define TOKEN_ERRO         -1 // Valor negativo para facilitar a identificação de erros léxicos

// Tabela de símbolos 
#define MAX_SIMBOLOS 200 // Qntd máxima de símbolos
#define MAX_LEXEMA   100 // Tamanho máximo de um lexema

// Símbolo: entrada na tabela de símbolos
typedef struct {
    char lexema[MAX_LEXEMA]; // Texto em si
    int  tipo;               // Código do token (pode ser um dos TOKEN_*)
} Simbolo;

// Palavras reservadas
#define NUM_RESERVADAS 17    // Quantidade de palavras reservadas

// Estrutura para palavras reservadas
typedef struct {
    char palavra[MAX_LEXEMA]; // Texto em si
    int  codigo;
} PalavraReservada;

// Protótipos
void inicializa_lexico(FILE *arquivo);                      // Recebe um arquivo aberto e prepara
int  proximo_token(char *lexema_out, int *pos_tabela);      // Recebe dois ponteiros para saída e retorna o código do token
int  busca_reservada(const char *lex);                      // Recebe o texto do lexema e retorna o código do token se for palavra reservada, ou -1 caso contrário
int  insere_simbolo(const char *lex, int tipo);             // Recebe o texto e o tipo e retorna a posição na tabela

// Variáveis externas
extern Simbolo tabela_simbolos[MAX_SIMBOLOS];
extern int     num_simbolos;
extern int     linha_do_token;

// Auxiliares para validação por linha no main
int  lex_houve_quebra_linha(void);
void lex_limpa_quebra_linha(void);

#endif