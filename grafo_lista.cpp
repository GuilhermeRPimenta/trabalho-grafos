#include "grafo_lista.h"
#include "lista_encad.h"
#include <iostream>
#include <fstream>
#include <sstream>


GrafoLista::GrafoLista()
    : vertices(nullptr), ordem(0), direcionado(false), vertices_ponderados(false), arestas_ponderadas(false) {}

GrafoLista::GrafoLista(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas)
    : ordem(ordem), direcionado(direcionado), vertices_ponderados(vertices_ponderados), arestas_ponderadas(arestas_ponderadas)
{
    inicializar_vertices(ordem);
}

GrafoLista::~GrafoLista()
{
    if (vertices)
    {
        delete[] vertices;
    }
}

void GrafoLista::inicializar_vertices(int tam)
{
    vertices = new ListaEncadeada[tam]; // N numeros de LL (mesmo nr de nos)
}

void GrafoLista::carrega_grafo(const std::string &arquivo)
{
    std::ifstream entrada(arquivo);
    if (!entrada.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo: " << arquivo << std::endl;
        return;
    }

    // Lê ordem, direcionado, vertices ponderados e arestas ponderadas
    entrada >> ordem >> direcionado >> vertices_ponderados >> arestas_ponderadas;
    inicializar_vertices(ordem);

    // Lê os pesos dos vértices (se vértices forem ponderados
    if (vertices_ponderados)
    {
        for (int i = 0; i < ordem; ++i)
        {
            float peso;
            entrada >> peso;
            vertices[i].setPesoV(peso); // IDs começam em 1
        }
    }

    // Lê as arestas
    int origem, destino;
    float pesoAresta = 1.0; // Peso padrão
    while (entrada >> origem >> destino)
    {
        if (arestas_ponderadas)
        {
            entrada >> pesoAresta;
        }
        // Adiciona a aresta (-1 para ajustar com info)
        vertices[origem - 1].insereFinal(destino - 1, pesoAresta);
    }

    entrada.close();

    std::cout << "Grafo carregado com sucesso!" << std::endl;
    std::cout << "Ordem: " << ordem << " | Direcionado: " << (direcionado ? "Sim" : "Não") << std::endl
              << std::endl;
}

void GrafoLista::novo_grafo(const std::string &descricao)
{
    std::ifstream entrada(descricao);
    if (!entrada.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de descrição: " << descricao << std::endl;
        return;
    }

    // Dados de entrada
    int grau, componentesConexas, completo, bipartido, arvore, ponte, articulacao;
    entrada >> grau >> ordem >> direcionado >> componentesConexas >> vertices_ponderados >> arestas_ponderadas >> completo >> bipartido >> arvore >> ponte >> articulacao;

    std::cout << "\nDados lidos do arquivo de descricao:\n"
              << " grau: " << grau << "\n"
              << " ordem: " << ordem << "\n"
              << " direcionado: " << direcionado << "\n"
              << " componentesConexas: " << componentesConexas << "\n"
              << " vertices_ponderados: " << vertices_ponderados << "\n"
              << " arestas_ponderadas: " << arestas_ponderadas << "\n"
              << " completo: " << completo << "\n"
              << " bipartido: " << bipartido << "\n"
              << " arvore: " << arvore << "\n"
              << " ponte: " << ponte << "\n"
              << " articulacao: " << articulacao << "\n";

    if (componentesConexas > ordem)
    {
        std::cerr << "Erro: O numero de componentes conexas nao pode ser maior que a ordem." << std::endl;
        exit(1);
    }

    if (grau > ordem - 1)
    {
        std::cerr << "Erro: O grau e maior ou igual a ordem, impossivel criar o grafo nao direcionado." << std::endl;
        exit(1);
    }

    inicializar_vertices(ordem);
    // Configuração inicial dos pesos dos vértices
    if (vertices_ponderados) {
        for (int i = 0; i < ordem; i++)
        {
            vertices[i].setPesoV(vertices_ponderados ? rand() % 10 + 1 : 1.0); // Peso do vértice
        }
        
    }

    if (completo)
    {
        // Gerar um grafo completo
        for (int i = 0; i < ordem; i++)
        {
            for (int j = 0; j < ordem; j++)
            {
                if (i != j)
                {
                    float pesoAresta = arestas_ponderadas ? (rand() % 10 + 1) / 2.0 : 1.0;
                    vertices[i].insereFinal(j, pesoAresta);
                }
            }
        }
        if (bipartido && ordem != 2) {
            std::cerr << "Erro: Impossível criar um grafo completo e bipartido com ordem diferente de 2" << std::endl;
            exit(1);
        }

        if (n_conexo() != 1)
        {
            std::cerr << "Erro: Impossível criar um grafo completo com " << componentesConexas << " componentes conexas" << std::endl;
            exit(1);
        }

        if (grau != ordem - 1)
        {
            std::cerr << "Erro: Impossível criar um grafo completo com grau " << grau << "diferente do vértice que tem maior grau" << std::endl;
            exit(1);
        }
    }
    else if (bipartido)
    {
        // Gerar um grafo bipartido
        int divisor = ordem - grau;
        for (int i = 0; i < divisor; i++)
        {
            for (int j = divisor; j < ordem; j++)
            {
                float pesoAresta = arestas_ponderadas ? (rand() % 10 + 1) / 2.0 : 1.0;
                vertices[i].insereFinal(j, pesoAresta);
                if (!direcionado)
                {
                    vertices[j].insereFinal(i, pesoAresta);
                }
            }
        }

        if (grau > ordem - 1) {
            std::cerr << "Erro: Impossível criar um grafo bipartido com grau maior que a ordem" << std::endl;
            exit(1);
        }
    }
    else if (arvore)
    {
        // Gerar uma árvore (conexão mínima sem ciclos)
        for (int i = 1; i < ordem; i++)
        {
            int pai = rand() % i; // Escolhe um vértice já existente como pai
            float pesoAresta = arestas_ponderadas ? (rand() % 10 + 1) / 2.0 : 1.0;
            vertices[pai].insereFinal(i, pesoAresta);
            if (!direcionado)
            {
                vertices[i].insereFinal(pai, pesoAresta);
            }
        }
    }
    else
    {
        // Gerar grafo geral com componentes conexas
        int componenteAtual = 0;
        int verticesPorComponente = ordem / componentesConexas;

        for (int i = 0; i < ordem; i++)
        {
            if (i / verticesPorComponente > componenteAtual && componenteAtual < componentesConexas - 1)
            {
                componenteAtual++;
            }
        }

        // Garantir um vértice com o grau especificado
        int verticePrincipal = rand() % ordem;
        int grauAtual = 0;

        while (grauAtual < grau)
        {
            int destino = rand() % ordem;
            if (destino != verticePrincipal)
            {
                float pesoAresta = arestas_ponderadas ? (rand() % 10 + 1) / 2.0 : 1.0;

                vertices[verticePrincipal].insereFinal(destino, pesoAresta);
                if (!direcionado)
                {
                    vertices[destino].insereFinal(verticePrincipal, pesoAresta);
                }

                grauAtual++;
            }
        }

        // Adicionar arestas para componentes restantes
        for (int i = 0; i < ordem; i++)
        {
            for (int j = 0; j < ordem; j++)
            {
                if (i != j && vertices[i].tamanho() < grau && vertices[j].tamanho() < grau)
                {
                    float pesoAresta = arestas_ponderadas ? (rand() % 10 + 1) / 2.0 : 1.0;

                    vertices[i].insereFinal(j, pesoAresta);
                    if (!direcionado)
                    {
                        vertices[j].insereFinal(i, pesoAresta);
                    }
                }
            }
        }
    }
}

void GrafoLista::salva_grafo(std::ofstream &saida) const
{
    saida << ordem << " " << direcionado << " "
          << vertices_ponderados << " " << arestas_ponderadas << std::endl;

    if (vertices_ponderados)
    {
        for (int i = 0; i < ordem; ++i)
        {
            saida << vertices[i].getPesoV() << " ";
        }
        saida << std::endl;
    }

    for (int i = 0; i < ordem; ++i)
    {
        vertices[i].escrever(saida, i);
    }
}

int GrafoLista::get_grau() const
{
    int max_grau = 0;

    // Contando os graus de saída
    for (int i = 0; i < ordem; ++i)
    {
        int grau_saida = vertices[i].tamanho(); // Grau de saída do vértice i

        // Atualizando o grau máximo
        if (grau_saida > max_grau)
        {
            max_grau = grau_saida;
        }
    }

    return max_grau;
}

int GrafoLista::get_ordem() const
{
    return ordem;
}

bool GrafoLista::eh_direcionado() const
{
    return direcionado;
}

bool GrafoLista::vertice_ponderado() const
{
    return vertices_ponderados;
}

bool GrafoLista::aresta_ponderada() const
{
    return arestas_ponderadas;
}

bool GrafoLista::eh_completo() const
{
    int max_arestas = ordem * (ordem - 1) / 2;
    if (direcionado)
    {
        max_arestas *= 2;
    }
    return get_grau() == max_arestas;
}

void GrafoLista::dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo) const
{
    visitado[vertice] = true;
    No *noAtual = vertices[vertice].getPrimeiro();
    while (noAtual)
    {
        int adj = noAtual->getInfo();
        if (!visitado[adj])
        {
            dfs_ordem(adj, visitado, pilha, topo);
        }
        noAtual = noAtual->getProx();
    }
    pilha[++topo] = vertice;
}

void GrafoLista::dfs(int vertice, bool *visitado) const
{
    visitado[vertice] = true;
    No *noAtual = vertices[vertice].getPrimeiro();
    while (noAtual)
    {
        int adj = noAtual->getInfo();
        if (!visitado[adj])
        {
            dfs(adj, visitado);
        }
        noAtual = noAtual->getProx();
    }
}

GrafoLista GrafoLista::transpor() const
{
    GrafoLista transposto(ordem, true, vertices_ponderados, arestas_ponderadas);
    for (int i = 0; i < ordem; ++i)
    {
        No *noAtual = vertices[i].getPrimeiro();
        while (noAtual)
        {
            int adj = noAtual->getInfo();
            float peso = noAtual->getPeso();
            transposto.vertices[adj].insereFinal(i, peso);
            noAtual = noAtual->getProx();
        }
    }
    return transposto;
}

int GrafoLista::n_conexo() const
{
    if (eh_direcionado())
    {
        // Algoritmo de Kosaraju para grafos direcionados
        int *pilha = new int[ordem];
        bool *visitado = new bool[ordem];
        int topo = -1;

        // Preenche a pilha na ordem inversa
        for (int i = 0; i < ordem; ++i)
            visitado[i] = false;

        for (int i = 0; i < ordem; ++i)
        {
            if (!visitado[i])
                dfs_ordem(i, visitado, pilha, topo);
        }

        GrafoLista grafoTransposto = transpor();

        for (int i = 0; i < ordem; ++i)
            visitado[i] = false;

        int numComponentes = 0;
        while (topo >= 0)
        {
            int v = pilha[topo--];
            if (!visitado[v])
            {
                grafoTransposto.dfs(v, visitado);
                ++numComponentes;
            }
        }

        delete[] pilha;
        delete[] visitado;

        return numComponentes;
    }
    else
    {
        // Algoritmo grafos não direcionados
        bool *visitado = new bool[ordem];
        for (int i = 0; i < ordem; ++i)
            visitado[i] = false;

        int numComponentes = 0;
        for (int i = 0; i < ordem; ++i)
        {
            if (!visitado[i])
            {
                dfs(i, visitado);
                ++numComponentes;
            }
        }

        delete[] visitado;
        return numComponentes;
    }
}

bool GrafoLista::bfs_bipartido(int inicio, int *cor) const
{
    int *fila = new int[ordem];
    int inicioFila = 0, fimFila = 0;

    // vértice inicial adicionado a uma fila e colore com 0
    fila[fimFila++] = inicio;
    cor[inicio] = 0;

    // processa os vértices da fila
    while (inicioFila < fimFila)
    {
        int atual = fila[inicioFila++];

        // percorre todos os adjacentes ao vértice atual
        No *noAtual = vertices[atual].getPrimeiro();
        while (noAtual)
        {
            int adj = noAtual->getInfo();
            if (cor[adj] == -1)
            {
                // adjacente ainda não foi visitado, colore com a cor oposta
                cor[adj] = 1 - cor[atual];
                fila[fimFila++] = adj;
            }
            else if (cor[adj] == cor[atual])
            {
                // vértice adjacente tem a mesma cor
                delete[] fila;
                return false;
            }
            noAtual = noAtual->getProx();
        }
    }

    delete[] fila;
    return true;
}

bool GrafoLista::eh_bipartido() const
{
    if(ordem == 1) return false;
    // -1: não visitado, 0: cor 0, 1: cor 1
    int *cor = new int[ordem];
    for (int i = 0; i < ordem; ++i)
    {
        cor[i] = -1; // todos os vértices estão sem cor
    }

    // percorre todos os vértices (para lidar com grafos desconexos)
    for (int i = 0; i < ordem; ++i)
    {
        if (cor[i] == -1)
        { // o vértice ainda não foi visitado
            if (!bfs_bipartido(i, cor))
            {
                delete[] cor;
                return false;
            }
        }
    }

    delete[] cor;
    return true;
}

bool GrafoLista::eh_arvore() const
{
    // Verifica se o grafo é conexo
    if (n_conexo() != 1)
    {
        return false;
    }

    // Verifica se o grafo tem exatamente ordem - 1 arestas
    int num_arestas = 0;
    for (int i = 0; i < ordem; ++i)
    {
        num_arestas += vertices[i].tamanho();
    }
    if (direcionado)
        num_arestas = num_arestas / 2;

    // Retorna true se o número de arestas for exatamente ordem - 1
    return (num_arestas == ordem - 1);
}

bool GrafoLista::possui_articulacao() const {
    // Verificar conectividade inicial
    if (n_conexo() > 1) {
        return false; // Se já não é conexo, não faz sentido buscar pontes
    }
    if(ordem == 1) return false;

    // Para cada vértice do grafo
    for (int u = 0; u < ordem; u++) {
        // Armazenar os vizinhos de u em um array dinâmico
        int grau = vertices[u].tamanho(); // Supondo que getGrau() retorna o número de conexões
        int* conexoes_originais = new int[grau]; // Array para armazenar os vizinhos
        int idx = 0;

        for (No* atual = vertices[u].getPrimeiro(); atual != nullptr; atual = atual->getProx()) {
            conexoes_originais[idx++] = atual->getInfo();
        }

        // Remover todas as arestas conectadas ao vértice u
        for (int i = 0; i < grau; i++) {
            int v = conexoes_originais[i];
            vertices[v].remove(u); // Remove a referência a u nos vértices adjacentes
        }
        vertices[u].limpar(); // Remove todas as arestas do vértice u

        // Verificar conectividade
        bool* visitado = new bool[ordem]();
        int inicio = (u == 0) ? 1 : 0;
        dfs(inicio, visitado);

        bool conexo = true;
        for (int i = 0; i < ordem; i++) {
            if (i != u && !visitado[i]) {
                conexo = false;
                break;
            }
        }
        delete[] visitado;

        // Restaurar as conexões originais
        for (int i = 0; i < grau; i++) {
            int v = conexoes_originais[i];
            vertices[v].insereFinal(u); // Restaura u nas listas adjacentes
            vertices[u].insereFinal(v); // Restaura v nas listas adjacentes de u
        }

        delete[] conexoes_originais; // Liberar memória
        if (!conexo) {
            return true; // Encontrou articulação
        }
    }
    return false;
}

bool GrafoLista::possui_ponte() const {
    // Verificar conectividade inicial
    if (n_conexo() > 1) {
        return false; // Se já não é conexo, não faz sentido buscar pontes
    }

    // Para cada vértice, iterar sobre suas arestas
    for (int u = 0; u < ordem; u++) {
        No* atual = vertices[u].getPrimeiro();
        while (atual != nullptr) {
            int v = atual->getInfo();

            // Remover aresta (u, v)
            vertices[u].remove(v);
            vertices[v].remove(u);

            // Verificar se o grafo continua conexo
            bool* visitado = new bool[ordem];
            for (int i = 0; i < ordem; i++) {
                visitado[i] = false;
            }

            dfs(0, visitado); // Realizar DFS a partir do vértice 0

            bool conexo = true;
            for (int i = 0; i < ordem; i++) {
                if (!visitado[i]) {
                    conexo = false;
                    break;
                }
            }

            delete[] visitado;

            // Restaurar aresta (u, v)
            vertices[u].insereFinal(v);
            vertices[v].insereFinal(u);

            // Se o grafo deixou de ser conexo, a aresta é uma ponte
            if (!conexo) {
                return true;
            }

            atual = atual->getProx();
        }
    }

    return false; // Nenhuma ponte encontrada
}
