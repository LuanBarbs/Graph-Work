#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED

#include "Node.hpp"
#include "defines.hpp"

using namespace std;

// GRAFO
class Graph {
    public:
        // Assinatura dos m�todos b�sicos para o funcionamento da classe.
        Graph(bool isDirected, bool isWeightedEdges, bool IsWeightedNodes); // Construtor padr�o.
        Graph(ifstream& instance, bool isDirected, bool isWeightedEdges, bool IsWeightedNodes); // Construtor que inicializa o grafo a partir de uma inst�ncia lida de um arquivo.
        ~Graph(); // Destrutor padr�o.

        void remove_node(size_t node_id);                                    // Remove o n� com base no identificador.
        void remove_edge(size_t node_id_1, size_t node_id_2);                // Remove a aresta entre os n�s especificados.
        void add_node(size_t node_id, float weight = 0);                     // Adiciona um novo n� com identificador e peso.
        void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0); // Adiciona uma nova aresta entre os n�s especificados.
        void print_graph(ofstream& output_file);                             // Imprime a representa��o de um grafo em um arquivo.
        void print_graph();                                                  // Imprime a representa��o do grafo no console.

        int conected(size_t node_id_1, size_t node_id_2); // Verifica se existe uma conex�o direta entre dois n�s.

        // Assinatura de novos m�todos criados para o trabalho.
        Node* search_node_by_id(size_t node_id);                          // Procura no grafo o n� com o Id fornecido.
        void print_graph_infos();                                         // Imprime informa��es gerais sobre o grafo.
        void print_node_infos(size_t node_id);                            // Imprime informa��es gerais sobre o n� passado como par�metro.

        void direct_transitive_closure(size_t node_id);   // M�todo para encontrar o fecho transitivo direto de um v�rtice.
        void indirect_transitive_closure(size_t node_id); // M�todo para encontrar o fecho transitivo indireto de um v�rtice.

        void dijkstra(size_t node_id_1, size_t node_id_2);  // M�todo para encontrar o caminho m�nimo entre os dois v�rtices usando o algoritmo de Djkstra.
        void floyd(size_t node_id_1, size_t node_id_2);     // M�todo para encontrar o caminho m�nimo entre os dois v�rtices usando o algoritmo de Floyd.

        void calculate_radius_diameter_center_periphery();  // M�todo para encontrar raio, di�metro, centro e periferia do grafo.

        void articulation_points(); // M�todo para encontrar v�rtices de articula��o.

        void prim_minimum_spanning_tree(vector<size_t> &subset_X);    // M�todo para encontrar a AGM sobre o subgrafo v�rtice-induzido usando o algoritmo de Prim.
        void kruskal_minimum_spanning_tree(vector<size_t> &subset_X); // M�todo para encontrar a AGM sobre o subgrafo v�rtice-induzido usando o algoritmo de Kruskal.

        void dfs_tree_with_back_edges(size_t start_node_id);  // M�todo que utiliza busca em profundidade (DFS) para gerar a �rvore pela ordem de caminhamento em profundidade.

    private:
        size_t _number_of_nodes; // Representa o n�mero de n�s.
        size_t _number_of_edges; // Representa o n�mero de arestas.
        bool _directed;          // Diz se o grafo � direcionado.
        bool _weighted_edges;    // Diz se o grafo utiliza arestas com pesos.
        bool _weighted_nodes;    // Diz se o grafo utiliza n�s com pesos.
        Node *_first;            // Ponteiro para o primeiro n� do grafo.
        Node *_last;             // Ponteiro para o �ltimo n� do grafo.

        // M�todo auxiliar de busca em profundidade(DFS) para encontrar pontos de articula��o.
        void DFSArticulation(size_t u, vector<bool> &visited, vector<int> &disc, vector<int> &low, vector<int> &parent, vector<bool> &ap);

        // M�todo auxiliar de busca em profundidade(DFS).
        void dfs_tree_with_back_edges_recursive(size_t u, vector<bool> &visited, vector<size_t> &parent, vector<pair<size_t, size_t>> &tree_edges, vector<pair<size_t, size_t>> &back_edges);
};

#endif // GRAPH_HPP_INCLUDED
