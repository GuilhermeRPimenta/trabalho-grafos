import networkx as nx
import random

def load_graph(file_path):
    G = nx.Graph()
    with open(file_path, 'r') as f:
        for line in f:
            if line.startswith('#'):
                continue  # Ignorar cabeçalhos
            parts = line.strip().split()
            if len(parts) != 2:
                print(f"Linha ignorada (formato inválido): {line.strip()}")
                continue
            try:
                from_node, to_node = map(int, parts)
                G.add_edge(from_node, to_node)
            except ValueError:
                print(f"Erro ao converter valores para inteiro: {line.strip()}")
    return G

def get_largest_connected_component(G):
    components = list(nx.connected_components(G))
    print(f"Número total de componentes conexos: {len(components)}")
    largest_component = max(components, key=len)  # Encontra o maior subgrafo conexo
    print(f"Tamanho do maior componente conexo: {len(largest_component)} nós")
    return G.subgraph(largest_component).copy()

def assign_random_clusters(G, num_clusters):
    nodes = list(G.nodes())
    random.shuffle(nodes)  # Embaralhar os nós para distribuir os clusters aleatoriamente
    cluster_map = {node: i % num_clusters for i, node in enumerate(nodes)}
    return cluster_map

def normalize_node_ids(G):
    node_mapping = {old_id: new_id for new_id, old_id in enumerate(sorted(G.nodes()))}
    G = nx.relabel_nodes(G, node_mapping)
    return G, node_mapping

def save_clustered_graph(file_path, G, cluster_map, node_mapping):
    with open(file_path, 'w') as f:
        f.write(f"{G.number_of_nodes()}\t{G.number_of_edges()}\n")  # Primeira linha com ordem do grafo
        for from_node, to_node in G.edges():
            new_from_node = from_node % G.number_of_nodes()
            new_to_node = to_node % G.number_of_nodes()
            cluster = cluster_map.get(from_node, -1)
            f.write(f"{new_from_node}\t{new_to_node}\t{cluster}\n")

# Configurações
input_file = "entradas/com-LiveJournal_Communities_top5000.txt"
output_file = "com-LiveJournal_Communities_top5000.txt"
num_clusters = 500  # Defina o número de clusters desejado

# Processamento
graph = load_graph(input_file)
print(f"Número total de nós no grafo original: {graph.number_of_nodes()}")
print(f"Número total de arestas no grafo original: {graph.number_of_edges()}")

largest_subgraph = get_largest_connected_component(graph)  # Obtém maior subgrafo conexo
print(f"Número total de nós no maior subgrafo conexo: {largest_subgraph.number_of_nodes()}")
print(f"Número total de arestas no maior subgrafo conexo: {largest_subgraph.number_of_edges()}")

largest_subgraph, node_mapping = normalize_node_ids(largest_subgraph)  # Normaliza os IDs dos nós
cluster_map = assign_random_clusters(largest_subgraph, num_clusters)  # Atribui clusters aleatórios

# Salvar resultados
save_clustered_graph(output_file, largest_subgraph, cluster_map, node_mapping)
print(f"Clusterização concluída. Arquivo salvo como {output_file}")
