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

        // Assinatura de novos métodos criados para o trabalho.
        Node* search_node_by_id(size_t node_id);                          // Procura no grafo o nó com o Id fornecido.

        size_t get_number_of_nodes() { return _number_of_nodes; }; // Retorna o número de nós do grafo.
        Node* get_first() { return _first; };                      // Retorna o primeiro nó do grafo.
        bool isDirected() { return _directed; };                   // Retorna se o grafo é direcionado.
        bool isWeightedEdges() { return _weighted_edges; };        // Retorna se o grafo é pesado nas arestas.
        bool isWeightedNodes() { return _weighted_nodes; };        // Retorna se o grafo é pesado nos vértices.

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
