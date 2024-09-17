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

        // Assinatura de novos m�todos criados para o trabalho.
        Node* search_node_by_id(size_t node_id);                          // Procura no grafo o n� com o Id fornecido.

        size_t get_number_of_nodes() { return _number_of_nodes; }; // Retorna o n�mero de n�s do grafo.
        Node* get_first() { return _first; };                      // Retorna o primeiro n� do grafo.
        bool isDirected() { return _directed; };                   // Retorna se o grafo � direcionado.
        bool isWeightedEdges() { return _weighted_edges; };        // Retorna se o grafo � pesado nas arestas.
        bool isWeightedNodes() { return _weighted_nodes; };        // Retorna se o grafo � pesado nos v�rtices.

    private:
        size_t _number_of_nodes; // Representa o n�mero de n�s.
        size_t _number_of_edges; // Representa o n�mero de arestas.
        bool _directed;          // Diz se o grafo � direcionado.
        bool _weighted_edges;    // Diz se o grafo utiliza arestas com pesos.
        bool _weighted_nodes;    // Diz se o grafo utiliza n�s com pesos.
        Node *_first;            // Ponteiro para o primeiro n� do grafo.
        Node *_last;             // Ponteiro para o �ltimo n� do grafo.
};

#endif // GRAPH_HPP_INCLUDED
