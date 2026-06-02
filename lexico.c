#include "lexico.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Define a tabela de símbolos
Simbolo tabela_simbolos[MAX_SIMBOLOS];
int     num_simbolos = 0;

// Arquivo fonte
static FILE *fonte = NULL;
static int   char_atual = ' ';
static int   linha_atual = 1;
static int   houve_quebra_linha = 0;

int linha_do_token = 1;

// Monta a tabela de palavras reservadas
static PalavraReservada reservadas[NUM_RESERVADAS] = {
    {"inicio",   TOKEN_INICIO},
    {"fim",      TOKEN_FIM},
    {"inteiro",  TOKEN_INTEIRO},
    {"se",       TOKEN_SE},
    {"entao",    TOKEN_ENTAO},
    {"senao",    TOKEN_SENAO},
    {"fim_se",   TOKEN_FIM_SE},
    {"para",     TOKEN_PARA},
    {"ate",      TOKEN_ATE},
    {"passo",    TOKEN_PASSO},
    {"fim_para", TOKEN_FIM_PARA},
    {"leia",     TOKEN_LEIA},
    {"imprima",  TOKEN_IMPRIMA},
    {"escreva",  TOKEN_ESCREVA},
    {"e",        TOKEN_E},
    {"ou",       TOKEN_OU},
    {"nao",      TOKEN_NAO}
};

// Prepara o analisador léxico
void inicializa_lexico(FILE *arquivo) {
    fonte      = arquivo;       // Armazena o ponteiro do arquivo
    num_simbolos = 0;           // Zera o contador de símbolos
    linha_atual = 1;            // Inicializa a linha atual
    houve_quebra_linha = 0;     // Reseta o indicador de quebra de linha
    linha_do_token = 1;         // Inicializa a linha do token
    char_atual = fgetc(fonte);  // Lê o primeiro caractere e guarda na variável global
}

// Função para saber se houve quebra de linha desde o último token
int lex_houve_quebra_linha(void) {
    return houve_quebra_linha;
}

// Limpa o indicador de quebra de linha
void lex_limpa_quebra_linha(void) {
    houve_quebra_linha = 0;
}

// Percorre a tabela de palavras reservadas para verificar se o lexema é uma delas
int busca_reservada(const char *lex) {
    int i;      // Define um counter
    for (i = 0; i < NUM_RESERVADAS; i++) {  // Percorre a tabela de palavras reservadas
        if (strcmp(reservadas[i].palavra, lex) == 0)    // Compara o lexema com a palavra
            return reservadas[i].codigo;                // Se for igual, retorna o código do token
    }
    return -1;      // Senão, retorna -1 para indicar que não é uma palavra reservada
}

// Insere um símbolo na tabela de símbolos
int insere_simbolo(const char *lex, int tipo) {
    int i;      // Define um counter
    for (i = 0; i < num_simbolos; i++) {                    // Percorre a tabela de símbolos
        if (strcmp(tabela_simbolos[i].lexema, lex) == 0)    // Compara o lexema com o símbolo
            return i;   // Se existir, retorna a posição do símbolo.
    }
    // Se não existir:
    if (num_simbolos < MAX_SIMBOLOS) {             // Verifica se há espaço na tabela   
        strncpy(tabela_simbolos[num_simbolos].lexema, lex, MAX_LEXEMA - 1); // Copia o lexema para a tabela, garantindo que não ultrapasse o tamanho máximo
        tabela_simbolos[num_simbolos].tipo = tipo;      // Armazena o tipo do token
        return num_simbolos++;                          // Retorna a posição do símbolo e depois incrementa o contador
    }
    // Se a tabela estiver cheia, retorna -1 para indicar erro
    return -1;
}


int proximo_token(char *lexema_out, int *pos_tabela) {
    char lexema[MAX_LEXEMA];  // Define um buffer para armazenar o lexema do token
    int  i = 0;     // Define um counter
    *pos_tabela = -1;   // Inicializa a posição na tabela como -1 (indica que não é um símbolo da tabela por padrão)

    // Descarta espaços e tabs enquanto não for o fim do arquivo
    while (char_atual != EOF && isspace(char_atual)) {
        if (char_atual == '\n') {
            linha_atual++;
            houve_quebra_linha = 1;
        }
        char_atual = fgetc(fonte);
    }

    linha_do_token = linha_atual;

    // Se for o fim do arquivo, retorna o token especial de fim
    if (char_atual == EOF) {
        strcpy(lexema_out, "EOF");
        return TOKEN_EOF;
    }

    // Se o identificador começar com letra ou '_' insere ele na tabela se possível e retorna o código do token
    if (isalpha(char_atual) || char_atual == '_') {
        while ((isalnum(char_atual) || char_atual == '_') && i < MAX_LEXEMA - 1) {  // Enquanto for dígio ou letra ou '_' e não ultrapassar o tamanho máximo do lexema
            lexema[i++] = tolower(char_atual);  // Converte para minúscula
            char_atual  = fgetc(fonte);         // Lê o próximo caractere
        }
        lexema[i] = '\0';               // Finaliza a string do lexema
        strcpy(lexema_out, lexema);     // Copia o lexema para a saída

        int cod = busca_reservada(lexema);      // Verifica se é uma palavra reservada
        if (cod != -1)                          // Se for, retorna o código do token
            return cod;

        *pos_tabela = insere_simbolo(lexema, TOKEN_IDENT);      // Senão, insere na tabela de símbolos como identificador
        return TOKEN_IDENT;                                     // Retorna o código do token
    }

    // Se o identificador começar com dígito, insere ele na tabela e retorna o código do token
    if (isdigit(char_atual)) {
        while (isdigit(char_atual) && i < MAX_LEXEMA - 1) {
            lexema[i++] = char_atual;
            char_atual  = fgetc(fonte);
        }
        lexema[i] = '\0';
        strcpy(lexema_out, lexema);
        *pos_tabela = insere_simbolo(lexema, TOKEN_NUM);
        return TOKEN_NUM;
    }

    // String literal
    if (char_atual == '"') {
        // Tira as aspas e lê o conteúdo até achar outra
        char_atual = fgetc(fonte);
        while (char_atual != '"' && char_atual != EOF && i < MAX_LEXEMA - 1) {
            lexema[i++] = char_atual;
            char_atual  = fgetc(fonte);
        }
        /* se atingimos EOF antes da aspas final => erro léxico */
        if (char_atual == EOF) {
            strcpy(lexema_out, "Erro lexico: string nao terminada");
            return TOKEN_ERRO;
        }
        /* consome a aspas final */
        if (char_atual == '"')
            char_atual = fgetc(fonte);
        lexema[i] = '\0';
        strcpy(lexema_out, lexema);
        *pos_tabela = insere_simbolo(lexema, TOKEN_LITERAL);
        return TOKEN_LITERAL;
    }

    // Operadores e símbolos
    switch (char_atual) {

        // Se for '<' pode significar 4 coisas: '<', '<=', '<>' ou '<-'
        case '<':
            lexema[i++] = char_atual;
            char_atual  = fgetc(fonte);
            if (char_atual == '-') {            // <- atribuição
                lexema[i++] = char_atual;
                char_atual  = fgetc(fonte);
                lexema[i]   = '\0';
                strcpy(lexema_out, lexema);
                return TOKEN_ATRIB;
            }
            if (char_atual == '=') {            // <= menor ou igual
                lexema[i++] = char_atual;
                char_atual  = fgetc(fonte);
                lexema[i]   = '\0';
                strcpy(lexema_out, lexema);
                return TOKEN_MENOR_IGUAL;
            }
            if (char_atual == '>') {            // <> diferente
                lexema[i++] = char_atual;
                char_atual  = fgetc(fonte);
                lexema[i]   = '\0';
                strcpy(lexema_out, lexema);
                return TOKEN_DIFERENTE;
            }
            lexema[i] = '\0';                   // < menor
            strcpy(lexema_out, lexema);
            return TOKEN_MENOR;

        // Se for '>' pode significar 2 coisas: '>' ou '>='
        case '>':
            lexema[i++] = char_atual;
            char_atual  = fgetc(fonte);
            if (char_atual == '=') {            // >= maior ou igual
                lexema[i++] = char_atual;
                char_atual  = fgetc(fonte);
                lexema[i]   = '\0';
                strcpy(lexema_out, lexema);
                return TOKEN_MAIOR_IGUAL;
            }
            lexema[i] = '\0';                   // > maior
            strcpy(lexema_out, lexema);
            return TOKEN_MAIOR;

        // Se for '='
        case '=':
            lexema[0] = '='; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_IGUAL;

        // Se for '+'
        case '+':
            lexema[0] = '+'; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_MAIS;

        // Se for '-'
        case '-':
            lexema[0] = '-'; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_MENOS;

        // Se for '*'
        case '*':
            lexema[0] = '*'; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_MULT;

        // Se for '/'
        case '/':
            lexema[0] = '/'; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_DIV;

        // Se for '('
        case '(':
            lexema[0] = '('; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_ABRE_PAR;

        // Se for ')'
        case ')':
            lexema[0] = ')'; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_FECHA_PAR;

        // Se for ';'
        case ';':
            lexema[0] = ';'; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_PONTO_VIRG;

        // Se for ':'
        case ':':
            lexema[0] = ':'; lexema[1] = '\0';
            strcpy(lexema_out, lexema);
            char_atual = fgetc(fonte);
            return TOKEN_DOIS_PONTOS;

        // Se não for nenhum, retorna um token de erro
        default:
            lexema[0] = char_atual;
            lexema[1] = '\0';
            snprintf(lexema_out, MAX_LEXEMA, "Erro lexico: caractere invalido '%c'", char_atual);
            char_atual = fgetc(fonte);
            return TOKEN_ERRO;
    }
}

/*  
   chamada de proximo_token
        │
        ▼
   pula espaços
        │
        ▼
   char_atual == EOF? ──► TOKEN_EOF
        │
        ▼
   é letra ou _? ──► lê identificador ──► é reservada? ──► retorna código reservada
        │                                       │
        │                                       └── não ──► insere tabela ──► TOKEN_IDENT
        ▼
   é dígito? ──► lê número ──► insere tabela ──► TOKEN_NUM
        │
        ▼
   é aspas? ──► lê até fechar aspas ──► insere tabela ──► TOKEN_LITERAL
        │
        ▼
   é operador? ──► analisa com lookahead ──► retorna token correspondente
        │
        ▼
   nenhum caso ──► TOKEN_ERRO 
*/