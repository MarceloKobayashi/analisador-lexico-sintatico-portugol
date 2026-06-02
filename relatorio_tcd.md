# CAPA

**Centro Universitário de Brasília - UniCEUB**  
**Disciplina:** TCD - Laboratório de Pesquisa de Compiladores  
**Projeto A:** Analisador Léxico para Linguagem Portugol  

**Alunos:** Marcelo Honda Kobayashi, Paulo Henrique Soares Paniago, Dimitri Sofoulis Cinnanti
**Professor:** Leonardo Pol  
**Data:** 5 de junho de 2026  

---

# RESUMO

Este trabalho apresenta a implementação de um analisador léxico para uma linguagem Portugol, desenvolvido em C. O sistema reconhece palavras reservadas, identificadores, números inteiros, literais, operadores e símbolos de pontuação, além de manter uma tabela de símbolos utilizada pelo analisador sintático.

O programa foi estruturado para operar token a token, retornando o código de cada token e a posição na tabela de símbolos quando aplicável. A função principal também simula parte do comportamento sintático, validando o fechamento de parênteses e o uso de ponto e vírgula ao final de comandos de linha única.

# 1. EMBASAMENTO TEÓRICO

## 1.1 Analisador léxico

O analisador léxico é a primeira etapa do processo de compilação. Sua função é percorrer o código-fonte, identificar padrões de caracteres e agrupá-los em unidades significativas chamadas tokens. Cada token representa uma categoria léxica da linguagem, como palavra reservada, identificador, número, literal, operador ou delimitador.

Essa etapa simplifica o trabalho do analisador sintático, pois em vez de processar caracteres isolados, ele passa a receber símbolos já classificados.

## 1.2 Tokens e tabela de símbolos

Um token é a representação simbólica de um lexema reconhecido pelo compilador. Em Portugol, exemplos típicos incluem `inicio`, `fim`, `se`, `entao`, `para`, `leia`, `imprima`, identificadores como `a` e `contador`, números inteiros e literais entre aspas.

A tabela de símbolos armazena os lexemas que precisam ser reutilizados pelo compilador. No projeto, ela guarda identificadores, números e literais, registrando o texto do lexema e o tipo correspondente. Quando o mesmo lexema aparece novamente, sua posição é reutilizada.

## 1.3 Palavras reservadas

Palavras reservadas são lexemas com significado fixo na linguagem. Para facilitar a implementação, foi criada uma tabela própria de palavras reservadas. Ao encontrar um identificador, o léxico consulta essa tabela: se houver correspondência, o token retornado é o da palavra reservada; caso contrário, o lexema é tratado como identificador.

## 1.4 Retorno token a token

O analisador léxico foi desenvolvido para funcionar sob demanda. Em vez de processar todo o arquivo de uma vez, cada chamada retorna apenas o próximo token disponível. Esse modelo é o mais adequado para integração com o analisador sintático.

## 1.5 Tratamento de erros

O projeto reconhece erros léxicos e sintáticos simples. Entre os erros léxicos estão strings sem aspas de fechamento e caracteres inválidos. Entre os erros sintáticos, o `main` simula o parser verificando parênteses, comandos de linha única e regras de uso de `;`.

# 2. OBJETIVO DO PROJETO

O objetivo é implementar um analisador léxico em Portugol que:

- identifique tokens da linguagem;
- construa e utilize uma tabela de símbolos;
- reconheça palavras reservadas com código único;
- retorne token e posição na tabela;
- considere token especial de fim de arquivo;
- simule parte do comportamento sintático no `main`.

# 3. DESCRIÇÃO DA IMPLEMENTAÇÃO

## 3.1 Estrutura geral

O projeto está organizado em três arquivos principais:

- `lexico.h`: define tokens, estruturas e protótipos;
- `lexico.c`: implementa a lógica de tokenização e tabela de símbolos;
- `main.c`: simula o analisador sintático e imprime os tokens reconhecidos.

## 3.2 Funcionamento do léxico

A função `proximo_token` lê o arquivo caractere por caractere e classifica o lexema encontrado. O reconhecimento cobre:

- identificadores;
- números inteiros;
- literais entre aspas;
- operadores aritméticos;
- operadores relacionais;
- operador de atribuição;
- parênteses;
- ponto e vírgula;
- dois pontos.

Quando o lexema é um identificador, o sistema verifica se ele pertence à tabela de palavras reservadas. Se sim, o token é retornado como reservada; se não, o lexema é inserido na tabela de símbolos.

## 3.3 Simulação do sintático no `main`

A função `main` chama o léxico sucessivamente e imprime o token encontrado e sua posição na tabela. Além disso, ela valida regras simples de estrutura, como:

- fechamento de parênteses;
- exigência de `;` ao final de comandos de linha única;
- isenção de `;` em linhas como `inicio`, `fim`, `se`, `entao`, `senao`, `fim_se`, `para`, `passo` e `fim_para`.

# 4. EXEMPLOS DE USO

## 4.1 Compilação

```powershell
gcc main.c lexico.c -o lexico.exe;
```

## 4.2 Execução

```powershell
.\lexico.exe teste_2.por;
```

## 4.3 Exemplo de programa aceito

```portugol
inicio
inteiro: b;
inteiro: c;
imprima("digite um valor para c:");
leia(c);
para b=0 ate c passo 2
imprima(b);
fim_para
fim
```

# 5. RESULTADOS

O programa produzido reconhece os principais elementos léxicos da linguagem Portugol, mantendo uma tabela de símbolos e imprimindo token e posição ao longo da análise. A implementação também realiza validações básicas de estrutura, o que permite simular o comportamento do analisador sintático em nível introdutório.

# 6. CONCLUSÃO

O trabalho atende à proposta de implementação de um analisador léxico para Portugol em C. O projeto reconhece palavras reservadas, identificadores, literais, números, operadores e pontuação, além de manter a tabela de símbolos e tratar erros léxicos e sintáticos básicos.

A estrutura separada entre `lexico.c`, `lexico.h` e `main.c` facilita a manutenção e futuras extensões, como a implementação de um analisador sintático completo.

# APÊNDICE A - CÓDIGO-FONTE

O código-fonte completo do trabalho está disponível nos arquivos do workspace:

- [lexico.h](lexico.h)
- [lexico.c](lexico.c)
- [main.c](main.c)

Esses arquivos correspondem à implementação final do analisador léxico e da simulação sintática exigida pelo enunciado.
