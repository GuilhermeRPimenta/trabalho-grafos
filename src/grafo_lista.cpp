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
        delete[] vertices;
    }
}

void GrafoLista::inicializar_vertices(int tam)
{
    vertices = new ListaEncadeada[tam]; // N numeros de LL (mesmo nr de nos)
}

void GrafoLista::setPesoV(float peso, int vertice)
{

    vertices[vertice].setPesoV(peso);
}

void GrafoLista::setAresta(int origem, float pesoAresta, int destino)
{
    vertices[origem - 1].insereFinal(destino - 1, pesoAresta);
}

void GrafoLista::nova_aresta(int origem, float pesoAresta, int destino)
{
    setAresta(origem, pesoAresta, destino);
}

void GrafoLista::deleta_aresta(int origem, int destino)
{
    vertices[origem].remove(destino);
}

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

int GrafoLista::getGrauV(int indice)
{
    return vertices[indice].tamanho();
}

void GrafoLista::dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo)
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

void GrafoLista::dfs(int vertice, bool *visitado)
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

int GrafoLista::conta_transposto(bool *visitado, int *pilha, int &topo)
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

    for (int i = 0; i < ordem; ++i)
        visitado[i] = false;

    int numComponentes = 0;
    while (topo >= 0)
    {
        int v = pilha[topo--];
        if (!visitado[v])
        {
            transposto.dfs(v, visitado);
            ++numComponentes;
        }
    }
    return numComponentes;
}
