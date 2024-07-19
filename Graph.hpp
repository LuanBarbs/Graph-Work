#ifndef GRAPH_HPP_INCLUDED
#define GRAPH_HPP_INCLUDED

#include "Node.hpp"
#include "defines.hpp"

using namespace std;

// GRAFO
class Graph {
    public:
        /* Assinatura dos m�todos b�sicos para o funcionamento da classe. */
        Graph(ifstream& instance); // Construtor que inicializa o grafo a partir de uma inst�ncia lida de um arquivo.
        Graph();                   // Construtor padr�o.
        ~Graph();                  // Destrutor padr�o.

        void remove_node(size_t node_id);                                    // Remove o n� com base no identificador.
        void remove_edge(size_t node_id_1, size_t node_id_2);                // Remove a aresta entre os n�s especificados.
        void add_node(size_t node_id, float weight = 0);                     // Adiciona um novo n� com identificador e peso.
        void add_edge(size_t node_id_1, size_t node_id_2, float weight = 0); // Adiciona uma nova aresta entre os n�s especificados.
        void print_graph(ofstream& output_file);                             // Imprime a representa��o de um grafo em um arquivo.
        void print_graph();                                                  // Imprime a representa��o do grafo no console.

        int conected(size_t node_id_1, size_t node_id_2); // Veficia se existe uma conex�o direta entre dois n�s.

        /* Assinatura de novos m�todos criados para o trabalho.*/
        Node* search_node_by_id(size_t node_id); // Procura no grafo o n� com o Id fornecido.
        vector<size_t> search_nodes_in_adjacency_lists(size_t target_id); // Procura no grafo por n�s que tenham o id passado na sua lista de adjac�ncia.
        void setDirected(bool directed); // M�todo para alterar o atributo _directed (define o grafo como direcionado ou n�o).
        void print_graph_infos(); // Imprime informa��es gerais sobre o grafo.
        void print_node_infos(size_t node_id); // Imprime informa��es gerais sobre o n� passado como par�metro.

        vector<size_t> direct_transitive_closure(size_t node_id);   // M�todo que retorna o fecho transitivo direto de um v�rtice.
        vector<size_t> indirect_transitive_closure(size_t node_id); // M�todo que retorna o fecho transitivo indireto de um v�rtice.

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
