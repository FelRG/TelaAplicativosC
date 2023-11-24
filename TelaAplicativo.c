#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Estruturas do programa
struct app {
    int numero_app; // ID do aplicativo
    char nome_app[30];
    struct app* prox;
};

struct tela {
    int numero_tela; // ID da tela
    struct tela* ant;
    struct tela* prox;
    struct app* lista_apps; // Lista de aplicativos associados a esta tela
};

typedef struct tela Tela;
typedef struct app App;

// Funcoes prototipadas
Tela *inserirTela(Tela *inicio, int numero_tela);
Tela* removerUltimaTela(Tela* inicio, int numero_total_telas);
void transformarMinusculas(char *str);
App *inserirApp(App *inicio, int numero_app, const char *nome_app);
void associarAppATela(Tela *tela, int numero_app, const char *nome_app);
void listarAppsNaTela(const Tela *tela);
int encontrarCorrespondencia(const Tela *inicio_telas, const char *busca);
int verificarTresApps(const Tela *tela);
int contadorAppsNaTela(const Tela *tela);
App *removerApp(App *inicio, const char *nome_app, int numero_tela);
Tela *avancarTela(Tela *atual);
Tela *voltarTela(Tela *atual);
int contarTelas(const Tela *inicio);
Tela* encontrarUltimaTela(const Tela* inicio);
Tela* reordenarTelas(Tela* inicio);

// Funcoes criadas
Tela *inserirTela(Tela *inicio, int numero_tela) {
    // Aloca memoria para uma nova tela
    Tela *nova_tela = (Tela *)malloc(sizeof(Tela));

    // Verifica se a alocacao de memoria falhou
    if (nova_tela == NULL) {
        printf("Erro: Falha na alocacao de memoria.\n");
        exit(1); // Encerra o programa com um codigo de erro
    }

    // Define o numero da nova tela
    nova_tela->numero_tela = numero_tela;

    // Inicializa os ponteiros para as telas anterior, proxima e a lista de aplicativos
    nova_tela->ant = NULL;
    nova_tela->prox = NULL;
    nova_tela->lista_apps = NULL;

    // Verifica se a lista de telas esta vazia (inicio eh NULL)
    if (inicio == NULL) {
        return nova_tela; // Se estiver vazia, a nova tela se torna a primeira
    } else {
        Tela *temp = inicio;
        // Encontra a ultima tela na lista
        while (temp->prox != NULL) {
            temp = temp->prox;
        }

        // Conecta a nova tela a ultima tela da lista
        temp->prox = nova_tela;
        nova_tela->ant = temp;
        return inicio; // Retorna a lista de telas atualizada
    }
}

Tela* removerUltimaTela(Tela* inicio, int numero_total_telas) {
    Tela* atual = inicio;
    Tela* anterior = NULL;

    // Percorre a lista de telas
    while (atual != NULL) {
        // Verifica se a tela atual eh a última com base no identificador
        if (atual->numero_tela == numero_total_telas) {
            // Se a tela anterior eh nula, significa que a tela atual eh a única na lista
            if (anterior == NULL) {
                free(atual); // Libera a memória da única tela
                return NULL; // A lista está vazia após a remoção
            } else {
                anterior->prox = NULL; // Desconecta a tela atual
                free(atual); // Libera a memória da última tela
                return inicio; // Retorna a lista com elementos após a remoção
            }
        }
        anterior = atual;
        atual = atual->prox;
    }

    // Se a função chegou aqui, a lista estava vazia
    return NULL;
}

void transformarMinusculas(char *str) {
    int i;

    // Percorre a string caractere por caractere
    for (i = 0; str[i]; i++) {
        // Converte o caractere para minúsculas usando a função tolower
        str[i] = tolower((unsigned char)str[i]);
    }
}

App *inserirApp(App *inicio, int numero_app, const char *nome_app) {
    // Aloca memória para um novo aplicativo
    App *novo_app = (App *)malloc(sizeof(App));

    if (novo_app == NULL) {
        printf("Erro: Falha na alocação de memória.\n");
        exit(1);
    }

    novo_app->numero_app = numero_app;

    // Cria uma cópia do nome do aplicativo em minúsculas
    char nome_app_minusculo[30];
    strcpy(nome_app_minusculo, nome_app);

    // Transforma o nome do aplicativo em letras minúsculas
    transformarMinusculas(nome_app_minusculo);

    // Copia o nome do aplicativo transformado para a estrutura do novo aplicativo
    strncpy(novo_app->nome_app, nome_app_minusculo, sizeof(novo_app->nome_app) - 1);

    novo_app->prox = NULL;

    if (inicio == NULL) {
        return novo_app;
    } else {
        App *temp = inicio;

        // Encontra o último aplicativo na lista
        while (temp->prox != NULL) {
            temp = temp->prox;
        }

        // Adiciona o novo aplicativo como o próximo do último aplicativo na lista
        temp->prox = novo_app;
        return inicio;
    }
}

void associarAppATela(Tela *tela, int numero_app, const char *nome_app) {
    // Chama a função 'inserirApp' para inserir um novo aplicativo na lista de aplicativos da tela
    tela->lista_apps = inserirApp(tela->lista_apps, numero_app, nome_app);
}

void listarAppsNaTela(const Tela *tela) {
    // Verifica se a tela eh nula (inexistente)
    if (tela == NULL) {
        printf("Tela não existe.\n");
        return;
    }

    printf("Aplicativos na tela %d:\n", tela->numero_tela);

    // Obtem a lista de aplicativos associados a esta tela
    const App *app = tela->lista_apps;
    int contador = 1;

    // Se a lista de aplicativos eh nula, significa que nenhum aplicativo está instalado nesta tela
    if (app == NULL) {
        printf("Nenhum aplicativo instalado nesta tela\n\n");
        return;
    }

    // Loop para listar os aplicativos instalados nesta tela
    while (app != NULL) {
        printf("%d. %s\n", contador, app->nome_app);
        app = app->prox;
        contador++;
    }
}

int encontrarCorrespondencia(const Tela *inicio_telas, const char *busca) {
    const Tela *tela = inicio_telas;

    // Percorre todas as telas
    while (tela != NULL) {
        const App *app = tela->lista_apps;

        // Percorre a lista de aplicativos associados a esta tela
        while (app != NULL) {
            // Compara o nome do aplicativo atual com o termo de busca
            if (strcmp(app->nome_app, busca) == 0) {
                return 1; // Se houver correspondência, retorna 1
            }

            app = app->prox; // Move para o próximo aplicativo
        }

        tela = tela->prox; // Move para a próxima tela
    }

    return 0; // Se nenhum aplicativo correspondente for encontrado, retorna 0
}

int verificarTresApps(const Tela *tela) {
    const App *app = tela->lista_apps;
    int contador = 0;

    // Percorre a lista de aplicativos associados a uma tela
    while (app != NULL) {
        contador++;
        app = app->prox; // Move para o próximo aplicativo

        // Verifica se o contador atingiu ou ultrapassou o limite de 3 aplicativos
        if (contador >= 3) {
            return 1; // Retorna 1 se tiver pelo menos 3 aplicativos
        }
    }

    return 0; // Retorna 0 se não houver pelo menos 3 aplicativos
}

int contadorAppsNaTela(const Tela *tela) {
    if (tela == NULL) {
        return 0; // Retorna 0 se a tela não existir (ponteiro nulo)
    }

    const App *app = tela->lista_apps;
    int contador = 0;

    if (app == NULL) {
        return 0; // Retorna 0 se não houver aplicativos na tela
    }

    // Percorre a lista de aplicativos associados à tela e conta quantos existem
    while (app != NULL) {
        app = app->prox; // Move para o próximo aplicativo
        contador++; // Incrementa o contador a cada aplicativo encontrado
    }

    return contador; // Retorna o número de aplicativos na tela
}

App *removerApp(App *inicio, const char *nome_app, int numero_tela) {
    if (inicio == NULL) {
        printf("Nenhum aplicativo para remover.\n");
        return NULL; // Retorna nulo se a lista de aplicativos for vazia
    }

    App *atual = inicio;
    App *anterior = NULL;

    // Verifica se o primeiro aplicativo da lista eh o que deve ser removido
    if (strcmp(atual->nome_app, nome_app) == 0) {
        if (numero_tela != 1 || atual->prox != NULL) {
            App *novo_inicio = atual->prox;
            free(atual);
            printf("\nAplicativo '%s' removido com sucesso.\n", nome_app);
            return novo_inicio; // Retorna o novo início da lista após a remoção
        } else {
            // Não remova o primeiro aplicativo da primeira tela
            printf("\nO primeiro aplicativo da primeira tela nao pode ser removido.\n");
            return inicio; // Mantem a lista inalterada
        }
    }

    while (atual->prox != NULL) {
        if (strcmp(atual->prox->nome_app, nome_app) == 0) {
            App *removido = atual->prox;
            atual->prox = removido->prox;
            free(removido);
            printf("\nAplicativo '%s' removido com sucesso.\n", nome_app);
            return inicio; // Retorna a lista após a remoção
        }
        atual = atual->prox;
    }

    // Se a função chegou aqui, o aplicativo não foi encontrado na lista
    printf("Aplicativo '%s' não encontrado na lista. Deseja tentar novamente? (S/N): ");
    char resposta[2];
    scanf("%s", resposta);

    if (resposta[0] == 'S' || resposta[0] == 's') {
        printf("Digite o nome do aplicativo novamente: ");
        char novo_nome_app[30];
        scanf("%s", novo_nome_app);
        return removerApp(inicio, novo_nome_app, numero_tela); // Chamada recursiva para tentar remover o novo aplicativo
    } else {
        return inicio; // Retorna a lista inalterada
    }
}

Tela *avancarTela(Tela *atual) {
    if (atual != NULL && atual->prox != NULL) {
        return atual->prox; // Retorna a próxima tela se ela existir
    } else {
        printf("Nao eh possível avancar para a proxima tela.\n");
        return atual; // Retorna a tela atual se não for possível avançar
    }
}

Tela *voltarTela(Tela *atual) {
    if (atual != NULL && atual->ant != NULL) {
        return atual->ant; // Retorna a tela anterior se ela existir
    } else {
        printf("Nao eh possível voltar para a tela anterior.\n");
        return atual; // Retorna a tela atual se não for possível voltar
    }
}

int contarTelas(const Tela *inicio) {
    int contador = 0;        // Inicializa o contador de telas
    const Tela *atual = inicio;  // Inicia a partir da primeira tela na lista

    while (atual != NULL) {  // Enquanto a tela atual não for nula (existir)
        contador++;          // Incrementa o contador de telas
        atual = atual->prox;  // Move para a próxima tela na lista
    }

    return contador;  // Retorna o número total de telas na lista
}

Tela* encontrarUltimaTela(const Tela* inicio) {
    if (inicio == NULL) {
        return NULL;  // Se não houver telas, retorna NULL
    }

    const Tela* ultima_tela = inicio;  // Inicia com a primeira tela da lista

    while (ultima_tela->prox != NULL) {
        ultima_tela = ultima_tela->prox;  // Move para a próxima tela na lista
    }

    return (Tela*)ultima_tela;  // Retorna um ponteiro para a última tela encontrada
}

Tela* reordenarTelas(Tela* T_inicio) {
    Tela* T_atual = T_inicio;
    int totalTelas = contarTelas(T_inicio);

    // Se houver apenas uma tela, não há nada para reordenar
    if (totalTelas == 1) {
        return T_inicio;
    }

    while (T_atual != NULL) {
        int appsNaTelaAtual = contadorAppsNaTela(T_atual);

        // Se a tela atual tiver menos de 3 aplicativos e houver uma próxima tela
        if (appsNaTelaAtual < 3 && T_atual->prox != NULL) {
            Tela* proximaTela = T_atual->prox;
            int appsNaProximaTela = contadorAppsNaTela(proximaTela);

            // Declare as variáveis fora do loop for
            App* app;
            int i;

            // Mova os aplicativos da próxima tela para preencher a tela atual
            for (i = 0; i < 3 - appsNaTelaAtual && appsNaProximaTela > 0; i++) {
                app = proximaTela->lista_apps;
                associarAppATela(T_atual, app->numero_app, app->nome_app);
                proximaTela->lista_apps = removerApp(proximaTela->lista_apps, app->nome_app, proximaTela->numero_tela);
                appsNaTelaAtual++;
                appsNaProximaTela--;
            }

            // Remova a tela vazia se ela estiver vazia
            if (appsNaProximaTela == 0) {
                Tela* telaRemovida = proximaTela;
                T_atual->prox = proximaTela->prox;
                if (proximaTela->prox != NULL) {
                    proximaTela->prox->ant = T_atual;
                }
                free(telaRemovida);
            }
        }

        T_atual = T_atual->prox;
    }

    // Verifique novamente e remova a primeira tela se estiver vazia
    Tela* primeiraTela = T_inicio;
    if (contadorAppsNaTela(primeiraTela) == 0) {
        Tela* telaRemovida = primeiraTela;
        T_inicio = primeiraTela->prox;
        if (T_inicio != NULL) {
            T_inicio->ant = NULL;
        }
        free(telaRemovida);
    }

    return T_inicio; // Retorna o ponteiro para o início da lista de telas atualizado
}


int main() {
    int escolha = 0;
    Tela *T = NULL;
    T = inserirTela(T, 1);
    int totalTelas = 1;
    char nome_aplicativo[30];
    char nome_aplicativo_remover[30];
    int id_app = 1;

    Tela *T_atual = T;
    Tela *T_proxima = NULL;
    Tela *ultima_tela = NULL;

    printf("Bem-vindo ao programa da tela de celular!\n\n");

    do {
        printf("\n");
        printf("Voce esta na tela %d\n\n", T_atual->numero_tela);
        printf("O que deseja realizar?\n\n");
        printf("1. Listar aplicativos da tela %d\n", T_atual->numero_tela);
        printf("2. Instalar um aplicativo\n");

        if (contadorAppsNaTela(T_atual) >= 1) {
            printf("3. Desinstalar um aplicativo\n");
        }

        if (totalTelas > 1 && (T_atual->numero_tela != totalTelas)) {
            printf("4. Avancar para a proxima tela\n");
        }

        if (T_atual->numero_tela != 1 && totalTelas > 1) {
            printf("5. Voltar para a tela anterior\n");
        }

        printf("6. Sair do programa\n");

        printf("\n");

        printf("Digite uma opcao: ");
        
        if (scanf("%d", &escolha) == 1) {
        
        printf("\n");

        switch (escolha) {
            case 1:
                listarAppsNaTela(T_atual);
                break;

            case 2:
                printf("Digite um nome para o aplicativo a ser instalado: \n");
                scanf("%s", nome_aplicativo);
                int retorno = encontrarCorrespondencia(T, nome_aplicativo);
                if (retorno == 1) {
                    printf("\nEsse aplicativo ja existe.\n");
                    break;
                }
				
                ultima_tela = encontrarUltimaTela(T); // Tenta encontrar a última tela pelo começo da lista
				
                int retorno1 = verificarTresApps(ultima_tela); // Verificar aqui
				
                if (retorno1 == 1) {
                    totalTelas++;
                    T_proxima = inserirTela(T, totalTelas);
                    T_atual = avancarTela(T_atual); 
                    associarAppATela(T_atual, id_app, nome_aplicativo);
                    id_app++;
                    printf("\nAplicativo '%s' instalado com sucesso!\n", nome_aplicativo);
                } else {
                    associarAppATela(ultima_tela, id_app, nome_aplicativo);
                    id_app++;
                    printf("\nAplicativo '%s' instalado com sucesso!\n", nome_aplicativo);
                }
                break;

            case 3:
                printf("Digite o nome do aplicativo para desinstalar: \n");
                scanf("%s", nome_aplicativo_remover);
                int retorno_des = encontrarCorrespondencia(T_atual, nome_aplicativo_remover);
                if (retorno_des == 1) {
                    T_atual->lista_apps = removerApp(T_atual->lista_apps, nome_aplicativo_remover, T_atual->numero_tela);
                    T = reordenarTelas(T);
                    // Verificar se a tela atual está vazia
                    if (contadorAppsNaTela(T_atual) < 1 && (T_atual->numero_tela != 1)) {
                        // Voltar para a tela anterior
                        T_atual = voltarTela(T_atual);
                        // Remover a última tela vazia
           				T = reordenarTelas(T);
            			// Atualizar o número total de telas
            			totalTelas = contarTelas(T);
                    }
 
                } else {
                    printf("\nNao foi encontrada nenhuma correspondencia do aplicativo mencionado\n");
                }
                
                break;

            case 4:
                if (totalTelas == 1 || totalTelas == T_atual->numero_tela) {
                    printf("Nao eh possivel avancar para a proxima tela.\n");
                } else {
                    T_atual = avancarTela(T_atual);
                }
                break;

            case 5:
                if (T_atual->numero_tela == 1) {
                    printf("Nao eh possivel voltar para a tela anterior.\n");
                } else {
                    T_atual = voltarTela(T_atual);
                }
                break;

            case 6:
                printf("Programa encerrado.\n");
                break;

            default:
                printf("Opcao invalida. Escolha um numero de 1 a 6.\n");
                break;
        }
    } else {
        printf("Opcao invalida. Escolha um numero de 1 a 6.\n");
        scanf("%*s"); // Limpa o buffer de entrada para evitar loops infinitos
    }
    } while (escolha != 6);

    // Libera a memória alocada para as telas e aplicativos
    Tela *proxima_tela = NULL;
    while (T != NULL) {
        proxima_tela = T->prox;
        while (T->lista_apps != NULL) {
            App *prox_app = T->lista_apps->prox;
            free(T->lista_apps);
            T->lista_apps = prox_app;
        }
        free(T);
        T = proxima_tela;
    }

    return 0;
}

