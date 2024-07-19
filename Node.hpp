#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include "Edge.hpp"
#include "defines.hpp"

// N�
struct Node {
    size_t _number_of_edges; // Armazena o n�mero de arestas conectadas a este n�.
    size_t _id;              // Armazena um identificador �nico para o n�.
    float _weight;           // Armazena um peso associado ao n�.
    Edge *_first_edge;       // Ponteiro para a primeira aresta na lista de adjac�ncias deste n�.
    Node *_next_node;        // Ponteiro para o pr�ximo n� na lista (encadeamento dos n�s).
    Node *_previous_node;    // Ponteiro para o n� anterior na lista (permite navega��o biderecional na lista).
};

#endif // NODE_HPP_INCLUDED
