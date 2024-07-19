#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED

#include "Node.hpp"
#include "defines.hpp"

using namespace std;

// GRAFO
class Graph {
    public:
        /* Assinatura dos métodos básicos para o funcionamento da classe. */
        Graph(ifstream& instance); // Construtor que inicializa o grafo a partir de uma instância lida de um arquivo.
        Graph();                   // Construtor padrão.
        ~Graph();                  // Destrutor padrão.

        void remove_node(size_t node_id);                                    // Remove o nó com base no identificador.
        void remove_edge(size_t node_id_1, size_t node_id_2);                // Remove a aresta entre os nós especificados.
        void add_node(size_t node_id, float weight = 0);                     // Adiciona um novo nó com identificador e peso.
        void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0); // Adiciona uma nova aresta entre os nós especificados.
        void print_graph(ofstream& output_file);                             // Imprime a representação de um grafo em um arquivo.
        void print_graph();                                                  // Imprime a representação do grafo no console.

        int conected(size_t node_id_1, size_t node_id_2); // Veficia se existe uma conexão direta entre dois nós.

        /* Assinatura de novos métodos criados para o trabalho.*/
        Node* search_node_by_id(size_t node_id); // Procura no grafo o nó com o Id fornecido.
        vector<size_t> search_nodes_in_adjacency_lists(size_t target_id); // Procura no grafo por nós que tenham o id passado na sua lista de adjacência.
        void setDirected(bool directed); // Método para alterar o atributo _directed (define o grafo como direcionado ou não).
        void print_graph_infos(); // Imprime informações gerais sobre o grafo.
        void print_node_infos(size_t node_id); // Imprime informações gerais sobre o nó passado como parâmetro.

        vector<size_t> direct_transitive_closure(size_t node_id);   // Método que retorna o fecho transitivo direto de um vértice.
        vector<size_t> indirect_transitive_closure(size_t node_id); // Método que retorna o fecho transitivo indireto de um vértice.

    private:
        size_t _number_of_nodes; // Representa o número de nós.
        size_t _number_of_edges; // Representa o número de arestas.
        bool _directed;          // Diz se o grafo é direcionado.
        bool _weighted_edges;    // Diz se o grafo utiliza arestas com pesos.
        bool _weighted_nodes;    // Diz se o grafo utiliza nós com pesos.
        Node *_first;            // Ponteiro para o primeiro nó do grafo.
        Node *_last;             // Ponteiro para o último nó do grafo.
};

#endif // GRAPH_HPP_INCLUDED
