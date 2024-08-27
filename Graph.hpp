#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED

#include "Node.hpp"
#include "defines.hpp"

using namespace std;

// GRAFO
class Graph {
    public:
        // Assinatura dos métodos básicos para o funcionamento da classe.
        Graph(bool isDirected, bool isWeightedEdges, bool IsWeightedNodes); // Construtor padrão.
        Graph(ifstream& instance, bool isDirected, bool isWeightedEdges, bool IsWeightedNodes); // Construtor que inicializa o grafo a partir de uma instância lida de um arquivo.
        ~Graph(); // Destrutor padrão.

        void remove_node(size_t node_id);                                    // Remove o nó com base no identificador.
        void remove_edge(size_t node_id_1, size_t node_id_2);                // Remove a aresta entre os nós especificados.
        void add_node(size_t node_id, float weight = 0);                     // Adiciona um novo nó com identificador e peso.
        void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0); // Adiciona uma nova aresta entre os nós especificados.
        void print_graph(ofstream& output_file);                             // Imprime a representação de um grafo em um arquivo.
        void print_graph();                                                  // Imprime a representação do grafo no console.

        int conected(size_t node_id_1, size_t node_id_2); // Verifica se existe uma conexão direta entre dois nós.

        // Assinatura de novos métodos criados para o trabalho.
        Node* search_node_by_id(size_t node_id);                          // Procura no grafo o nó com o Id fornecido.
        void print_graph_infos();                                         // Imprime informações gerais sobre o grafo.
        void print_node_infos(size_t node_id);                            // Imprime informações gerais sobre o nó passado como parâmetro.

        void direct_transitive_closure(size_t node_id);   // Método para encontrar o fecho transitivo direto de um vértice.
        void indirect_transitive_closure(size_t node_id); // Método para encontrar o fecho transitivo indireto de um vértice.

        void dijkstra(size_t node_id_1, size_t node_id_2);  // Método para encontrar o caminho mínimo entre os dois vértices usando o algoritmo de Djkstra.
        void floyd(size_t node_id_1, size_t node_id_2);     // Método para encontrar o caminho mínimo entre os dois vértices usando o algoritmo de Floyd.

        void calculate_radius_diameter_center_periphery();  // Método para encontrar raio, diâmetro, centro e periferia do grafo.

        void articulation_points(); // Método para encontrar vértices de articulação.

        void prim_minimum_spanning_tree(vector<size_t> &subset_X);    // Método para encontrar a AGM sobre o subgrafo vértice-induzido usando o algoritmo de Prim.
        void kruskal_minimum_spanning_tree(vector<size_t> &subset_X); // Método para encontrar a AGM sobre o subgrafo vértice-induzido usando o algoritmo de Kruskal.

        void dfs_tree_with_back_edges(size_t start_node_id);  // Método que utiliza busca em profundidade (DFS) para gerar a árvore pela ordem de caminhamento em profundidade.

    private:
        size_t _number_of_nodes; // Representa o número de nós.
        size_t _number_of_edges; // Representa o número de arestas.
        bool _directed;          // Diz se o grafo é direcionado.
        bool _weighted_edges;    // Diz se o grafo utiliza arestas com pesos.
        bool _weighted_nodes;    // Diz se o grafo utiliza nós com pesos.
        Node *_first;            // Ponteiro para o primeiro nó do grafo.
        Node *_last;             // Ponteiro para o último nó do grafo.

        // Método auxiliar de busca em profundidade(DFS) para encontrar pontos de articulação.
        void DFSArticulation(size_t u, vector<bool> &visited, vector<int> &disc, vector<int> &low, vector<int> &parent, vector<bool> &ap);

        // Método auxiliar de busca em profundidade(DFS).
        void dfs_tree_with_back_edges_recursive(size_t u, vector<bool> &visited, vector<size_t> &parent, vector<pair<size_t, size_t>> &tree_edges, vector<pair<size_t, size_t>> &back_edges);
};

#endif // GRAPH_HPP_INCLUDED
