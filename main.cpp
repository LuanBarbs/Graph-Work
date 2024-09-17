#include "Graph.hpp"
#include "defines.hpp"

using namespace std;

/// Estrutura auxiliar para ordenar arestas.
struct EdgeWithDifference {
    size_t u;
    size_t v;
    float weight_diff;
};

/// Função auxiliar para verificar adjacência.
vector<bool> check_adjacent_trees(vector<vector<Node*>>& forest, EdgeWithDifference& edge) {
    vector<bool> adjacency(forest.size(), false); // Inicializa o vetor de adjacência com falso.

    for(size_t i = 0; i < forest.size(); i++) {
        for(Node* node : forest[i]) {
            Edge* current_edge = node->_first_edge;
            while(current_edge != nullptr) {
                // Se a aresta conecta o nó com u ou v, marca como adjacente.
                if(current_edge->_target_id == edge.u || current_edge->_target_id == edge.v) {
                    adjacency[i] = true;
                    break;
                }
                current_edge = current_edge->_next_edge;
            }
        }
    }

    return adjacency;
}

/// Função auxiliar para calcular o gap de uma árvore.
float total_gap_tree(vector<Node*>& tree, Node* u = nullptr, Node* v = nullptr) {
    float min_weight = numeric_limits<float>::max();
    float max_weight = numeric_limits<float>::lowest();

    // Calcula o maior e o menor peso entre os nós da árvore.
    for (Node* node : tree) {
        if (node->_weight < min_weight) min_weight = node->_weight;
        if (node->_weight > max_weight) max_weight = node->_weight;
    }

    // Verifica se os nós u e v precisam ser considerados no cálculo do gap.
    if (u) {
        if (u->_weight < min_weight) min_weight = u->_weight;
        if (u->_weight > max_weight) max_weight = u->_weight;
    }
    if (v) {
        if (v->_weight < min_weight) min_weight = v->_weight;
        if (v->_weight > max_weight) max_weight = v->_weight;
    }

    return max_weight - min_weight;
}

/// Função auxiliar para calcular o gap total das árvores.
float calculate_total_gap(vector<vector<Node*>>& subgraphs) {
    float total_gap = 0.0f;
    for(auto& subgraph : subgraphs) {
        total_gap += total_gap_tree(subgraph);
    }
    return total_gap;
}

/// Função para a construção inicial da floresta utilizando Prim.
vector<vector<Node*>> initial_construction(Graph& graph, size_t p) {
    vector<vector<Node*>> forest; // Floresta com p árvores.

    // Verifica se o grafo está vazio.
    if(graph.get_number_of_nodes() == 0) {
        cout << "Grafo vazio!" << endl;
        return forest;
    }
    // Verifica se o p é valido.
    if(p > graph.get_number_of_nodes() / 2) {
        cout << "O numero de subgrafos eh invalido" << endl;
        return forest;
    }

    // Passo 1: Ordenar as arestas (u, v) por valores não decrescentes dos gaps |w_u - w_v|.
    vector<EdgeWithDifference> edges;
    Node* current_node = graph.get_first();
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
            // Verifica se a aresta já foi processada (evita duplicatas).
            if(current_node->_id < current_edge->_target_id) {
                Node* target_node = graph.search_node_by_id(current_edge->_target_id);
                EdgeWithDifference newEdge;
                newEdge.u = current_node->_id;
                newEdge.v = target_node->_id;
                newEdge.weight_diff = abs(current_node->_weight - target_node->_weight);
                edges.push_back(newEdge);
            }
            current_edge = current_edge->_next_edge;
        }
        current_node = current_node->_next_node;
    }
    // Ordena as arestas pela diferença de peso entre os nós (não decrescente).
    sort(edges.begin(), edges.end(), [](const EdgeWithDifference& e1, const EdgeWithDifference& e2) {
        return e1.weight_diff < e2.weight_diff;
    });

    // Passo 2: Enquanto o numero de árvores na floresta for menor que p.
    vector<bool> is_verified(graph.get_number_of_nodes(), false);
    int index = 0;
    while(forest.size() < p && index < edges.size()) {
        EdgeWithDifference edge = edges[index];
        Node* node1 = graph.search_node_by_id(edge.u);
        Node* node2 = graph.search_node_by_id(edge.v);

        // Verifica se ambos os nós ainda não foram processados.
        if(!is_verified[node1->_id - 1] && !is_verified[node2->_id -1]) {
            vector<bool> adjacency = check_adjacent_trees(forest, edge);

            // Se não houver árvores na floresta, cria a primeira árvore.
            if(forest.size() == 0) {
                vector<Node*> tree;
                tree.push_back(node1);
                tree.push_back(node2);
                forest.push_back(tree);
            } else {
                int tree_id = -1;
                float min_gap = numeric_limits<float>::max();

                 // Para cada árvore na floresta, calcula-se o gap ao adicionar os nós u e v.
                for(int i = 0; i < forest.size(); i++) {
                    if(!adjacency[i]) {
                        float current_gap = total_gap_tree(forest[i], node1, node2);
                        if(current_gap < min_gap) {
                            min_gap = current_gap;
                            tree_id = i;
                        }
                    }
                }

                // Se não for possível adicionar os nós em nenhuma árvore, cria-se uma nova árvore.
                if(tree_id == -1) {
                    vector<Node*> tree;
                    tree.push_back(node1);
                    tree.push_back(node2);
                    forest.push_back(tree);
                } else {
                    forest[tree_id].push_back(node1);
                    forest[tree_id].push_back(node2); // Adiciona os nós à árvore com o menor gap.
                }
            }
            // Marca os nós como verificados.
            is_verified[node1->_id - 1] = true;
            is_verified[node2->_id - 1] = true;
        }
        index++;
    }

    // Passo 3: Preencher as ávores com os vértices ainda não cobertos.
    for(int i = 0; i < graph.get_number_of_nodes(); i++) {
        // Verifica se o nó i ainda não foi inserido em nenhuma árvore.
        if(!is_verified[i]) {
            Node* node = graph.search_node_by_id(i+1);
            // Descobrir em qual árvore o nó i deve entrar.
            int tree_id = -1;
            float min_gap = numeric_limits<float>::max();

            // Para cada árvore, calcula o gap ao adicionar o nó.
            for(int i = 0; i < forest.size(); i++) {
                float current_gap = total_gap_tree(forest[i], node);
                if(current_gap < min_gap) {
                    min_gap = current_gap;
                    tree_id = i;
                }
            }

            // Adiciona o nó à árvore com o menor gap.
            forest[tree_id].push_back(node);
            is_verified[i] = true;
        }
    }
    return forest; // Retorna a floresta final.
}

/// Função auxiliar para calcular o impacto da troca de pares de vértices.
float calculate_swap_impact(vector<vector<Node*>>& subgraphs, size_t tree1_idx, size_t tree2_idx, Node* v1, Node* v2) {
    vector<Node*> tree1 = subgraphs[tree1_idx];
    vector<Node*> tree2 = subgraphs[tree2_idx];

    // Calcula o gap total antes da troca.
    float original_gap = calculate_total_gap(subgraphs);

    // Simula a troca de v1 e v2 entre as árvores.
    auto it1 = find(tree1.begin(), tree1.end(), v1);
    auto it2 = find(tree2.begin(), tree2.end(), v2);
    if(it1 != tree1.end() && it2 != tree2.end()) {
        // Substitui os vértices nas árvores.
        tree1.erase(it1);
        tree1.push_back(v2);
        tree2.erase(it2);
        tree2.push_back(v1);
    }

    // Calcula o gap total após a troca.
    vector<vector<Node*>> new_subgraphs = subgraphs;
    new_subgraphs[tree1_idx] = tree1;
    new_subgraphs[tree2_idx] = tree2;
    float new_gap = calculate_total_gap(new_subgraphs);

    // Retorna a diferença no gap.
    return original_gap - new_gap;
}

/// Função auxiliar para encontrar a melhor troca de pares de vértices entre as árvores.
void optimize_with_swaps(vector<vector<Node*>>& subgraphs) {
    size_t num_trees = subgraphs.size();
    bool improved = true;

    while(improved) {
        improved = false;
        float best_impact = 0.0f;
        size_t best_tree1_idx = 0;
        size_t best_tree2_idx = 0;
        Node* best_v1 = nullptr;
        Node* best_v2 = nullptr;

        // Tenta todas as possíveis trocas de pares de vértices entre as árvores.
        for(size_t i = 0; i < num_trees; ++i) {
            for(size_t j = i + 1; j < num_trees; ++j) {
                for(Node* v1 : subgraphs[i]) {
                    for(Node* v2 : subgraphs[j]) {
                        float impact = calculate_swap_impact(subgraphs, i, j, v1, v2);
                        if(impact > best_impact) {
                            best_impact = impact;
                            best_tree1_idx = i;
                            best_tree2_idx = j;
                            best_v1 = v1;
                            best_v2 = v2;
                        }
                    }
                }
            }
        }

        // Se uma melhoria foi encontrada, aplica a troca.
        if(best_impact > 0) {
            auto it1 = find(subgraphs[best_tree1_idx].begin(), subgraphs[best_tree1_idx].end(), best_v1);
            auto it2 = find(subgraphs[best_tree2_idx].begin(), subgraphs[best_tree2_idx].end(), best_v2);
            if(it1 != subgraphs[best_tree1_idx].end() && it2 != subgraphs[best_tree2_idx].end()) {
                subgraphs[best_tree1_idx].erase(it1);
                subgraphs[best_tree1_idx].push_back(best_v2);
                subgraphs[best_tree2_idx].erase(it2);
                subgraphs[best_tree2_idx].push_back(best_v1);
            }
            improved = true;
        }
    }
}

/// Função principal do algoritmo guloso.
vector<vector<Node*>> greedy_algorithm(Graph& graph, size_t p) {
    vector<vector<Node*>> subgraphs = initial_construction(graph, p);

    // Otimizar a solução usando troca de pares de vértices entre árvores.
    optimize_with_swaps(subgraphs);

    return subgraphs;
}

/// Função principal do algoritmo guloso randomizado adaptativo.
vector<vector<Node*>> randomized_greedy_algorithm(Graph& graph, size_t p, float alpha) {
}

int main(int argc, char* argv[])
{
    // Criando o grafo.
    ifstream input_file;
    input_file.open("instances_example/n100plap1i1.txt");
    Graph graph = Graph(input_file, false, true, true);

    // Número de subgrafos
    size_t p = 5;

    vector<vector<Node*>> forest = initial_construction(graph, p);

    float total_gap = 0;
    int cont = 0;
    int i = 1;
    for(vector<Node*> tree : forest) {
        cout << "Subgrafo " << i << endl;
        for(Node* node : tree) {
            cout << node->_id << " -> " << node->_weight << endl;
            cont++;
        }
        cout << endl;
        i++;
        total_gap += total_gap_tree(tree);
    }

    cout << "Gap total: " << total_gap << endl;
    cout << "Numero de nos: " << cont << endl << endl;

    vector<vector<Node*>> subgraphs = greedy_algorithm(graph, p);

    total_gap = 0;
    cont = 0;
    i = 1;
    for(vector<Node*> subgraph : subgraphs) {
        cout << "Subgrafo " << i << endl;
        for(Node* node : subgraph) {
            cout << node->_id << " -> " << node->_weight << endl;
            cont++;
        }
        cout << endl;
        i++;
        total_gap += total_gap_tree(subgraph);
    }

    cout << "Gap total: " << total_gap << endl;
    cout << "Numero de nos: " << cont << endl << endl;

//    subgraphs = randomized_greedy_algorithm(graph, p, 0.1);
//
//    total_gap = 0;
//    cont = 0;
//    i = 1;
//    for(vector<Node*> subgraph : subgraphs) {
//        cout << "Subgrafo " << i << endl;
//        for(Node* node : subgraph) {
//            cout << node->_id << " -> " << node->_weight << endl;
//            cont++;
//        }
//        cout << endl;
//        i++;
//        total_gap += total_gap_tree(subgraph);
//    }
//
//    cout << "Gap total: " << total_gap << endl;
//    cout << "Numero de nos: " << cont << endl;

    return 0;
}
