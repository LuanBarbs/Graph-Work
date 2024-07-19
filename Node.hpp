#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include "Edge.hpp"
#include "defines.hpp"

// NÓ
struct Node {
    size_t _number_of_edges; // Armazena o número de arestas conectadas a este nó.
    size_t _id;              // Armazena um identificador único para o nó.
    float _weight;           // Armazena um peso associado ao nó.
    Edge *_first_edge;       // Ponteiro para a primeira aresta na lista de adjacências deste nó.
    Node *_next_node;        // Ponteiro para o próximo nó na lista (encadeamento dos nós).
    Node *_previous_node;    // Ponteiro para o nó anterior na lista (permite navegação biderecional na lista).
};

#endif // NODE_HPP_INCLUDED
