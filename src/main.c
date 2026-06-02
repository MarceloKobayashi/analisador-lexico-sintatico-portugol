#include <stdio.h>
#include <stdlib.h>
#include "../include/lexico.h" // Traz os códigos de token, a estrutura de símbolo e as funções do léxico

// Função para converter o código de token em texto
static const char *nome_token(int codigo) {
    switch (codigo) {
        case TOKEN_EOF:         return "EOF";
        case TOKEN_INICIO:      return "INICIO";
        case TOKEN_FIM:         return "FIM";
        case TOKEN_INTEIRO:     return "INTEIRO";
        case TOKEN_SE:          return "SE";
        case TOKEN_ENTAO:       return "ENTAO";
        case TOKEN_SENAO:       return "SENAO";
        case TOKEN_FIM_SE:      return "FIM_SE";
        case TOKEN_PARA:        return "PARA";
        case TOKEN_ATE:         return "ATE";
        case TOKEN_PASSO:       return "PASSO";
        case TOKEN_FIM_PARA:    return "FIM_PARA";
        case TOKEN_LEIA:        return "LEIA";
        case TOKEN_IMPRIMA:     return "IMPRIMA";
        case TOKEN_ESCREVA:     return "ESCREVA";
        case TOKEN_E:           return "E";
        case TOKEN_OU:          return "OU";
        case TOKEN_NAO:         return "NAO";
        case TOKEN_IDENT:       return "IDENTIFICADOR";
        case TOKEN_NUM:         return "NUMERO";
        case TOKEN_LITERAL:     return "LITERAL";
        case TOKEN_ATRIB:       return "ATRIBUICAO";
        case TOKEN_IGUAL:       return "IGUAL";
        case TOKEN_DIFERENTE:   return "DIFERENTE";
        case TOKEN_MENOR:       return "MENOR";
        case TOKEN_MAIOR:       return "MAIOR";
        case TOKEN_MENOR_IGUAL: return "MENOR_IGUAL";
        case TOKEN_MAIOR_IGUAL: return "MAIOR_IGUAL";
        case TOKEN_MAIS:        return "MAIS";
        case TOKEN_MENOS:       return "MENOS";
        case TOKEN_MULT:        return "MULT";
        case TOKEN_DIV:         return "DIV";
        case TOKEN_ABRE_PAR:    return "ABRE_PAR";
        case TOKEN_FECHA_PAR:   return "FECHA_PAR";
        case TOKEN_PONTO_VIRG:  return "PONTO_VIRG";
        case TOKEN_DOIS_PONTOS: return "DOIS_PONTOS";
        case TOKEN_ERRO:        return "ERRO";
        default:                return "DESCONHECIDO";
    }
}

// Função para saber se um token precisa de ';'
static int linha_exige_ponto_virgula(int token) {
    return token == TOKEN_INTEIRO || token == TOKEN_LEIA || token == TOKEN_IMPRIMA ||
           token == TOKEN_ESCREVA || token == TOKEN_IDENT || token == TOKEN_NUM ||
           token == TOKEN_LITERAL || token == TOKEN_ABRE_PAR || token == TOKEN_ATRIB ||
           token == TOKEN_IGUAL || token == TOKEN_DIFERENTE || token == TOKEN_MENOR ||
           token == TOKEN_MAIOR || token == TOKEN_MENOR_IGUAL || token == TOKEN_MAIOR_IGUAL ||
           token == TOKEN_MAIS || token == TOKEN_MENOS || token == TOKEN_MULT ||
           token == TOKEN_DIV || token == TOKEN_DOIS_PONTOS || token == TOKEN_E ||
           token == TOKEN_OU || token == TOKEN_NAO || token == TOKEN_ATE;
}

// Função para saber se um token é isento de ';'
static int token_exempta_linha(int token) {
    return token == TOKEN_INICIO || token == TOKEN_FIM || token == TOKEN_SE ||
           token == TOKEN_ENTAO || token == TOKEN_SENAO || token == TOKEN_FIM_SE ||
           token == TOKEN_PARA || token == TOKEN_PASSO || token == TOKEN_FIM_PARA;
}

// Função principal: argc é o número de argumentos e argv é um array de strings com os argumentos
int main(int argc, char *argv[]) {
    FILE *arquivo;
    int   token, pos;
    char  lexema[MAX_LEXEMA];
    int   balance_par = 0;                  // Parenteses
    int   linha_tem_token = 0;              // Se a linha tem um token que exige ';'
    int   linha_tem_ponto_virgula = 0;      // Se a linha tem um ';'
    int   linha_exempta = 0;                // Se a linha é isenta de ';'

    if (argc < 2) {
        printf("Uso: %s <arquivo.por>\n", argv[0]);
        return 1;
    }

    arquivo = fopen(argv[1], "r");
    if (!arquivo) {
        printf("Erro: nao foi possivel abrir o arquivo '%s'\n", argv[1]);
        return 1;
    }

    inicializa_lexico(arquivo);

    printf("%-20s %-16s %s\n", "LEXEMA", "TOKEN", "POS.TABELA");
    printf("%-20s %-16s %s\n", "--------------------",
                               "----------------", "----------");

    do {
        token = proximo_token(lexema, &pos);

        // Se der erro, quebra
        if (token == TOKEN_ERRO) {
            printf("%s\n", lexema);
            break;
        }

        // Se houve quebra de linha, verifica se a linha anterior estava correta
        if (lex_houve_quebra_linha()) {
            if (linha_tem_token) {
                if (balance_par > 0) {
                    printf("erro sintatico: esperado ')'\n");
                    break;
                }
                if (linha_exempta && linha_tem_ponto_virgula) {
                    printf("erro sintatico: ';' nao permitido\n");
                    break;
                }
                if (!linha_exempta && !linha_tem_ponto_virgula) {
                    printf("erro sintatico: esperado ';'\n");
                    break;
                }
            }

            // Reseta os indicadores para a nova linha
            linha_tem_token = 0;
            linha_tem_ponto_virgula = 0;
            linha_exempta = 0;
            lex_limpa_quebra_linha();
        }

        // Valida os parênteses
        if (token == TOKEN_ABRE_PAR) {
            balance_par++;
        } else if (token == TOKEN_FECHA_PAR) {
            if (balance_par <= 0) {
                printf("erro sintatico: esperado ')'\n");
                break;
            }
            balance_par--;
        }

        if (token == TOKEN_PONTO_VIRG) {
            if (linha_exempta) {
                printf("erro sintatico: ';' nao permitido\n");
                break;
            }
            linha_tem_ponto_virgula = 1;
        }

        if (token_exempta_linha(token)) {
            linha_exempta = 1;
        } else if (linha_exige_ponto_virgula(token)) {
            linha_tem_token = 1;
        }

        // Se der erro
        if (token == TOKEN_EOF) {
            if (balance_par > 0) {
                printf("erro sintatico: esperado ')'\n");
                break;
            }
            if (linha_tem_token) {
                if (linha_exempta && linha_tem_ponto_virgula) {
                    printf("erro sintatico: ';' nao permitido\n");
                    break;
                }
                if (!linha_exempta && !linha_tem_ponto_virgula) {
                    printf("erro sintatico: esperado ';'\n");
                    break;
                }
            }
            if (linha_exempta && linha_tem_ponto_virgula) {
                printf("erro sintatico: ';' nao permitido\n");
                break;
            }
            if (!linha_exempta && linha_tem_token && !linha_tem_ponto_virgula) {
                printf("erro sintatico: esperado ';'\n");
                break;
            }
        }

        linha_tem_token = 1;

        if (pos >= 0)
            printf("%-20s %-16s %d\n", lexema, nome_token(token), pos);
        else
            printf("%-20s %-16s -\n", lexema, nome_token(token));

    } while (token != TOKEN_EOF && token != TOKEN_ERRO);

    // Se houver símbolos na tabela, imprime a tabela de símbolos
    if (num_simbolos > 0) {
        printf("\n--- TABELA DE SIMBOLOS ---\n");
        printf("%-6s %-20s %s\n", "POS", "LEXEMA", "TIPO");
        printf("%-6s %-20s %s\n", "------",
                                  "--------------------", "--------");
        // Percorre a tabela de símbolos e imprime a posição, o lexema e o tipo do token
        int i;
        for (i = 0; i < num_simbolos; i++)
            printf("%-6d %-20s %s\n", i,
                   tabela_simbolos[i].lexema,
                   nome_token(tabela_simbolos[i].tipo));
    }

    // Fecha o arquivo e encerra o programa
    fclose(arquivo);
    return 0;
}

/*
main.c
    │
    ├── recebe nome do arquivo via argv[1]
    │
    ├── abre o arquivo com fopen
    │
    ├── inicializa_lexico(arquivo)
    │
    ├── imprime cabeçalho
    │
    ├── LOOP ──► chama proximo_token
    │               │
    │               ├── pos >= 0 ──► imprime lexema + token + posição
    │               └── pos < 0  ──► imprime lexema + token + "-"
    │
    ├── até TOKEN_EOF ou TOKEN_ERRO
    │
    ├── imprime tabela de símbolos
    │
    └── fclose + return 0
*/