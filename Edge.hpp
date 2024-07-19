#ifndef EDGE_HPP_INCLUDED
#define EDGE_HPP_INCLUDED

#include "defines.hpp"

// ARESTAS
struct Edge {
    Edge *_next_edge;   // Ponteiro para outra aresta (encadeamento das arestas).
    float _weight;      // Armazena o peso de cada aresta.
    size_t _target_id;  // Um tipo inteiro sem sinal para armazenar o identificador do nó alvo da aresta.
};

#endif // EDGE_HPP_INCLUDED
