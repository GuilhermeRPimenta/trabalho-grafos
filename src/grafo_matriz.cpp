#include "../include/grafo_matriz.h"
#include "../include/grafo.h"
#include <ctime>
#include <cstdlib>
int gerar_numero_aleatorio(int min, int max)
{
    return min + rand() % ((max + 1) - min);
}

GrafoMatriz::GrafoMatriz()
{
    matriz = nullptr;
    matriz_sem_direcao = nullptr;
    dim_matriz = 10;
    vertices = nullptr;
    ordem = 0;
    direcionado = true;
    arestas_ponderadas = false;
    vertices_ponderados = false;
}

GrafoMatriz::GrafoMatriz(int ordem, bool direcionado, bool vertices_ponderados, bool arestas_ponderadas)
{
    this->direcionado = true;
    this->arestas_ponderadas = arestas_ponderadas;
    this->vertices_ponderados = vertices_ponderados;
    inicializar_vertices(ordem);
}

GrafoMatriz::~GrafoMatriz()
{
    /*
    for (int i = 0; i<dim_matriz; i++) {
        for (int j = 0; j<dim_matriz; j++) {
            cout << get_aresta(i,j) << " ";
        }
        cout << endl;
    }
    */

    if (direcionado)
    {
        for (int i = 0; i < dim_matriz; i++)
        {
            delete[] matriz[i];
        }
        delete[] matriz;
    }
    else
    {
        delete[] matriz_sem_direcao;
    }

    delete[] vertices;
}

void GrafoMatriz::inicializar_vertices(int ordem)
{
    // Sempre começa com tamanho 10
    dim_matriz = 10;
    this->ordem = 0;

    // Incialização das Matrizes de Adjacência
    if (direcionado)
    {
        matriz = new float *[dim_matriz];
        for (int i = 0; i < dim_matriz; i++)
        {
            matriz[i] = new float[dim_matriz];
            for (int j = 0; j < dim_matriz; j++)
            {
                matriz[i][j] = 0;
            }
        }
    }
    else
    {
        int tam = ((dim_matriz * (dim_matriz - 1)) / 2);
        matriz_sem_direcao = new float[tam];
        for (int i = 0; i < tam; i++)
        {
            matriz_sem_direcao[i] = 0;
        }
    }

    vertices = new int[dim_matriz];
    for (int i = 0; i < ordem; i++)
    {
        novo_no(0, 0.0);
    }
}

void GrafoMatriz::setPesoV(float peso, int vertice)
{
    vertices[vertice] = peso;
}

void GrafoMatriz::novo_grafo(const std::string &arquivo)
{
    ifstream arquivo_grafo(arquivo);
    int grau; // Não tenho certeza o que seria grau no grafo
    int comp_conexas;
    bool completo;
    bool bipartido;
    bool arvore;
    bool aresta_ponte;
    bool vertice_articulacao;
    if (arquivo_grafo.is_open())
    {
        arquivo_grafo >> grau >> ordem >> direcionado >> comp_conexas;
        arquivo_grafo >> vertices_ponderados >> arestas_ponderadas >> completo >> bipartido >> arvore;
        arquivo_grafo >> aresta_ponte >> vertice_articulacao;
        inicializar_vertices(ordem);
        srand(time(0));

        if (vertices_ponderados)
        {
            for (int i = 0; i < ordem; i++)
            {
                vertices[i] = gerar_numero_aleatorio(1, 10);
            }
        }

        /* Se for completo, assume-se apenas um componente conexo, não bipartido/arvore e etc. */
        if (completo)
        {
            for (int i = 0; i < ordem; i++)
            {
                for (int j = 0; j < ordem; j++)
                {
                    if (i != j)
                    {
                        if (arestas_ponderadas)
                        {
                            setAresta(i, gerar_numero_aleatorio(1, 10), j);
                        }
                        else
                        {
                            setAresta(i, 1, j);
                        }
                    }
                }
            }
            return;
        }

        if (bipartido)
        {
            int tamComp = ordem / comp_conexas;
            int indice = 0;
            for (int i = 0; i < comp_conexas; i++)
            {
                int tam = tamComp;
                if (i == 0)
                {
                    tam += (ordem % comp_conexas) / 2 + (ordem % comp_conexas) % 2;
                }

                if (i == comp_conexas - 1)
                {
                    tam += (ordem % comp_conexas) / 2;
                }

                for (int j = indice; j < tam + indice; j++)
                {
                    int k = 0;
                    while (getGrauV(j + 1) < grau && j + k <= tam)
                    {
                        k++;
                        if (arestas_ponderadas)
                        {
                            setAresta(j, gerar_numero_aleatorio(1, 10), j + k);
                        }
                        else
                        {
                            setAresta(j, 1, j + k);
                        }
                    }
                    j = j + k - 1;
                }

                indice += tam;
            }
            return;
        }
        /*
           Se não é árvore ou bipartido:
           Tenta dividir componentes em partes iguais
        */

        int tamComp = ordem / comp_conexas;
        /* no caso de possuir aresta_ponte, devemos ter certeza que sobra um componente conexo pra conectar no outro */
        if (aresta_ponte)
        {
            /* diminui grau para garantir que não ultrapassem o maximo */
            grau--;
            tamComp = ordem / (comp_conexas + 1);
            comp_conexas++;

            int verticeAtual = 0;
            for (int i = 0; i < comp_conexas; i++)
            {
                int tam = tamComp;
                if (i == 0)
                {
                    /* estranho, mas evita casos de terem componentes conexas de tamanho 2 no início ou final
                       o que causa vertices de ariculacao indesejados
                    */
                    tam += (ordem % comp_conexas) / 2 + (ordem % comp_conexas) % 2;
                }
                if (i == comp_conexas - 1)
                {
                    tam += (ordem % comp_conexas) / 2;
                }

                int grauComp = 0;
                /* para quando todos os componentes chegam ao grau máximo e temos uma componente cíclica */
                while (grauComp < grau && grauComp < tam - 1)
                {
                    for (int j = verticeAtual; j < tam + verticeAtual; j++)
                    {
                        int proxVizinho = j + 1;

                        if (proxVizinho >= tam + verticeAtual)
                        {
                            proxVizinho = verticeAtual;
                        }

                        while (get_aresta(j, proxVizinho) != 0)
                        {
                            proxVizinho++;
                            if (proxVizinho >= tam + verticeAtual)
                            {
                                proxVizinho = verticeAtual;
                            }
                            if (proxVizinho == j)
                                break;
                        }

                        if (proxVizinho == j)
                        {
                            continue;
                        }

                        if (arestas_ponderadas)
                        {
                            setAresta(j, gerar_numero_aleatorio(1, 10), proxVizinho);
                        }
                        else
                        {
                            setAresta(j, 1, proxVizinho);
                        }
                    }

                    grauComp = getGrauV(verticeAtual + 1);
                }
                verticeAtual += tam;
            }
            int ultimo = ordem - 1;
            setAresta(0, 1, ultimo);
            if (arestas_ponderadas)
                setAresta(0, gerar_numero_aleatorio(1, 10), ultimo);
            return;
        }

        /*
        Caso de ter vértice de articulação, mas não aresta ponte, bem estranho e difícil de ocorrer

        Precisa ter no mínimo 5 vértices e 1 componente conexo,
        tendo que dividir corretamente caso haja mais que um.
        */

        if (vertice_articulacao)
        {
            if (ordem < 5 && comp_conexas >= 1)
            {
                cout << "Impossível criar grafo com vértice de articulação sem aresta ponte" << endl;
                return;
            }

            int tamanho_maior = ordem - comp_conexas + 1;

            /* reservando os primeiros 3 e tendo o mínimo grau possível */

            for (int i = 0; i < 3; i++)
            {
                if (i == 2)
                {
                    if (arestas_ponderadas)
                    {
                        setAresta(i, gerar_numero_aleatorio(1, 10), 0);
                    }
                    else
                    {
                        setAresta(i, 1, 0);
                    }
                }
                else
                {
                    if (arestas_ponderadas)
                    {
                        setAresta(i, gerar_numero_aleatorio(1, 10), i + 1);
                    }
                    else
                    {
                        setAresta(i, 1, i + 1);
                    }
                }
            }

            /* os restantes do grupo principal se juntam em um único componente */
            for (int i = 3; i < tamanho_maior; i++)
            {
                if (i == tamanho_maior - 1)
                {
                    if (arestas_ponderadas)
                    {
                        setAresta(i, gerar_numero_aleatorio(1, 10), 3);
                    }
                    else
                    {
                        setAresta(i, 1, 3);
                    }
                }
                else
                {
                    if (arestas_ponderadas)
                    {
                        setAresta(i, gerar_numero_aleatorio(1, 10), i + 1);
                    }
                    else
                    {
                        setAresta(i, 1, i + 1);
                    }
                }
            }

            /* os restantes são componentes isolados */

            /* agora o primeiro vértice conecta com vértices do segundo grupo
               até chegar no grau maximo */

            for (int i = 3; i < tamanho_maior; i++)
            {
                if (grau == getGrauV(1))
                {
                    break;
                }
                if (arestas_ponderadas)
                {
                    setAresta(0, gerar_numero_aleatorio(1, 10), i);
                }
                else
                {
                    setAresta(0, 1, i);
                }
            }

            return;
        } 
    }
}

float GrafoMatriz::get_aresta(int i, int j) const
{
    // Suporte a grafos direcionados e não direcionados

    if (i == j)
    {
        return 0;
    }
    if (direcionado)
    {
        return matriz[i][j];
    }
    else
    {
        int n = ordem;
        if (i < j)
        {
            int index = (n * (n - 1)) / 2 - ((n - i) * (n - i - 1)) / 2 + (j - i - 1);
            return matriz_sem_direcao[index];
        }
        else
        {
            int index = (n * (n - 1)) / 2 - ((n - j) * (n - j - 1)) / 2 + (i - j - 1);
            return matriz_sem_direcao[index];
        }
    }
}

bool GrafoMatriz::existe_Aresta(int i, int j)
{
    // Suporte a grafos direcionados e não direcionados
    i = i-1;
    j = j-1;
    if (i == j)
    {
        return false;
    }
    if (direcionado)
    {   
        if(matriz[i][j] == 0)
            return false;
        return true;
    }
    else
    {
        if (i < j)
        {
            if(matriz[i][j] == 0)
                return false;
            return true;
            
        }
        else
        {
            return false;
        }
    }
}

float GrafoMatriz::get_Pesoaresta(int i, int j)
{
    // Suporte a grafos direcionados e não direcionados
    i = i-1;
    j = j-1;
    if (i == j)
    {
        return 1000000000;
    }
    if (direcionado)
    {   
        if(matriz[i][j] == 0)
            return 1000000000;
        return matriz[i][j];
    }
    else
    {
        int n = ordem;
        if (i < j)
        {
            if(matriz[i][j] == 0)
                return 1000000000;
            int index = (n * (n - 1)) / 2 - ((n - i) * (n - i - 1)) / 2 + (j - i - 1);
            return matriz_sem_direcao[index];
            
        }
        else
        {
            return 1000000000;
        }
    }
}

void GrafoMatriz::setAresta(int i, float valor, int j)
{
    i = i-1;
    j = j-1;
    if (i == j)
    {
        return;
    }
    // Supote a grafos direcionados e não direcionados
    if (direcionado)
    {
        matriz[i][j] = valor;
    }
    else
    {
        int n = ordem;
        if (i < j)
        {
            int index = (n * (n - 1)) / 2 - ((n - i) * (n - i - 1)) / 2 + (j - i - 1);
            matriz_sem_direcao[index] = valor;
        }
        else
        {
            int index = (n * (n - 1)) / 2 - ((n - j) * (n - j - 1)) / 2 + (i - j - 1);
            matriz_sem_direcao[index] = valor;
        }
    }
}

int GrafoMatriz::getGrauV(int vertice)
{
    int grau = 0;
    for (int i = 0; i < dim_matriz; i++)
    {
        if (get_aresta(vertice, i) != 0)
        {
            grau++;
        } else if (get_aresta(i, vertice) != 0) {
            grau++;
        }
    }
    return grau;
}

void GrafoMatriz::dfs(int vertice, bool *visitado)
{
    visitado[vertice] = true;
    for (int i = 0; i < ordem; i++)
    {
        if (!visitado[i] && get_aresta(vertice, i) != 0)
        {
            dfs(i, visitado);
        }
    }
    return;
}

void GrafoMatriz::dfs_ordem(int vertice, bool *visitado, int *pilha, int &topo)
{
    visitado[vertice] = true;
    for (int i = 0; i < ordem; i++)
    {
        if (!visitado[i] && get_aresta(vertice, i) != 0)
        {
            dfs_ordem(i, visitado, pilha, topo);
        }
    }
    pilha[++topo] = vertice;
    return;
}

int GrafoMatriz::conta_transposto(bool *visitado, int *pilha, int &topo)
{
    GrafoMatriz transposto(ordem, true, vertices_ponderados, arestas_ponderadas);
    for (int i = 0; i < ordem; ++i)
    {
        for (int j = 0; j < ordem; j++)
        {
            if (matriz[i][j] != 0)
            {
                float peso = matriz[i][j];
                transposto.setAresta(j + 1, peso, i + 1);
            }
        }
    }
    for (int i = 0; i < ordem; i++)
    {
        visitado[i] = false;
    }
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

/*
void GrafoMatriz::salva_grafo(std::ofstream &saida) const
{
    saida << ordem << " " << direcionado << " "
          << vertices_ponderados << " " << arestas_ponderadas << std::endl;

    if (vertices_ponderados)
    {
        for (int i = 0; i < ordem; ++i)
        {
            saida << vertices[i] << " ";
        }
        saida << std::endl;
    }

    for (int i = 0; i < ordem; ++i)
    {
        for (int j = 0; j < ordem; ++j)
        {
            if (get_aresta(i, j) != 0)
            {
                saida << i + 1 << " " << j + 1 << " ";
                if (arestas_ponderadas)
                {
                    saida << get_aresta(i, j);
                }
                saida << std::endl;
            }
        }
    }
}*/

void GrafoMatriz::deleta_no(int no){
    int indexNo =  no - 1;
    if(indexNo < 0 || indexNo >= ordem){
        cout << "Vértice não encontrado" << endl;
        return;
    }
    for(int i = indexNo; i < ordem - 1; i++){
        vertices[i] = vertices[i + 1];
    }

    
    if(direcionado){
        for(int i = 0; i< ordem; i++){
            for(int j = indexNo; j < ordem - 1; j++){
                int proxJ = j + 1;
                matriz[i][j] = matriz[i][proxJ];
            }
        }
        for(int i = indexNo; i < ordem - 1; i++){
            matriz[i] = matriz[i + 1];
        }
    }else{
        int n = ordem;
        int index = indexNo;
        for (int i = indexNo; i < ordem; i++) {
            for (int j = i + 1; j < ordem; j++) {
                if (i != indexNo && j != indexNo) {
                    int indexAtual = (n * (n - 1)) / 2 - ((n - i) * (n - i - 1)) / 2 + (j - i - 1);
                    matriz_sem_direcao[index] = matriz_sem_direcao[indexAtual];
                    index++;
                }
            }
        }
    }
    
    ordem--;
    
}

/*void GrafoMatriz::imprime_vertices(){
    cout << "=========================" << endl;
    if(!direcionado){
        for(int i = 0; i < ordem ; i++){
            
           for(int j = 0; j < ordem; j++){
            cout << get_aresta(i, j) << " ";
           }
            cout << endl;
        }
        cout << "=========================" << endl;
    }   
}*/


void GrafoMatriz::aumentar_matriz() {
    int nova_dim = dim_matriz * 2;
    if (direcionado)
    {
        float **nova_matriz = new float *[nova_dim];
        for (int i = 0; i < nova_dim; i++)
        {
            nova_matriz[i] = new float[nova_dim];
            for (int j = 0; j < nova_dim; j++)
            {
                if (i < dim_matriz && j < dim_matriz)
                {
                    nova_matriz[i][j] = matriz[i][j];
                }
                else
                {
                    nova_matriz[i][j] = 0;
                }
            }
        }
        for (int i = 0; i < dim_matriz; i++)
        {
            delete[] matriz[i];
        }
        delete[] matriz;
        matriz = nova_matriz;
    }
    else
    {
        int tam = (nova_dim * (nova_dim - 1)) / 2;
        float *nova_matriz_sem_direcao = new float[tam];
        for (int i = 0; i < tam; i++)
        {
            if (i < (dim_matriz * (dim_matriz - 1)) / 2)
            {
                nova_matriz_sem_direcao[i] = matriz_sem_direcao[i];
            }
            else
            {
                nova_matriz_sem_direcao[i] = 0;
            }
        }
        delete[] matriz_sem_direcao;
        matriz_sem_direcao = nova_matriz_sem_direcao;
    }

    int *novos_vertices = new int[nova_dim];
    for (int i = 0; i < nova_dim; i++)
    {
        if (i < dim_matriz)
        {
            novos_vertices[i] = vertices[i];
        }
        else
        {
            novos_vertices[i] = 0;
        }
    }
    delete[] vertices;
    vertices = novos_vertices;

    dim_matriz = nova_dim;
}

void GrafoMatriz::novo_no(int indice, float peso)
{
    while (ordem >= dim_matriz || indice >= dim_matriz)
    {
        aumentar_matriz();
    }
    if (vertices_ponderados)
        vertices[indice] = peso;
    ordem++;
}

void GrafoMatriz::nova_aresta(int origem, float peso, int destino)
{
    // Assumindo 1-indexado
    origem = origem;
    destino = destino;
    if (direcionado)
    {
        setAresta(origem, 1,destino);    
    } else {
        setAresta(origem,peso, destino);
    }
    
    
}

void GrafoMatriz::deleta_aresta(int origem, int destino) {
    // Assumindo 1-indexado
    origem = origem;
    destino = destino;
    setAresta(origem, 0, destino);
}