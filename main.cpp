#include "Graph.hpp"
#include "defines.hpp"

using namespace std;

/// Estrutura auxiliar para ordenar arestas.
struct EdgeWithDifference {
    size_t u;
    size_t v;
    float weight_diff;
};

/// Fun��o auxiliar para verificar adjac�ncia.
vector<bool> check_adjacent_trees(vector<vector<Node*>>& forest, EdgeWithDifference& edge) {
    vector<bool> adjacency(forest.size(), false); // Inicializa o vetor de adjac�ncia com falso.

    for(size_t i = 0; i < forest.size(); i++) {
        for(Node* node : forest[i]) {
            Edge* current_edge = node->_first_edge;
            while(current_edge != nullptr) {
                // Se a aresta conecta o n� com u ou v, marca como adjacente.
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

/// Fun��o auxiliar para calcular o gap de uma �rvore.
float total_gap_tree(vector<Node*>& tree, Node* u = nullptr, Node* v = nullptr) {
    float min_weight = numeric_limits<float>::max();
    float max_weight = numeric_limits<float>::lowest();

    // Calcula o maior e o menor peso entre os n�s da �rvore.
    for (Node* node : tree) {
        if (node->_weight < min_weight) min_weight = node->_weight;
        if (node->_weight > max_weight) max_weight = node->_weight;
    }

    // Verifica se os n�s u e v precisam ser considerados no c�lculo do gap.
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

/// Fun��o auxiliar para calcular o gap de uma �rvore ao se remover um n�.
float remove_gap_tree(vector<Node*>& tree, Node* u) {
        float min_weight = numeric_limits<float>::max();
    float max_weight = numeric_limits<float>::lowest();

    // Calcula o maior e o menor peso entre os n�s da �rvore, exclu�do o n� removido.
    for (Node* node : tree) {
        if(node->_id != u->_id) {
            if (node->_weight < min_weight) min_weight = node->_weight;
            if (node->_weight > max_weight) max_weight = node->_weight;
        }
    }

    return max_weight - min_weight;
}

/// Fun��o auxiliar para calcular o gap total das �rvores.
float calculate_total_gap(vector<vector<Node*>>& subgraphs) {
    float total_gap = 0.0f;
    for(auto& subgraph : subgraphs) {
        total_gap += total_gap_tree(subgraph);
    }
    return total_gap;
}

/// Fun��o principal do algoritmo guloso, constroi uma floresta utilizando Prim..
vector<vector<Node*>> greedy_algorithm(Graph& graph, size_t p) {
    vector<vector<Node*>> forest; // Floresta com p �rvores.

    // Verifica se o grafo est� vazio.
    if(graph.get_number_of_nodes() == 0) {
        cout << "Grafo vazio!" << endl;
        return forest;
    }
    // Verifica se o p � v�lido.
    if(p > graph.get_number_of_nodes() / 2) {
        cout << "O numero de subgrafos eh invalido!" << endl;
        return forest;
    }

    // Passo 1: Ordenar as arestas (u, v) por valores n�o decrescentes dos gaps |w_u - w_v|.
    vector<EdgeWithDifference> edges;
    Node* current_node = graph.get_first();
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
            // Verifica se a aresta j� foi processada (evita duplica��es).
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
    // Ordena as arestas pela diferen�a de peso entre os n�s (n�o decrescente).
    sort(edges.begin(), edges.end(), [](const EdgeWithDifference& e1, const EdgeWithDifference& e2) {
        return e1.weight_diff < e2.weight_diff;
    });

    // Passo 2: Construir a floresta inicial com p �rvores.
    vector<bool> is_verified(graph.get_number_of_nodes(), false);
    int index = 0;
    while(forest.size() < p && index < edges.size()) {
        EdgeWithDifference edge = edges[index];
        Node* node1 = graph.search_node_by_id(edge.u);
        Node* node2 = graph.search_node_by_id(edge.v);

        // Verifica se ambos os n�s ainda n�o foram processados.
        if(!is_verified[node1->_id - 1] && !is_verified[node2->_id - 1]) {
            vector<bool> adjacency = check_adjacent_trees(forest, edge);

            // Se n�o houver �rvores na floresta, cria a primeira �rvore.
            if(forest.size() == 0) {
                vector<Node*> tree;
                tree.push_back(node1);
                tree.push_back(node2);
                forest.push_back(tree);
            } else {
                int tree_id = -1;
                float min_gap = numeric_limits<float>::max();

                // Para cada �rvore na floresta, calcula-se o gap ao adicionar os n�s u e v.
                for(int i = 0; i < forest.size(); i++) {
                    if(!adjacency[i]) {
                        float current_gap = total_gap_tree(forest[i], node1, node2);
                        if(current_gap < min_gap) {
                            min_gap = current_gap;
                            tree_id = i;
                        }
                    }
                }

                // Se n�o for poss�vel adicionar os n�s em nenhuma �rvore, cria-se uma nova �rvore.
                if(tree_id == -1) {
                    vector<Node*> tree;
                    tree.push_back(node1);
                    tree.push_back(node2);
                    forest.push_back(tree);
                } else {
                    forest[tree_id].push_back(node1);
                    forest[tree_id].push_back(node2); // Adiciona os n�s � �rvore com o menor gap.
                }
            }
            // Marca os n�s como verificados.
            is_verified[node1->_id - 1] = true;
            is_verified[node2->_id - 1] = true;
        }
        index++;
    }

    // Passo 3: Expandir as �rvores com os v�rtices n�o cobertos.
    for(int i = 0; i < graph.get_number_of_nodes(); i++) {
        // Verifica se o n� i ainda n�o foi inserido em nenhuma �rvore.
        if(!is_verified[i]) {
            Node* node = graph.search_node_by_id(i+1);
            if(forest.size() < p) {
                // Cria uma nova �rvore e garante que tenha pelo menos dois v�rtices.
                vector<Node*> new_tree;
                new_tree.push_back(node);

                // Encontra um segundo v�rtice n�o verificado para adicionar � nova �rvore.
                for(int j = i + 1; j < graph.get_number_of_nodes(); j++) {
                    if(!is_verified[j]) {
                        Node* second_node = graph.search_node_by_id(j+1);
                        new_tree.push_back(second_node);
                        is_verified[j] = true;
                        break;
                    }
                }

                forest.push_back(new_tree);
            } else {
                // Adiciona o n� � �rvore com o menor gap.
                int tree_id = -1;
                float min_gap = numeric_limits<float>::max();

                for(int j = 0; j < forest.size(); j++) {
                    float current_gap = total_gap_tree(forest[j], node);
                    if(current_gap < min_gap) {
                        min_gap = current_gap;
                        tree_id = j;
                    }
                }

                forest[tree_id].push_back(node);
            }
            is_verified[i] = true;
        }
    }

    return forest;
}

/// Fun��o principal do algoritmo guloso randomizado adaptativo.
vector<vector<Node*>> greedy_randomized_algorithm(Graph& graph, size_t p, float alfa) {
    vector<vector<Node*>> forest;

    // Verifica se o grafo est� vazio.
    if(graph.get_number_of_nodes() == 0) {
        cout << "Grafo vazio!" << endl;
        return forest;
    }
    // Verifica se o p � v�lido.
    if(p > graph.get_number_of_nodes() / 2) {
        cout << "O numero de subgrafos eh invalido!" << endl;
        return forest;
    }

    // Passo 1: Ordenar as arestas (u, v) por valores n�o decrescentes dos gaps |w_u - w_v|.
    vector<EdgeWithDifference> edges;
    Node* current_node = graph.get_first();
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
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
    // Ordena as arestas pela diferen�a de peso entre os n�s (n�o decrescente).
    sort(edges.begin(), edges.end(), [](const EdgeWithDifference& e1, const EdgeWithDifference& e2) {
        return e1.weight_diff < e2.weight_diff;
    });

    // Passo 2: Selecionar arestas randomicamente com base no alfa.
    vector<bool> is_verified(graph.get_number_of_nodes(), false);
    vector<bool> used_edges(edges.size(), false); // Vetor que marca arestas j� usadas.
    while(forest.size() < p) {
        int rcl_size = max(1, (int)(alfa * edges.size())); // Define o tamanho da lista de candidatos restritos.
        vector<int> rcl;

        for(int i = 0; i < rcl_size; i++) {
            // Adiciona � RCL apenas arestas que ainda n�o foram usadas.
            if(!used_edges[i]) {
                rcl.push_back(i);
            }
        }

        // Se n�o houver arestas dispon�veis, interrompe.
        if(rcl.empty()) break;

        int selected_index = rand() % rcl.size(); // Seleciona uma aresta aleatoriamente da RCL.
        EdgeWithDifference edge = edges[rcl[selected_index]];
        used_edges[rcl[selected_index]] = true; // Marca a aresta como usada.

        Node* node1 = graph.search_node_by_id(edge.u);
        Node* node2 = graph.search_node_by_id(edge.v);

        if(!is_verified[node1->_id - 1] && !is_verified[node2->_id - 1]) {
            vector<bool> adjacency = check_adjacent_trees(forest, edge);

            if(forest.size() == 0) {
                vector<Node*> tree;
                tree.push_back(node1);
                tree.push_back(node2);
                forest.push_back(tree);
            } else {
                int tree_id = -1;
                float min_gap = numeric_limits<float>::max();

                for(int i = 0; i < forest.size(); i++) {
                    if(!adjacency[i]) {
                        float current_gap = total_gap_tree(forest[i], node1, node2);
                        if(current_gap < min_gap) {
                            min_gap = current_gap;
                            tree_id = i;
                        }
                    }
                }

                if(tree_id == -1) {
                    vector<Node*> tree;
                    tree.push_back(node1);
                    tree.push_back(node2);
                    forest.push_back(tree);
                } else {
                    forest[tree_id].push_back(node1);
                    forest[tree_id].push_back(node2);
                }
            }
            is_verified[node1->_id - 1] = true;
            is_verified[node2->_id - 1] = true;
        }
    }

    // Passo 3: Expandir as �rvores com os v�rtices n�o cobertos.
    for(int i = 0; i < graph.get_number_of_nodes(); i++) {
        if(!is_verified[i]) {
            Node* node = graph.search_node_by_id(i+1);
            if(forest.size() < p) {
                // Cria uma nova �rvore e garante que tenha pelo menos dois v�rtices.
                vector<Node*> new_tree;
                new_tree.push_back(node);

                // Encontra um segundo v�rtice n�o verificado para adicionar � nova �rvore.
                for(int j = i + 1; j < graph.get_number_of_nodes(); j++) {
                    if(!is_verified[j]) {
                        Node* second_node = graph.search_node_by_id(j+1);
                        new_tree.push_back(second_node);
                        is_verified[j] = true;
                        break;
                    }
                }

                forest.push_back(new_tree);
            } else {
                // Adiciona o n� � �rvore com o menor gap.
                int tree_id = -1;
                float min_gap = numeric_limits<float>::max();

                for(int j = 0; j < forest.size(); j++) {
                    float current_gap = total_gap_tree(forest[j], node);
                    if(current_gap < min_gap) {
                        min_gap = current_gap;
                        tree_id = j;
                    }
                }

                forest[tree_id].push_back(node);
            }
            is_verified[i] = true;
        }
    }

    return forest;
}

vector<vector<Node*>> greedy_reactive_randomized_algorithm(Graph& graph, size_t p, vector<float>& alfas, int bloco) {
    vector<vector<Node*>> best_forest;  // Floresta com a melhor solu��o encontrada.
    float best_gap = numeric_limits<float>::max();  // Melhor gap total encontrado.
    float best_alfa = 0.0f; // Inicializa o melhor alfa.

    vector<float> probabilidades(alfas.size(), 1.0 / alfas.size()); // Inicializa as probabilidades de cada alfa.
    vector<float> medias(alfas.size(), 0.0); // M�dia de qualidade para cada alfa.
    vector<int> contadores(alfas.size(), 0); // N�mero de vezes que cada alfa foi usado.

    for(int iteracao = 0; iteracao < bloco; iteracao++) {
        // Seleciona um alfa de acordo com as probabilidades.
        float rand_val = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        int alfa_index = 0;
        float cumulative_probability = 0.0;
        for(int i = 0; i < alfas.size(); i++) {
            cumulative_probability += probabilidades[i];
            if (rand_val <= cumulative_probability) {
                alfa_index = i;
                break;
            }
        }
        float alfa = alfas[alfa_index];

        // Aplica o algoritmo guloso randomizado com o alfa selecionado.
        vector<vector<Node*>> solucao_atual = greedy_randomized_algorithm(graph, p, alfa);

        // Avalia a qualidade da solu��o atual.
        float qualidade = calculate_total_gap(solucao_atual); // Fun��o que calcula o gap total da solu��o.

        // Se a solu��o atual for melhor que a melhor solu��o, atualiza a melhor solu��o.
        if(qualidade < best_gap) {
            best_forest = solucao_atual;
            best_gap = qualidade;
            best_alfa = alfa;
        }

        // Atualiza as estat�sticas da solu��o para o alfa escolhido.
        medias[alfa_index] = (medias[alfa_index] * contadores[alfa_index] + qualidade) / (contadores[alfa_index] + 1);
        contadores[alfa_index]++;

        // A cada bloco de itera��es, atualiza as probabilidades
        if(iteracao % bloco == 0 && iteracao != 0) {
            float soma_inversa_media = 0.0;
            for(int i = 0; i < alfas.size(); i++) {
                soma_inversa_media += 1.0 / medias[i];
            }

            for(int i = 0; i < alfas.size(); i++) {
                probabilidades[i] = (1.0 / medias[i]) / soma_inversa_media; // Ajusta a probabilidade com base nas m�dias
            }
        }
    }

    cout << "Melhor alfa: " << best_alfa << endl;

    return best_forest;  // Retorna a melhor solu��o encontrada.
}

int main(int argc, char* argv[])
{
    // Criando o grafo.
    ifstream input_file;
    input_file.open("instances_example/n100plap2i1.txt");
    Graph graph = Graph(input_file, false, true, true);

    // N�mero de subgrafos
    size_t p = 10;

    vector<vector<Node*>> forest = greedy_algorithm(graph, p);

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

    forest = greedy_randomized_algorithm(graph, p, 0.5);

    total_gap = 0;
    cont = 0;
    i = 1;
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

    vector<float> alfas = {0.1, 0.2, 0.3, 0.4};
    forest = greedy_reactive_randomized_algorithm(graph, p, alfas, 100);

    total_gap = 0;
    cont = 0;
    i = 1;
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

    return 0;
}
