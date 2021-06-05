/*
    UNIVERSIDADE ESTADUAL DA PARAÍBA-UEPB
    CAMPUS: CAMPINA GRANDE-PB
    CENTRO: CENTRO DE CIÊNCIAS E TECNOLOGIA-CCT
    DEPARTAMENTO: DEPARTAMENTO DE COMPUTAÇÃO-DC
    DISCIPLINA: LINGUAGEM DE PROGRAMAÇÃO 1-LP1
    PROFESSOR: HERON ARAGÃO MONTEIRO
    ALUNOS: MATEUS HENRIQUE SALES, MATEUS CORDEIRO VIANA, LUCAS SANTOS ANDRADE, MARCELO VICTOR A. PEDROSA,
     ARTON DE LIMA MÉLO, FRANCISCO WELIO FIRMINO DA SILVA JUNIOR, JOÃO ANTONIO DE LIMA REIS, LUDMILA MARIA
     PEREIRA DA SILVA, LUANA DOS SANTOS LIBERATO
    DATA: JUNHO DE 2021
*/

//--------------------------PROJETO DE PROGRAMAÇÃO 3-----------------------------//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <time.h>

typedef struct tempo
{
    int tm_sec; //representa os segundos de 0 a 59
    int tm_min; //representa os minutos de 0 a 59
    int tm_hour; //representa as horas de 0 a 24
    int tm_mday; //dia do mês de 1 a 31
    int tm_mon; //representa os meses do ano de 0 a 11
    int tm_year; //representa o ano a partir de 1900
} tempo;

typedef struct
{
    FILE *cadastroCliente, *cadastroContaCorrente, *registroAtendimento;
} ArquivosBanco;

typedef struct
{
    char cpf[15], telefone[15], nome[120], endereco[120];
    int idade;
} infoCliente;

typedef struct
{
    char cpf[15];
    char conta[15];
    double valorAtual;
} contaCliente;

typedef struct
{
    time_t seconds;
    char operacao, conta[15];
    double valor;
    int status;
} registroDiario;

typedef struct
{
    char conta[15], nome[120];
    time_t horaDeRegistro;
    int idade, tempoDeEspera;
} dadosFila;

typedef enum {CPF, CONTA, CONTA_CPF} dados;
typedef enum {STRING, NUMBER, ALPHANUM} typeCheck;

void clrscr(void)
{
    system("@cls||clear");
}

void menu(void);
int DataCheck(FILE *, char *, dados);
int checkTheChars(char *, int, typeCheck);
contaCliente DadosDaConta(char *,  FILE *);
infoCliente DadosCliente(char *, FILE *);
void ordeneFila(dadosFila *, int);

int lowSize = 15, bigSize = 120, sizeForFile = 200;

int main()
{
    setlocale(LC_ALL, "Portuguese");

    int posicao = 0, printar;
    ArquivosBanco arquivos;
    dadosFila * fila;

    fila = calloc(1, sizeof(dadosFila));

    while(1)
    {
        int opcao = 0;
        // PARECE QUE O ARQUIVO ESTÁ SE CRIANDO SOZINHO QUANDO É APAGADO, MAS NÃO CONTÉM MUITO COM ISSO JÁ DEIXEM TUDO PRONTO
        menu();

        printf("Código: ");
        scanf(" %d", &opcao);
        fflush(stdin);

        clrscr();
        switch(opcao)
        {
        case 1:  //CADASTRO DO CLIENTE
        {
            arquivos.cadastroCliente = fopen("dadosdocliente.txt", "a+b");

            if(arquivos.cadastroCliente != NULL)
            {
                infoCliente cliente;
                int checagemCondicoes = 0, saia = 1;

                do
                {
                    if(!saia)
                        break;
                    if(checagemCondicoes)
                        printf("\nTIPO INCORRETO ENCONTRADO // NOME MENOR QUE TRÊS DIGITOS // SEM PONTUAÇÃO!\n\a\n");

                    printf("Digite o nome completo (para voltar digite \"EXIT\"): ");
                    fgets(cliente.nome, bigSize, stdin);
                    fflush(stdin);

                    checagemCondicoes = checkTheChars(cliente.nome, strlen(cliente.nome), STRING) || strlen(cliente.nome) <= 2;
                    saia = strcmp(cliente.nome, "EXIT\n");
                }
                while(checagemCondicoes);

                checagemCondicoes = 1;
                do
                {
                    if(!saia)
                        break;
                    if(!checagemCondicoes)
                        printf("\nTIPO INCORRETO ENCONTRADO!\n\a\n");

                    printf("Digite a idade: ");

                    checagemCondicoes = scanf(" %d", &cliente.idade);
                    fflush(stdin);
                }
                while(!checagemCondicoes || cliente.idade <= 0);

                checagemCondicoes = 0;
                do
                {
                    if(!saia)
                        break;
                    if(checagemCondicoes)
                        printf("\nTIPO INCORRETO ENCONTRADO // CPF JÁ UTILIZADO // TAMANHO DO CPF INVÁLIDO // SEM PONTUAÇÃO!\n\a\n");

                    rewind(arquivos.cadastroCliente);

                    printf("Digite o CPF (sendo ele composto por 11 números): ");
                    fgets(cliente.cpf, lowSize, stdin);
                    fflush(stdin);

                    checagemCondicoes = checkTheChars(cliente.cpf, strlen(cliente.cpf), NUMBER) || DataCheck(arquivos.cadastroCliente, cliente.cpf, CPF) || strlen(cliente.cpf) != 12;
                }
                while(checagemCondicoes);

                do
                {
                    if(!saia)
                        break;
                    if(checagemCondicoes)
                        printf("\nTIPO INCORRETO ENCONTRADO // TAMANHO INSUFICIENTE PARA UM ENDEREÇO // SEM PONTUAÇÃO!\n\a\n");

                    printf("Digite o endereço (ex.: Rua riachuelo 100 liberdade): ");
                    fgets(cliente.endereco, bigSize, stdin);
                    fflush(stdin);

                    checagemCondicoes = checkTheChars(cliente.endereco, strlen(cliente.endereco), ALPHANUM) || strlen(cliente.endereco) < 12;
                }
                while(checagemCondicoes);

                do
                {
                    if(!saia)
                        break;
                    if(checagemCondicoes)
                        printf("\nTIPO INCORRETO ENCONTRADO!\n\a\n");

                    printf("Digite o número de telefone(ex.: 558388282518): ");
                    fgets(cliente.telefone, lowSize, stdin);
                    fflush(stdin);

                    checagemCondicoes = checkTheChars(cliente.telefone, strlen(cliente.telefone), NUMBER) || strlen(cliente.telefone) < 8;
                }
                while(checagemCondicoes);

                if(saia)
                    fwrite(&cliente, sizeof(infoCliente), 1, arquivos.cadastroCliente);

                fclose(arquivos.cadastroCliente);
            }
            else
            {
                printf("Arquivo não encontrado (dadosdocliente.txt)!\n");
            }
            system("pause");
            clrscr();
            break;
        }
        case 2:  // CADASTRO DE CONTA CORRENTE
        {
            arquivos.cadastroCliente = fopen("dadosdocliente.txt", "rb");
            arquivos.cadastroContaCorrente = fopen("cadastrocc.txt", "a+b");

            if(arquivos.cadastroContaCorrente != NULL && arquivos.cadastroCliente != NULL)
            {
                //OPÇAO 2
                int checagemCondicoes = 0, checagemCondicoesAdicionais = 1, checagemCondicoesAdicionais2 = 0, saia = 1;
                contaCliente dados;

                do
                {
                    if(!saia)
                        break;
                    if(checagemCondicoes || !checagemCondicoesAdicionais || checagemCondicoesAdicionais2)
                        printf("\nTIPO INCORRETO ENCONTRADO // SEM PONTUAÇÃO //CPF NÃO ENCONTRADO // CONTA COM CPF ESPECIFICADO JÁ EXISTE // TAMANHO DO CPF INVÁLIDO!\n\a\n");

                    rewind(arquivos.cadastroCliente);
                    rewind(arquivos.cadastroContaCorrente);

                    printf("Digite o CPF do correntista(sendo ele composto por 11 números ou para sair digite \"EXIT\"): ");
                    fgets(dados.cpf, lowSize, stdin);
                    fflush(stdin);

                    saia = strcmp(dados.cpf, "EXIT\n");

                    checagemCondicoes = checkTheChars(dados.cpf, strlen(dados.cpf), NUMBER);
                    checagemCondicoesAdicionais = DataCheck(arquivos.cadastroCliente, dados.cpf, CPF);
                    checagemCondicoesAdicionais2 = DataCheck(arquivos.cadastroContaCorrente, dados.cpf, CONTA_CPF);
                }
                while(checagemCondicoes || !checagemCondicoesAdicionais || checagemCondicoesAdicionais2 || strlen(dados.cpf) != 12);

                checagemCondicoes = 0, checagemCondicoesAdicionais = 0;

                do
                {
                    if(!saia)
                        break;
                    if(checagemCondicoes || checagemCondicoesAdicionais)
                        printf("\nTIPO INCORRETO ENCONTRADO // SEM PONTUAÇÃO // CONTA JÁ CADASTRADA // QUANTIDADE DE NÚMEROS INCORRETA PARA UM CONTA!\n\a\n");
                    rewind(arquivos.cadastroContaCorrente);
                    printf("Digite o número da conta (qtd.: 9 números): ");
                    fgets(dados.conta, lowSize, stdin);
                    fflush(stdin);

                    checagemCondicoes = checkTheChars(dados.conta, strlen(dados.conta), NUMBER);
                    checagemCondicoesAdicionais = DataCheck(arquivos.cadastroContaCorrente, dados.conta, CONTA);
                }
                while(checagemCondicoes || checagemCondicoesAdicionais || strlen(dados.conta) != 10);

                checagemCondicoes = 1, checagemCondicoesAdicionais = 0;

                do
                {
                    if(!saia)
                        break;
                    if(!checagemCondicoes || checagemCondicoesAdicionais)
                        printf("\nTIPO INCORRETO ENCONTRADO // VALOR MENOR QUE 100!\n\a\n");

                    printf("Digite o valor inicial (utilize , para valores decimais, limite mínimo de 100 reais): R$ ");
                    checagemCondicoes = scanf(" %lf", &dados.valorAtual);
                    fflush(stdin);

                    checagemCondicoesAdicionais = dados.valorAtual < 100;
                }
                while(!checagemCondicoes || checagemCondicoesAdicionais);

                if(saia)
                    fwrite(&dados, sizeof(contaCliente), 1, arquivos.cadastroContaCorrente);

                fclose(arquivos.cadastroContaCorrente);
                fclose(arquivos.cadastroCliente);
            }
            else
            {
                printf("Arquivo não encontrado (cadastrocc.txt) // (dadosdocliente.txt)!\n");
            }
            system("pause");
            clrscr();
            break;
        }
        case 3:  // REGISTRO ATENDIMENTO DIARIO
        {
            arquivos.cadastroCliente = fopen("dadosdocliente.txt", "rb");
            arquivos.registroAtendimento= fopen("extratobancario.txt","a+b");
            arquivos.cadastroContaCorrente = fopen("cadastrocc.txt", "rb");

            if(arquivos.registroAtendimento != NULL && arquivos.cadastroContaCorrente != NULL)
            {
                double resultado;
                contaCliente * clientes;

                int checagemCondicao = 0, sair = 1, novoTamanho, posicaoConta, verificador;

                registroDiario dadosDiarios;
                dadosFila dadosClienteFila;
                infoCliente dadosDoCliente;
                contaCliente dadosConta;

                time(&dadosDiarios.seconds);
                dadosClienteFila.horaDeRegistro = dadosDiarios.seconds;

                clientes = (contaCliente *)calloc(sizeForFile, sizeof(contaCliente));
                novoTamanho = fread(clientes, sizeof(contaCliente), sizeForFile, arquivos.cadastroContaCorrente);
                clientes = realloc(clientes, novoTamanho * sizeof(contaCliente));

                do
                {
                    if(!sair)
                        break;
                    if(checagemCondicao)
                        printf("\nTIPO INCORRETO ENCONTRADO // SEM PONTUAÇÃO // CONTA INVÁLIDA // QUANTIDADE DE NÚMEROS INCORRETA PARA UM CONTA!\n\a\n");

                    rewind(arquivos.cadastroContaCorrente);
                    printf("Entre com a conta bancária(qtd.: 9 números, para sair digite \"EXIT\"): ");
                    fgets(dadosDiarios.conta, lowSize, stdin);
                    fflush(stdin);

                    sair = strcmp(dadosDiarios.conta, "EXIT\n");
                    checagemCondicao = checkTheChars(dadosDiarios.conta, strlen(dadosDiarios.conta), NUMBER) || !DataCheck(arquivos.cadastroContaCorrente, dadosDiarios.conta, CONTA) || strlen(dadosDiarios.conta) != 10;
                }
                while(checagemCondicao);

                for(verificador = 0; verificador < novoTamanho && sair; verificador++)
                {
                    if(!strcmp(clientes[verificador].conta, dadosDiarios.conta))
                    {
                        posicaoConta = verificador;
                        break;
                    }
                }
                if(sair)
                    printf("\nValor em Conta: R$ %.2lf\n\n", clientes[posicaoConta].valorAtual);

                checagemCondicao = 0;
                do
                {
                    if(!sair)
                        break;
                    if(checagemCondicao)
                        printf("\nOPERAÇÃO INEXISTENTE!\n\a\n");

                    printf("Entre com a operação (S-saque, D-deposito, P-pagamento): ");
                    scanf(" %c",&dadosDiarios.operacao);
                    fflush(stdin);

                    dadosDiarios.operacao = toupper(dadosDiarios.operacao);

                    checagemCondicao = dadosDiarios.operacao != 'S' && dadosDiarios.operacao != 'P' && dadosDiarios.operacao != 'D';
                }
                while(checagemCondicao);

                checagemCondicao = 1;
                do
                {
                    if(!sair)
                        break;
                    resultado = clientes[posicaoConta].valorAtual;

                    if(!checagemCondicao)
                        printf("\nTIPO INCORRETO ENCONTRADO // VALOR DE RETIRADA MAIOR QUE O VALOR DO SALDO\n\a\n");

                    printf("Entre com o valor (min.: 10 reais): R$ ");
                    checagemCondicao = scanf(" %lf", &dadosDiarios.valor);
                    fflush(stdin);

                    if(dadosDiarios.operacao == 'S' || dadosDiarios.operacao == 'P')
                        resultado -= dadosDiarios.valor;
                    else
                        resultado += dadosDiarios.valor;
                }
                while(!checagemCondicao || dadosDiarios.valor < 10);

                if(sair)
                {
                    if(resultado >= 0)
                    {
                        clientes[posicaoConta].valorAtual = resultado;
                        dadosDiarios.status = 1;
                    }
                    else
                        dadosDiarios.status = 0;

                    fclose(arquivos.cadastroContaCorrente);
                    arquivos.cadastroContaCorrente = fopen("cadastrocc.txt", "r+b");

                    fwrite(clientes, sizeof(contaCliente), novoTamanho, arquivos.cadastroContaCorrente);
                    fwrite(&dadosDiarios, sizeof(registroDiario), 1, arquivos.registroAtendimento);

                    // FILA

                    strcpy(dadosClienteFila.conta, dadosDiarios.conta);

                    rewind(arquivos.cadastroContaCorrente);
                    dadosConta = DadosDaConta(dadosClienteFila.conta, arquivos.cadastroContaCorrente);

                    dadosDoCliente = DadosCliente(dadosConta.cpf, arquivos.cadastroCliente);

                    dadosClienteFila.idade = dadosDoCliente.idade;
                    strcpy(dadosClienteFila.nome, dadosDoCliente.nome);

                    if(dadosClienteFila.idade >= 60)
                        dadosClienteFila.tempoDeEspera = 5;
                    else
                        dadosClienteFila.tempoDeEspera = 0;

                    if(posicao >= 0)
                        fila = realloc(fila, (posicao + 1) * sizeof(dadosFila));
                    fila[posicao] = dadosClienteFila;
                    posicao++;
                }
            }

            else
            {
                printf("Arquivo não encontrado!\n");
                break;
            }
            ordeneFila(fila, posicao);

            fclose(arquivos.cadastroContaCorrente);
            fclose(arquivos.registroAtendimento);
            fclose(arquivos.cadastroCliente);

            system("pause");
            clrscr();
            break;
        }
        case 4:  // FILA DE CORRENTISTAS PARA ATENDIMENTO
        {
            int colocarPosicao;
            dadosFila * filaReposicao;

            if(posicao > 1)
                filaReposicao = calloc(posicao - 1, sizeof(dadosFila));

            for(colocarPosicao = 1; colocarPosicao < posicao; colocarPosicao++)
            {
                filaReposicao[colocarPosicao - 1] = fila[colocarPosicao];
            }

            fila = NULL;
            if(posicao > 0)
                posicao--;

            if(posicao > 0)
            {
                fila = realloc(fila, posicao * sizeof(dadosFila));

                for(colocarPosicao = 0; colocarPosicao < posicao; colocarPosicao++)
                {
                    if(filaReposicao[colocarPosicao].idade >= 60)
                        filaReposicao[colocarPosicao].tempoDeEspera += 2;
                    else
                        filaReposicao[colocarPosicao].tempoDeEspera += 1;
                    fila[colocarPosicao] = filaReposicao[colocarPosicao];
                }
                ordeneFila(fila, posicao);
            }

            system("pause");
            clrscr();
            break;
        }
        case 5:  // MOSTRA OS CORRENTISTAS PARA ATENDIMENTO
        {
            struct tm *p;
            for(printar = 0; printar < posicao; printar++)
            {
                p = localtime(&fila[printar].horaDeRegistro);
                printf("*****posição: %d*****\nNome: %stempo de espera: %d\nidade: %d\n", printar + 1, fila[printar].nome, fila[printar].tempoDeEspera, fila[printar].idade);
                printf("Hora de registro:\n");
                printf("Data: %d/%d/%d\n", p->tm_mday, p->tm_mon  + 1, p->tm_year + 1900);
                printf("Hora: %d:%d:%d\n\n", p->tm_hour, p->tm_min, p->tm_sec);
            }

            system("pause");
            clrscr();
            break;
        }
        case 6:  // MOSTRA AS MOVIMENTAÇÕES
        {
            arquivos.registroAtendimento = fopen("extratobancario.txt","rb");

            if(arquivos.registroAtendimento != NULL )
            {
                int novoTamanho, printandoDados;
                registroDiario * dadosDiarios;
                struct tm *p;

                dadosDiarios = (registroDiario *)calloc(sizeForFile, sizeof(dadosDiarios));

                novoTamanho = fread(dadosDiarios, sizeof(registroDiario), sizeForFile, arquivos.registroAtendimento);

                dadosDiarios = realloc(dadosDiarios, novoTamanho * sizeof(registroDiario));

                for(printandoDados = 0; printandoDados < novoTamanho; printandoDados++)
                {
                    p = localtime(&dadosDiarios[printandoDados].seconds);
                    printf("\n**********EXTRATO**********\n\n");

                    printf("Data: %d/%d/%d\n", p->tm_mday, p->tm_mon  + 1, p->tm_year + 1900);
                    printf("Hora: %d:%d:%d\n", p->tm_hour, p->tm_min, p->tm_sec);
                    printf("Conta corrente: %s\n", dadosDiarios[printandoDados].conta);
                    printf("Operação: %c\n", dadosDiarios[printandoDados].operacao);
                    printf("Valor: R$ %.2lf\n", dadosDiarios[printandoDados].valor);
                    printf("Status: %d\n\n", dadosDiarios[printandoDados].status);
                }
                fclose(arquivos.registroAtendimento);
                system("pause");
            }

            else
            {
                printf("Arquivo não encontrado!\n");
                break;
            }
            clrscr();
            break;
        }
        default:
            printf("ESCOLHA UMA OPÇÃO VÁLIDA!\n\a\n");
            system("pause");
            clrscr();
        }
    }

    return 0;
}

void menu()
{
    printf("%s\n %s\n %s\n %s\n %s\n %s\n %s\n",
           "MENU",
           "1. Cadastrar correntista",
           "2. Cadastrar conta corrente",
           "3. Registrar atendimento diário",
           "4. Chamar correntista para atendimento",
           "5. Mostrar fila de atendimento",
           "6. Mostrar movimentação diária");
}

int DataCheck(FILE * arquivo, char * dadoVerificar, dados tipo_de_check)
{
    int novoTamanho, verificador;

    if(tipo_de_check == CPF)
    {
        infoCliente * clientes;

        clientes = (infoCliente *)calloc(sizeForFile, sizeof(infoCliente));

        novoTamanho = fread(clientes, sizeof(infoCliente), sizeForFile, arquivo);

        clientes = realloc(clientes, novoTamanho * sizeof(infoCliente));

        for(verificador = 0; verificador < novoTamanho; verificador++)
            if(!strcmp(clientes[verificador].cpf, dadoVerificar))
                return 1;
    }
    else if(tipo_de_check == CONTA || tipo_de_check == CONTA_CPF)
    {
        contaCliente * clientes;

        clientes = (contaCliente *)calloc(sizeForFile, sizeof(contaCliente));

        novoTamanho = fread(clientes, sizeof(contaCliente), sizeForFile, arquivo);

        clientes = realloc(clientes, novoTamanho * sizeof(contaCliente));

        if(tipo_de_check == CONTA)
        {
            for(verificador = 0; verificador < novoTamanho; verificador++)
                if(!strcmp(clientes[verificador].conta, dadoVerificar))
                    return 1;
        }
        else
        {
            for(verificador = 0; verificador < novoTamanho; verificador++)
                if(!strcmp(clientes[verificador].cpf, dadoVerificar))
                    return 1;
        }
    }
    return 0;
}

int checkTheChars(char * stringValue, int size, typeCheck type)
{
    int checkChars;
    for(checkChars = 0; checkChars < size; checkChars++)
    {
        if(type == STRING && (!isalpha(stringValue[checkChars]) && !isspace(stringValue[checkChars]) && stringValue[checkChars] != '\n'))
            return 1;
        else if(type == NUMBER && (!isdigit(stringValue[checkChars]) && stringValue[checkChars] != '\n'))
            return 1;
        else if(type == ALPHANUM && (!isalnum(stringValue[checkChars]) && !isspace(stringValue[checkChars]) && stringValue[checkChars] != '\n'))
            return 1;
    }
    return 0;
}

contaCliente DadosDaConta(char * dadoParaChecagem, FILE * arquivo)
{
    int novoTamanho, verificador;
    contaCliente * clientes;

    clientes = (contaCliente *) calloc(sizeForFile, sizeof(contaCliente));

    novoTamanho = fread(clientes, sizeof(contaCliente), sizeForFile, arquivo);

    clientes = realloc(clientes, novoTamanho * sizeof(contaCliente));

    for(verificador = 0; verificador < novoTamanho; verificador++)
    {
        if(!strcmp(clientes[verificador].conta, dadoParaChecagem))
        {
            return clientes[verificador];
        }
    }
    return clientes[0];
}

infoCliente DadosCliente(char * dadoParaChecagem, FILE * arquivo)
{
    int novoTamanho, verificador;
    infoCliente * clientes;

    clientes = (infoCliente *) calloc(sizeForFile, sizeof(infoCliente));

    novoTamanho = fread(clientes, sizeof(infoCliente), sizeForFile, arquivo);

    clientes = realloc(clientes, novoTamanho * sizeof(infoCliente));

    for(verificador = 0; verificador < novoTamanho; verificador++)
    {
        if(!strcmp(clientes[verificador].cpf, dadoParaChecagem))
        {
            return clientes[verificador];
        }
    }
    return clientes[0];
}
void ordeneFila(dadosFila * listaValores, int listRange)
{
    int VerificacaoFila, ordenar;
    dadosFila temp;

    for(VerificacaoFila = 1; VerificacaoFila < listRange; VerificacaoFila++)
        for(ordenar = VerificacaoFila; ordenar > 0; ordenar--)
        {
            if(listaValores[ordenar].tempoDeEspera > listaValores[ordenar - 1].tempoDeEspera)
            {
                temp = listaValores[ordenar];
                listaValores[ordenar] = listaValores[ordenar - 1];
                listaValores[ordenar - 1] = temp;
            }
            if(listaValores[ordenar].tempoDeEspera == listaValores[ordenar - 1].tempoDeEspera && listaValores[ordenar].idade > listaValores[ordenar - 1].idade)
            {
                temp = listaValores[ordenar];
                listaValores[ordenar] = listaValores[ordenar - 1];
                listaValores[ordenar - 1] = temp;
            }
            if(listaValores[ordenar].tempoDeEspera == listaValores[ordenar - 1].tempoDeEspera && listaValores[ordenar].idade == listaValores[ordenar - 1].idade && listaValores[ordenar].horaDeRegistro == listaValores[ordenar - 1].horaDeRegistro)
            {
                temp = listaValores[ordenar];
                listaValores[ordenar] = listaValores[ordenar - 1];
                listaValores[ordenar - 1] = temp;
            }
        }
}
