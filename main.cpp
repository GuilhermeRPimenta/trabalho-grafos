#include "grafo_lista.h"
#include "grafo_matriz.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>

void imprimir_dados_grafo(Grafo *grafo)
{
    std::cout << "Grau: " << grafo->get_grau() << std::endl;
    std::cout << "Ordem: " << grafo->get_ordem() << std::endl;
    std::cout << "Direcionado: " << (grafo->eh_direcionado() ? "Sim" : "Nao") << std::endl;
    std::cout << "Componentes conexas: " << (grafo->n_conexo()) << std::endl;
    std::cout << "Vertices ponderados: " << (grafo->vertice_ponderado() ? "Sim" : "Nao") << std::endl;
    std::cout << "Arestas ponderadas: " << (grafo->aresta_ponderada() ? "Sim" : "Nao") << std::endl;
    std::cout << "Completo: " << (grafo->eh_completo() ? "Sim" : "Nao") << std::endl;
    std::cout << "Bipartido: " << (grafo->eh_bipartido() ? "Sim" : "Nao") << std::endl;
    std::cout << "Arvore: " << (grafo->eh_arvore() ? "Sim" : "Nao") << std::endl;
    std::cout << "Ponte: " << (grafo->possui_ponte() ? "Sim" : "Nao") << std::endl;
    std::cout << "Articulacao: " << (grafo->possui_articulacao() ? "Sim" : "Nao") << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Uso: main.out -d|-c -m|-l <arquivo_grafo> [<descricao>]" << std::endl;
        return 1;
    }

    std::string operacao = argv[1];
    std::string estrutura = argv[2];
    std::string arquivo_grafo = argv[3];

    if (operacao == "-d")
    {
        // Caso 1 e Caso 2: Carregar e descrever o grafo
        if (estrutura == "-m")
        {
            // Carregar grafo com matriz de adjacência
            GrafoMatriz grafoMatriz;
            grafoMatriz.carrega_grafo(arquivo_grafo);
            imprimir_dados_grafo(&grafoMatriz);
        }
        else if (estrutura == "-l")
        {
            // Carregar grafo com lista encadeada
            GrafoLista grafo;
            grafo.carrega_grafo(arquivo_grafo);
            imprimir_dados_grafo(&grafo);
        }
        else
        {
            std::cerr << "Estrutura inválida: use -m para matriz ou -l para lista" << std::endl;
            return 1;
        }
    }
    else if (operacao == "-c")
    {

        if (argc < 5)
        {
            std::cerr << "Uso: main.out -c -m ou -l <arquivo_descricao> <arquivo_grafo>" << std::endl;
            return 1;
        }

        std::string arquivo_descricao = argv[3];
        std::string arquivo_grafo = argv[4];

        if (estrutura == "-m")
        {
            GrafoMatriz grafo;
            grafo.novo_grafo(arquivo_descricao);

            std::ofstream saida(arquivo_grafo);
            if (!saida.is_open())
            {
                std::cerr << "Erro ao abrir arquivo para salvar o grafo: " << arquivo_grafo << std::endl;
                return 1;
            }
            grafo.salva_grafo(saida);
            saida.close();
        }
        else if (estrutura == "-l")
        {
            GrafoLista grafo;
            grafo.novo_grafo(arquivo_descricao);

            std::ofstream saida(arquivo_grafo);
            if (!saida.is_open())
            {
                std::cerr << "Erro ao abrir arquivo para salvar o grafo: " << arquivo_grafo << std::endl;
                return 1;
            }
            grafo.salva_grafo(saida);
            saida.close();
        }
        else
        {
            std::cerr << "Estrutura inválida: use -m para matriz ou -l para lista" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cerr << "Operação inválida: use -d para descrever ou -c para criar" << std::endl;
        return 1;
    }

    return 0;
}
