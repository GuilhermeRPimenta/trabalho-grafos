#include "../include/grafo_lista.h"
#include "../include/lista_encad.h"
#include "../include/grafo.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

GrafoLista::GrafoLista()
{
    this->direcionado = false;
    this->vertices = nullptr;
    this->ordem = 0;
    this->arestas_ponderadas = false;
    this->vertices_ponderados = false;
}

GrafoLista::GrafoLista(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas)
{
    this->direcionado = direcionado;
    this->ordem = ordem;
    this->arestas_ponderadas = arestas_ponderadas;
    this->vertices_ponderados = vertices_ponderados;
    inicializar_vertices(ordem);
}

GrafoLista::~GrafoLista()
{
    if (vertices)
    {
        delete vertices; // Apenas deletamos a LinkedList
    }

    delete[] clusters_visitados;
    delete[] relacao_id_cluster;
}

void GrafoLista::inicializar_vertices(int tam)
{
    this->vertices = new LinkedList(); // Aloca memória dinamicamente

    for (int i = 0; i < tam; i++)
    {
        this->vertices->adicionarVertice(i, 0.0);
    }
}

void GrafoLista::novo_no(int indice, float peso)
{
    if (!vertices)
    {
        std::cerr << "Erro: Lista de vértices não inicializada!" << std::endl;
        return;
    }
    vertices->adicionarVertice(indice - 1, peso);
    this->ordem = ordem + 1;
}

void GrafoLista::deleta_no(int indice)
{
    vertices->removerVertice(indice - 1);
    this->ordem = ordem - 1;
}

void GrafoLista::setPesoV(float peso, int vertice)
{
    if (!vertices || !vertices->primeiro)
    {
        std::cerr << "Erro: Lista de vértices não inicializada!" << std::endl;
        return;
    }

    NoLL *atual = vertices->primeiro;
    while (atual && atual->indice != vertice)
    {
        atual = atual->proximo;
    }

    if (!atual)
    {
        std::cerr << "Erro: Vértice " << vertice << " não encontrado!" << std::endl;
        return;
    }

    atual->lista.setPesoV(peso);
}

void GrafoLista::nova_aresta(int origem, float pesoAresta, int destino)
{
    setAresta(origem, pesoAresta, destino - 1);
}

void GrafoLista::deleta_aresta(int origem, int destino)
{
    NoLL *atual = vertices->primeiro;
    while (atual->indice != (origem - 1))
    {
        atual = atual->proximo;
    }
    atual->lista.remove(destino - 1);
}

void GrafoLista::setAresta(int origem, float pesoAresta, int destino)
{
    if (!vertices || !vertices->primeiro)
    {
        std::cerr << "Erro: Lista de vértices não inicializada!" << std::endl;
        return;
    }

    NoLL *atual = vertices->primeiro;
    while (atual && atual->indice != (origem - 1))
    {
        atual = atual->proximo;
    }

    if (!atual)
    {
        std::cerr << "Erro: Vértice de origem " << origem << " não encontrado!" << std::endl;
        return;
    }

    atual->lista.insereFinal((destino - 1), pesoAresta);
}

bool GrafoLista::existe_Aresta(int node, int i)
{
    NoLL *atual = vertices->primeiro;
    while (atual && atual->indice != (node - 1)) {
        atual = atual->proximo;
    } 
    if (!atual) 
    {
        return false;
    }
    return true; 
}

float GrafoLista::get_Pesoaresta(int origem, int destino) {
    // Encontra o nó de origem
    NoLL *atual = vertices->primeiro;
    while (atual && atual->indice != (origem - 1)) {
        atual = atual->proximo;
    }

    // Se o nó de origem não foi encontrado
    if (!atual) {
        return 1000000000;  // Retorna um valor de erro
    }

    // Encontra a aresta associada ao nó de origem
    No *auxAresta = atual->lista.getPrimeiro();
    while (auxAresta && auxAresta->getInfo() != (destino - 1)) {
        auxAresta = auxAresta->getProx();
    }

    // Se a aresta não for encontrada
    if (!auxAresta) {
        return 1000000000;  // Retorna um valor de erro
    }

    // Retorna o peso da aresta
    return auxAresta->getPeso();
}


void GrafoLista::salva_grafo(std::ofstream &saida) const
{
    saida << ordem << " " << direcionado << " "
          << vertices_ponderados << " " << arestas_ponderadas << std::endl;

    if (vertices_ponderados)
    {
        NoLL *atual = vertices->primeiro;
        while (atual != nullptr)
        {
            saida << atual->lista.getPesoV() << " ";
            atual = atual->proximo;
        }
        saida << std::endl;

        atual = vertices->primeiro;
        while (atual != nullptr)
        {
            atual->lista.escrever(saida, atual->indice);
            atual = atual->proximo;
        }
        saida << std::endl;
    }
}

int GrafoLista::getGrauV(int indice)
{
    NoLL *atual = vertices->primeiro;
    while (atual && atual->indice != indice)
    {
        atual = atual->proximo;
    }
    if (!atual)
    {
        return -1;
    }

    return atual->lista.tamanho();
}

void GrafoLista::aux_dfs_ordem(NoLL *atual, int vertice, bool *visitado, int *pilha, int &topo)
{
    visitado[vertice] = true;

    No *noAtual = atual->lista.getPrimeiro();
    while (noAtual)
    {
        int adj = noAtual->getInfo();
        if (!visitado[adj])
        {
            aux_dfs_ordem(atual->proximo, adj, visitado, pilha, topo);
        }
        noAtual = noAtual->getProx();
    }
    pilha[++topo] = vertice;
}

void GrafoLista::dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo)
{
    NoLL *atual = vertices->primeiro;
    aux_dfs_ordem(atual, vertice, visitado, pilha, topo);
}

void GrafoLista::aux_dfs(NoLL *atual, int vertice, bool *visitado)
{
    visitado[vertice] = true;

    No *noAtual = atual->lista.getPrimeiro();
    while (noAtual)
    {
        int adj = noAtual->getInfo();
        if (!visitado[adj])
        {
            aux_dfs(atual->proximo, adj, visitado);
        }
        noAtual = noAtual->getProx();
    }
}

void GrafoLista::dfs(int vertice, bool *visitado)
{
    NoLL *atual = vertices->primeiro;
    aux_dfs(atual, vertice, visitado);
}

int GrafoLista::conta_transposto(bool *visitado, int *pilha, int &topo)
{
    NoLL *atual = vertices->primeiro;
    NoLL *transp;
    GrafoLista transposto(ordem, true, vertices_ponderados, arestas_ponderadas);
    for (int i = 0; i < ordem; ++i)
    {
        No *noAtual = atual->lista.getPrimeiro();
        while (noAtual)
        {
            int adj = noAtual->getInfo();
            float peso = noAtual->getPeso();
            transposto.vertices->adicionarVertice(adj, peso);
            transp = transposto.vertices->buscarIndice(adj);
            transp->lista.insereFinal(i, peso);
            noAtual = noAtual->getProx();
        }
    }

    for (int i = 0; i < ordem; ++i)
        visitado[i] = false;

    int numComponentes = 0;
    while (topo >= 0)
    {
        int v = pilha[topo--];
        if (!visitado[v])
        {
            transposto.dfs(v, visitado);
            numComponentes++;
        }
    }
    return numComponentes;
}

void GrafoLista::imprimir()
{
    if (!vertices || !vertices->primeiro)
    {
        std::cerr << "Erro: Grafo vazio!" << std::endl;
        return;
    }

    NoLL *atual = vertices->primeiro;
    while (atual != nullptr)
    {
        std::cout << "Vértice " << atual->indice << ": ";
        atual->lista.imprimir();
        atual = atual->proximo;
    }
}

/*
void GrafoLista::novo_grafo(const std::string &descricao)
{
    std::string caminho_completo = "./entradas/" + descricao; // Ajuste para o diretório atual
    std::ifstream entrada(caminho_completo);
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
    if (vertices_ponderados)
    {
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
        if (bipartido && ordem != 2)
        {
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

        if (grau > ordem - 1)
        {
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
*/