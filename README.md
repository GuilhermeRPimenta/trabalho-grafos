# A FAZER
- *Modificar a criação do grafo e leitura para ser AGMG* para Fazer os 3 Algoritimos: Guloso, Reativo e Aleatório.



# Trabalho de Teoria dos Grafos - UFJF

## Descrição
Este projeto foi desenvolvido para a disciplina **DCC059 - Teoria dos Grafos** da **Universidade Federal de Juiz de Fora (UFJF)**. O objetivo principal é a implementação de uma aplicação em **C++** para manipulação de grafos.

A aplicação utiliza duas abordagens distintas para a representação dos grafos:
- **Matriz de adjacência**
- **Lista encadeada**

O projeto atende aos requisitos definidos para a avaliação do curso e explora conceitos fundamentais da **Teoria dos Grafos**.

---

## Estrutura do Projeto
O projeto está organizado da seguinte forma:

```
📂 docs/
   ├── documentacao.pdf    # Descrição detalhada do projeto
   ├── apresentacao.pdf    # Slides da apresentação
   ├── contribuidores.pdf  # Contribuições de cada membro do grupo
   ├── descricao.pdf       # Panorama geral do trabalho

📂 include/
   ├── *.h, *.hpp, *.tpp   # Arquivos de cabeçalho usados na implementação

📂 src/
   ├── grafo_matriz.cpp    # Implementação da estrutura de matriz de adjacência
   ├── grafo_lista.cpp     # Implementação da estrutura de lista encadeada

📂 entradas/
   ├── grafo.txt           # Arquivos de entrada para os grafos

📄 main.cpp                # Ponto de entrada do programa
📄 README.md               # Este arquivo com informações sobre o projeto
```

---

## Funcionalidades Implementadas

A aplicação conta com diversas funcionalidades para análise e manipulação de grafos:

- `n_conexo` - Identifica a quantidade de componentes conexas do grafo.
- `get_grau` - Retorna o grau do grafo.
- `get_ordem` - Retorna a ordem do grafo.
- `eh_direcionado` - Verifica se o grafo é direcionado.
- `vertice_ponderado` - Informa se os vértices possuem peso.
- `aresta_ponderada` - Informa se as arestas possuem peso.
- `eh_completo` - Determina se o grafo é completo.
- `carrega_grafo` - Lê um arquivo de entrada (`grafo.txt`) e carrega o grafo.

---

## Compilação e Execução

Para compilar e executar o programa via terminal, utilize os seguintes comandos:

### Usando Matriz de Adjacência
```bash
./main.out -d -m grafo.txt
```

### Usando Lista Encadeada
```bash
./main.out -d -l grafo.txt
```

### Exemplo de Saída Esperada
```yaml
Grau: 3  
Ordem: 3  
Direcionado: Sim  
Vértices ponderados: Sim  
Arestas ponderadas: Sim  
Completo: Sim  
```

---

## Boas Práticas e Avaliação
O código segue boas práticas de **Orientação a Objetos**, incluindo o uso adequado de **herança** e **encapsulamento**. Além disso:
- O uso eficiente de memória será avaliado com **Valgrind**.
- As bibliotecas permitidas incluem:
  - `<fstream>`, `<iostream>`, `<iomanip>`, `<cmath>`, `<cstdlib>`, `<cstdarg>`, `<ctime>`, `<string>`.
- Os grafos **não aceitam arestas múltiplas ou laços**, garantindo integridade na estrutura.

---

## Conclusão
Este trabalho visa explorar conceitos fundamentais da **Teoria dos Grafos** por meio de uma implementação eficiente e bem estruturada. Ele proporciona uma base sólida para futuros projetos e estudos na área.

---

**Desenvolvido para a disciplina DCC059 - Teoria dos Grafos | UFJF**

