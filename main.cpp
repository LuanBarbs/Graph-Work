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
vector<bool> check_adjacent_trees(vector<Graph*>& forest, EdgeWithDifference& edge) {
    vector<bool> adjacency(forest.size(), false); // Inicializa o vetor de adjacência com falso.

    for(size_t i = 0; i < forest.size(); i++) {
        Node* current_node = forest[i]->get_first();
        while(current_node != nullptr) {
            Edge* current_edge = current_node->_first_edge;
            while(current_edge != nullptr) {
                // Se a aresta conecta o nó u ou v, marca como adjacente.
                if(current_edge->_target_id == edge.u || current_edge->_target_id == edge.v) {
                    adjacency[i] = true;
                    break;
                }
                current_edge = current_edge->_next_edge;
            }
            current_node = current_node->_next_node;
        }
    }

    return adjacency;
}

/// Função auxiliar para calcular o gap de uma árvore.
float total_gap_tree(Graph& tree, Node* u = nullptr, Node* v = nullptr) {
    float min_weight = numeric_limits<float>::max();
    float max_weight = numeric_limits<float>::lowest();

    // Calcula o maior e o menor peso entre os nós da árvore.
    Node* current_node = tree.get_first();
    while(current_node != nullptr) {
        if(current_node->_weight < min_weight) min_weight = current_node->_weight;
        if(current_node->_weight > max_weight) max_weight = current_node->_weight;
        current_node = current_node->_next_node;
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
float calculate_total_gap(vector<Graph*>& forest) {
    float total_gap = 0.0f;
    for(auto& tree : forest) {
        total_gap += total_gap_tree(*tree);
    }
    return total_gap;
}

/// Função principal do algoritmo guloso, constroi uma floresta utilizando Prim..
vector<Graph*> greedy_algorithm(Graph& graph, size_t p) {
    vector<Graph*> forest; // Floresta com p árvores.

    // Verifica se o grafo está vazio.
    if(graph.get_number_of_nodes() == 0) {
        cout << "Grafo vazio!" << endl;
        return forest;
    }
    // Verifica se o p é válido.
    if(p > graph.get_number_of_nodes() / 2) {
        cout << "O numero de subgrafos eh invalido!" << endl;
        return forest;
    }

    // Passo 1: Ordenar as arestas (u, v) por valores não decrescentes dos gaps |w_u - w_v|.
    vector<EdgeWithDifference> edges;
    Node* current_node = graph.get_first();
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
            // Verifica se a aresta já foi processada (evita duplicações).
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

    // Passo 2: Construir a floresta inicial com p árvores.
    vector<bool> is_verified(graph.get_number_of_nodes(), false);
    int index = 0;
    while(forest.size() < p && index < edges.size()) {
        EdgeWithDifference edge = edges[index];
        Node* node1 = graph.search_node_by_id(edge.u);
        Node* node2 = graph.search_node_by_id(edge.v);

        // Verifica se ambos os nós ainda não foram processados.
        if(!is_verified[node1->_id - 1] && !is_verified[node2->_id - 1]) {
            // Criar uma nova árvore com dois nós conectados pela aresta.
            Graph* tree = new Graph(false, true, true);
            tree->add_node(node1->_id, node1->_weight);
            tree->add_node(node2->_id, node2->_weight);
            tree->add_edge(node1->_id, node2->_id);
            forest.push_back(tree);

            // Marca os nós como verificados.
            is_verified[node1->_id - 1] = true;
            is_verified[node2->_id - 1] = true;
        }
        index++;
    }

    // Passo 3: Expandir as árvores existentes com novas arestas.
    while(index < edges.size()) {
        EdgeWithDifference edge = edges[index];
        Node* node1 = graph.search_node_by_id(edge.u);
        Node* node2 = graph.search_node_by_id(edge.v);

        // Verifica se apenas um dos nós já foi verificado, garantindo conectividade.
        if(is_verified[node1->_id - 1] ^ is_verified[node2->_id - 1]) {
            // Encontra o subgrafo que já contém o nó verificado.
            int tree_id = -1;
            Node* verified_node = is_verified[node1->_id - 1] ? node1 : node2;
            Node* unverified_node = is_verified[node1->_id - 1] ? node2 : node1;

            for(int i = 0; i < forest.size(); i++) {
                if(forest[i]->search_node_by_id(verified_node->_id)) {
                    tree_id = i;
                    break;
                }
            }

            // Verifica se a aresta já existe em `edges` antes de adicioná-la.
            bool edge_exists = false;
            for(EdgeWithDifference& e : edges) {
                if((e.u == verified_node->_id && e.v == unverified_node->_id) ||
                    (e.u == unverified_node->_id && e.v == verified_node->_id)) {
                    edge_exists = true;
                    break;
                }
            }
            // Se a aresta existir, adiciona o nó não verificado ao subgrafo e cria a aresta.
            if(tree_id != -1 && edge_exists) {
                forest[tree_id]->add_node(unverified_node->_id, unverified_node->_weight);
                forest[tree_id]->add_edge(verified_node->_id, unverified_node->_id);
                is_verified[unverified_node->_id - 1] = true;
            }
        }
        index++;
    }

    // Passo 4: Preencher as árvores com os vértices não cobertos.
    bool all_verified = false;
    while(!all_verified) {
        all_verified = true;

        for(int i = 0; i < graph.get_number_of_nodes(); i++) {
            if(!is_verified[i]) {
                Node* node = graph.search_node_by_id(i+1);

                // Encontra o subgrafo que minimiza o gap e ao qual o nó pode ser conectado.
                int tree_id = -1;
                float min_gap = numeric_limits<float>::max();
                int best_connection_node = -1;

                // Procura o subgrafo com o menor gap e encontra um nó existente para conectar.
                for(int j = 0; j < forest.size(); j++) {
                    Node* connection_node = nullptr;
                    Node* current_node = forest[j]->get_first();

                    while(current_node != nullptr) {
                        // Verifica se existe uma aresta entre "node' e "current_node".
                        bool edge_exists = false;
                        for(EdgeWithDifference& e : edges) {
                            if((e.u == node->_id && e.v == current_node->_id) ||
                                (e.u == current_node->_id && e.v == node->_id)) {
                                edge_exists = true;
                                break;
                            }
                        }

                        // Se a aresta existir, avalia se adicionar essa aresta minimiza o gap.
                        float current_gap = total_gap_tree(*forest[j], node, current_node);
                        if(edge_exists) {
                            float current_gap = total_gap_tree(*forest[j], node, current_node);
                            if(current_gap < min_gap) {
                                min_gap = current_gap;
                                tree_id = j;
                                best_connection_node = current_node->_id;
                            }
                        }

                        current_node = current_node->_next_node;
                    }
                }

                // Adiciona o nó e a aresta ao subgrafo, se a aresta existir.
                if(tree_id != -1 && best_connection_node != -1) {
                    forest[tree_id]->add_node(node->_id, node->_weight);
                    forest[tree_id]->add_edge(node->_id, best_connection_node);
                    is_verified[i] = true;
                } else {
                    // Ainda há vértices não verificados, continua rodando o passo 4.
                    all_verified = false;
                }
            }
        }
    }

    // Verifica se todos os vértices foram cobertos ao final.
    for(int i = 0; i < graph.get_number_of_nodes(); i++) {
        if(!is_verified[i]) {
            cout << "Vertice " << (i+1) << " nao foi adicionado a nenhum subgrafo!" << endl;
        }
    }

    return forest;
}

/// Função principal do algoritmo guloso randomizado adaptativo.
vector<Graph*> greedy_randomized_algorithm(Graph& graph, size_t p, float alfa) {
    vector<Graph*> forest; // Floresta com p árvores.

    // Verifica se o grafo está vazio.
    if(graph.get_number_of_nodes() == 0) {
        cout << "Grafo vazio!" << endl;
        return forest;
    }
    // Verifica se o p é válido.
    if(p > graph.get_number_of_nodes() / 2) {
        cout << "O numero de subgrafos eh invalido!" << endl;
        return forest;
    }

    // Passo 1: Ordenar as arestas (u, v) por valores não decrescentes dos gaps |w_u - w_v|.
    vector<EdgeWithDifference> edges;
    Node* current_node = graph.get_first();
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
            // Verifica se a aresta já foi processada.
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

    // Passo 2: Selecionar arestas randomicamente com base no alfa.
    vector<bool> is_verified(graph.get_number_of_nodes(), false);
    vector<bool> used_edges(edges.size(), false); // Vetor que marca arestas já usadas.
    int index = 0;

    while(forest.size() < p && index < edges.size()) {
        // Define o tamanho da lista de candidatos restritos com base no alfa.
        int rcl_size = max(1, (int)(alfa * edges.size()));
        vector<int> rcl; // Lista de candidatos restritos.

        // Adiciona à RCL apenas arestas que ainda não foram usadas.
        for(int i = 0; i < rcl_size; i++) {
            if(!used_edges[i]) {
                rcl.push_back(i);
            }
        }

        // Se não houver arestas disponíveis, interrompe.
        if(rcl.empty()) break;

        // Seleciona uma aresta aleatoriamente da RCL.
        int selected_index = rand() % rcl.size();
        EdgeWithDifference edge = edges[rcl[selected_index]];
        used_edges[rcl[selected_index]] = true; // Marca a aresta como usada.

        Node* node1 = graph.search_node_by_id(edge.u);
        Node* node2 = graph.search_node_by_id(edge.v);

        if(!is_verified[node1->_id - 1] && !is_verified[node2->_id - 1]) {
            // Criar uma nova árvore com dois nós conectados pela aresta.
            Graph* tree = new Graph(false, true, true);
            tree->add_node(node1->_id, node1->_weight);
            tree->add_node(node2->_id, node2->_weight);
            tree->add_edge(node1->_id, node2->_id);
            forest.push_back(tree);

            // Marca os nós como verificados.
            is_verified[node1->_id - 1] = true;
            is_verified[node2->_id - 1] = true;
        }
        index++;
    }

    // Passo 3: Expandir as árvores existentes com novas arestas.
    while(index < edges.size()) {
        EdgeWithDifference edge = edges[index];
        Node* node1 = graph.search_node_by_id(edge.u);
        Node* node2 = graph.search_node_by_id(edge.v);

        // Verifica se apenas um dos nós já foi verificado, garantindo conectividade.
        if(is_verified[node1->_id - 1] ^ is_verified[node2->_id - 1]) {
            // Encontra o subgrafo que já contém o nó verificado.
            int tree_id = -1;
            Node* verified_node = is_verified[node1->_id - 1] ? node1 : node2;
            Node* unverified_node = is_verified[node1->_id - 1] ? node2 : node1;

            // Verifica se a aresta entre os nós verificado e não verificado existe em "edges".
            bool edge_exists = false;
            for(const EdgeWithDifference& e : edges) {
                if((e.u == verified_node->_id && e.v == unverified_node->_id) ||
                   (e.u == unverified_node->_id && e.v == verified_node->_id)) {
                    edge_exists = true;
                    break;
                }
            }

            // Se a aresta existir, podemos adicionar o nó não verificado à árvore.
            if(edge_exists) {
                // Adiciona uma aleatoriedade na escolha da árvore que vai expandir.
                vector<int> candidate_trees;
                for(int i = 0; i < forest.size(); i++) {
                    if(forest[i]->search_node_by_id(verified_node->_id)) {
                        candidate_trees.push_back(i);
                    }
                }

                // Se houver árvore candidatas, escolhemos aleatoriamente uma para expandir.
                if (!candidate_trees.empty()) {
                    int selected_tree = candidate_trees[rand() % candidate_trees.size()];
                    forest[selected_tree]->add_node(unverified_node->_id, unverified_node->_weight);
                    forest[selected_tree]->add_edge(verified_node->_id, unverified_node->_id);
                    is_verified[unverified_node->_id - 1] = true;
                }
            }
        }
        index++;
    }

    // Passo 4: Preencher as árvores com os vértices não cobertos.
    bool all_verified = false;
    while(!all_verified) {
        all_verified = true;

        for(int i = 0; i < graph.get_number_of_nodes(); i++) {
            if(!is_verified[i]) {
                Node* node = graph.search_node_by_id(i+1);

                // Encontra o subgrafo que minimiza o gap e ao qual o nó pode ser conectado.
                int tree_id = -1;
                float min_gap = numeric_limits<float>::max();
                int best_connection_node = -1;

                // Procura o subgrafo com o menor gap e encontra um nó existente para conectar.
                for(int j = 0; j < forest.size(); j++) {
                    Node* connection_node = nullptr;
                    Node* current_node = forest[j]->get_first();

                    while(current_node != nullptr) {
                        // Verifica se existe uma aresta entre "node' e "current_node".
                        bool edge_exists = false;
                        for(const EdgeWithDifference& e : edges) {
                            if((e.u == node->_id && e.v == current_node->_id) ||
                               (e.u == current_node->_id && e.v == node->_id)) {
                                edge_exists = true;
                                break;
                            }
                        }

                        // Se a aresta existir, avalia se adicionar essa aresta minimiza o gap.
                        float current_gap = total_gap_tree(*forest[j], node, current_node);
                        if(edge_exists) {
                            if(current_gap < min_gap) {
                                min_gap = current_gap;
                                tree_id = j;
                                best_connection_node = current_node->_id;
                            }
                        }

                        current_node = current_node->_next_node;
                    }
                }

                // Adiciona o nó e a aresta ao subgrafo, se a aresta existir.
                if(tree_id != -1 && best_connection_node != -1) {
                    forest[tree_id]->add_node(node->_id, node->_weight);
                    forest[tree_id]->add_edge(node->_id, best_connection_node);
                    is_verified[i] = true;
                } else {
                    // Ainda há vértices não verificados, continua rodando o passo 4.
                    all_verified = false;
                }
            }
        }
    }

    return forest;
}

vector<Graph*> greedy_reactive_randomized_algorithm(Graph& graph, size_t p, vector<float>& alfas, int bloco) {
    vector<Graph*> best_forest;  // Floresta com a melhor solução encontrada.
    float best_gap = numeric_limits<float>::max();  // Melhor gap total encontrado.
    float best_alfa = 0.0f; // Inicializa o melhor alfa.

    // Inicializa as probabilidades e as médias dos alfas.
    vector<float> probabilidades(alfas.size(), 1.0 / alfas.size()); // Inicializa as probabilidades de cada alfa.
    vector<float> medias(alfas.size(), 0.0); // Média de qualidade para cada alfa.
    vector<int> contadores(alfas.size(), 0); // Número de vezes que cada alfa foi usado.

    // Iterações do algoritmo reativo.
    for(int iteracao = 0; iteracao < bloco; iteracao++) {
        // Seleciona um alfa de acordo com as probabilidades.
        float rand_val = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
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
        vector<Graph*> solucao_atual = greedy_randomized_algorithm(graph, p, alfa);

        // Avalia a qualidade da solução atual (calculando o gap total).
        float qualidade = calculate_total_gap(solucao_atual);

        // Se a solução atual for melhor que a melhor solução, atualiza a melhor solução.
        if(qualidade < best_gap) {
            best_forest = solucao_atual;
            best_gap = qualidade;
            best_alfa = alfa;
        } else {
            // Desaloca a memória da solução atual, pois ela não é melhor.
            for(Graph* g : solucao_atual) {
                delete g;
            }
        }


        // Atualiza as estatísticas da solução para o alfa escolhido.
        medias[alfa_index] = (medias[alfa_index] * contadores[alfa_index] + qualidade) / (contadores[alfa_index] + 1);
        contadores[alfa_index]++;

        // A cada bloco de iterações, atualiza as probabilidades com base nas médias de qualidade.
        if(iteracao % bloco == 0 && iteracao != 0) {
            float soma_inversa_media = 0.0;
            for(int i = 0; i < alfas.size(); i++) {
                soma_inversa_media += 1.0 / medias[i];
            }

            // Atualiza as probabilidades com base nas médias inversas.
            for(int i = 0; i < alfas.size(); i++) {
                probabilidades[i] = (1.0 / medias[i]) / soma_inversa_media;
            }
        }
    }

    cout << endl << "Melhor alfa: " << best_alfa << endl;

    return best_forest;  // Retorna a melhor solução encontrada.
}

int main(int argc, char* argv[])
{
    srand(time(NULL)); // Inicializa a semente aleatória com o tempo atual.

    if(argc != 3) {
        cerr << "Programa nao inicializado da forma correta!" << endl << "Execute <nome_instancia> <arquivo_saida>" << endl;
        cout << "O arquivo de entrada deve estar na pasta instances_example e deve ter o mesmo formato para leitura." << endl;
        return 1;
    }

    string inputFileName = argv[1];
    string outputFileName = argv[2];

    // Criando um arquivo de entrada.
    ifstream input_file("instances_example/" + inputFileName);
    if(!input_file) {
        cerr << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }

    // Criando o grafo.
    Graph grafo = Graph(input_file, false, true, true);

    // Numero de subgrafos.
    size_t p = 0;

    // Valor alfa.
    float alpha = 0.1;

    // Vetor alfa.
    vector<float> alfas = {0.1, 0.15, 0.20, 0.25, 0.3};

    // Numero de iteracoes.
    int numIter = 100;

    // Solução.
    vector<Graph*> forest;

    int choise = 0;

    while(choise != -1) {
        if(choise == 0) {
            cout << "Escolha alguma das funcionalidades a seguir (digite o numero):" << endl;
            cout << "[1] Imprimir grafo. [2] - Gerar solucao com algoritmo guloso. [3] Gerar solucao com algoritmo guloso randomizado." << endl;
            cout << "[4] Gerar solucao com algoritmo guloso randomizado reativo. [-1] - SAIR" << endl;
        }
        cin >> choise;
        switch(choise) {
            case 1: {
                ofstream output_file(outputFileName);
                if(!output_file) {
                    cerr << "Erro ao abrir o arquivo de saida." << endl;
                    return 1;
                }
                else grafo.print_graph(output_file);
                choise = 0;
                break;
            }
            case 2: {
                cout << "Escolha o numero de subgrafos:" << endl;
                cin >> p;
                forest = greedy_algorithm(grafo, p);
                cout << "Escolha uma das opcoes a seguir (digite o numero - se nao funcionar coloque o valor novamente):" << endl;
                cout << "[5] Imprimir subgrafos. [6] Imprimir gap total. [-1] -SAIR" << endl;
                cin >> choise;
                break;
            }
            case 3: {
                cout << "Escolha o numero de subgrafos:" << endl;
                cin >> p;
                cout << "Escolha o valor de alfa (de 0 a 1):" << endl;
                cin >> alpha;
                forest = greedy_randomized_algorithm(grafo, p, alpha);
                cout << "Escolha uma das opcoes a seguir (digite o numero - se nao funcionar coloque o valor novamente):" << endl;
                cout << "[5] Imprimir subgrafos. [6] Imprimir gap total. [-1] -SAIR" << endl;
                cin >> choise;
                break;
            }
            case 4: {
                cout << "Escolha o numero de subgrafos:" << endl;
                cin >> p;
                cout << "Escolha os valores de alfa (5 valores - predefinido):" << endl;
                for(int i = 0; i < 5; i++) {
                    cout << "Escolha o valor:" << endl;
                    cin >> alfas[i];
                }
                cout << "Escolha quantas iteracoes:" << endl;
                cin >> numIter;
                forest = greedy_reactive_randomized_algorithm(grafo, p, alfas, numIter);
                cout << "Escolha uma das opcoes a seguir (digite o numero - se nao funcionar coloque o valor novamente):" << endl;
                cout << "[5] Imprimir subgrafos. [6] Imprimir gap total. [-1] SAIR" << endl;
                cin >> choise;
                break;
            }
            case 5: {
                cout << "Escolha qual subgrafo (valores possiveis: 0 a " << p-1 << "):" << endl;
                int i = 0;
                cin >> i;
                if(i < 0 || i >= p) {
                    cout << "Valor de subgrafo impossivel." << endl;
                    choise = 5;
                }
                ofstream output_file(outputFileName);
                if(!output_file) {
                    cerr << "Erro ao abrir o arquivo de saida." << endl;
                    return 1;
                }
                else forest[i]->print_graph(output_file);
                cout << "Escolha uma das opcoes a seguir (digite o numero - se nao funcionar coloque o valor novamente):" << endl;
                cout << "[5] Imprimir novo subgrafo. [0] Voltar. [-1] SAIR." << endl;
                cin >> choise;
                break;
            }
            case 6: {
                float total_gap = 0;
                for(Graph* tree : forest) {
                    total_gap += total_gap_tree(*tree);
                    cout << endl;
                }
                cout << "Gap total: " << total_gap << endl;
                cout << "[5] Imprimir subgrafos. [-1] SAIR." << endl;
                cin >> choise;
                break;
            }
            default:
                choise = -1;
                break;
        }

    }

    // Desalocar a memória dos subgrafos
    for(Graph* tree : forest) {
        delete tree; // Libera a memória de cada subgrafo.
    }

    return 0;
}
