import networkx as nx
import random

def load_graph(file_path):
    G = nx.Graph()
    with open(file_path, 'r') as f:
        for line in f:
            if line.startswith('#'):
                continue  # Ignorar cabeçalhos
            from_node, to_node = map(int, line.strip().split())
            G.add_edge(from_node, to_node)
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

def save_clustered_graph(file_path, G, cluster_map):
    with open(file_path, 'w') as f:
        f.write("FromNodeId\tToNodeId\tCluster\n")
        for from_node, to_node in G.edges():
            cluster = cluster_map.get(from_node, -1)
            f.write(f"{from_node}\t{to_node}\t{cluster}\n")

def save_cluster_info(file_path, cluster_map):
    with open(file_path, 'w') as f:
        f.write("NodeId\tCluster\n")
        for node, cluster in cluster_map.items():
            f.write(f"{node}\t{cluster}\n")

# Configurações
input_file = "entradas/web-BerkStan.txt"
largest_component_file = "largest_component.txt"
output_file = "web-BerkStan_clustered_graph.txt"
cluster_info_file = "node_clusters.txt"
num_clusters = 500  # Defina o número de clusters desejado

# Processamento
graph = load_graph(input_file)
print(f"Número total de nós no grafo original: {graph.number_of_nodes()}")
print(f"Número total de arestas no grafo original: {graph.number_of_edges()}")

largest_subgraph = get_largest_connected_component(graph)  # Obtém maior subgrafo conexo
print(f"Número total de nós no maior subgrafo conexo: {largest_subgraph.number_of_nodes()}")
print(f"Número total de arestas no maior subgrafo conexo: {largest_subgraph.number_of_edges()}")

cluster_map = assign_random_clusters(largest_subgraph, num_clusters)  # Atribui clusters aleatórios

# Salvar resultados
save_clustered_graph(output_file, largest_subgraph, cluster_map)
save_cluster_info(cluster_info_file, cluster_map)

print(f"Maior subgrafo conexo salvo como {largest_component_file}")
print(f"Clusterização concluída. Arquivo salvo como {output_file}")
print(f"Informações dos clusters salvas em {cluster_info_file}")
