#include "Graph.hpp"
#include <locale.h>

using namespace std;

// Construtor a partir de uma instância lida de um arquivo.
Graph::Graph(ifstream& instance, const string& filename) {
    if (!instance.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return;
    }

    // Determina se o grafo é direcionado ou não baseado no nome do arquivo.
    if (filename.find('D') != string::npos) {
        _directed = true;
    }
    else {
        _directed = false;
    }

    // Inicializa as propriedades do grafo.
    size_t numberOfNodes;
    instance >> numberOfNodes;
    _number_of_nodes = 0;
    _number_of_edges = 0;
    _weighted_edges = true;
    _weighted_nodes = true;
    _first = nullptr;
    _last = nullptr;

    size_t node_id, target_id;
    float edge_weight;

    // Adiciona todos os nós ao grafo.
    for (size_t i = 1; i <= numberOfNodes; ++i) {
        add_node(i);
    }

    // Adiciona as arestas ao grafo.
    while(instance >> node_id >> target_id >> edge_weight) {
        add_edge(node_id, target_id, edge_weight);
    }
}

// Construtor padrão.
Graph::Graph(bool isDirected, bool isWeightedEdges, bool IsWeightedNodes) {
    // Inicialização do grafo vazio (sem nós ou arestas).
    _number_of_nodes = 0;
    _number_of_edges = 0;
    _directed = isDirected;
    _weighted_edges = isWeightedEdges;
    _weighted_nodes = IsWeightedNodes;
    _first = nullptr;
    _last = nullptr;
}

// Destrutor padrão.
Graph::~Graph() {
    // Liberando a memória alocada para o grafo.
    Node* current_node = _first;
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
            Edge* next_edge = current_edge->_next_edge;
            delete current_edge;
            current_edge = next_edge;
        }
        Node* next_node = current_node->_next_node;
        delete current_node;
        current_node = next_node;
    }

    // Definindo os ponteiros como null.
    _first = nullptr;
    _last = nullptr;
    _number_of_nodes = 0;
    _number_of_edges = 0;
    _directed = false;
    _weighted_nodes = false;
    _weighted_edges = false;
}

// Método que remove um nó do grafo.
void Graph::remove_node(size_t node_id) {
    // Procura o nó a ser removido.
    Node* remove_node = search_node_by_id(node_id);

    // Nó a ser removido não existe no grafo.
    if(remove_node == nullptr) {
        cout << "Nó a ser removido não encontrado!" << endl;
        return;
    }

    // Removendo as arestas da lista de adjacências do nó.
    Edge* remove_node_edge = remove_node->_first_edge;
    while(remove_node_edge != nullptr) {
        Edge* next_edge = remove_node_edge->_next_edge;
        delete remove_node_edge;
        _number_of_edges--;
        remove_node_edge = next_edge;
    }

    // Ajustando os ponteiros.
    if(_number_of_nodes == 1) { // Nó a ser removido é o único.
        _first = nullptr;
        _last = nullptr;
    }
    else if(remove_node == _first) { // Nó a ser removido é o primerio mas não o único.
        remove_node->_next_node->_previous_node = nullptr;
        _first = remove_node->_next_node;
    }
    else if(remove_node == _last) { // Nó a ser removido é o último.
        remove_node->_previous_node->_next_node = nullptr;
        _last = remove_node->_previous_node;
    }
    else { //Nó a ser removido não é o primeiro nem o último.
        remove_node->_previous_node->_next_node = remove_node->_next_node;
        remove_node->_next_node->_previous_node = remove_node->_previous_node;
    }

    // Removendo o nó.
    delete remove_node;

    // Decrementando a quantidade de nós do grafo.
    _number_of_nodes--;

    // Removendo as arestas que tem o nó como alvo.
    Node* current_node = _first;
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        Edge* previous_edge = nullptr;
        while(current_edge != nullptr) {
            if(current_edge->_target_id == node_id) {
                if(previous_edge == nullptr) { // A aresta a ser removida é a primeira da lista de adjacências.
                    current_node->_first_edge = current_edge->_next_edge;
                }
                else { // A aresta a ser removido não é a primeira (não importa se é a última).
                    previous_edge->_next_edge = current_edge->_next_edge;
                }
                delete current_edge;
                current_node->_number_of_edges--;
                break;
            }
            previous_edge = current_edge;
            current_edge = current_edge->_next_edge;
        }
        current_node = current_node->_next_node;
    }
}

// Método que remove uma aresta entre dois nós.
void Graph::remove_edge(size_t node_id_1, size_t node_id_2) {
    // Procurando os nós pelo id.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    // Verificando se os nós existem.
    if(node_1 == nullptr || node_2 == nullptr) {
        cout << "O(s) nó(s) não existe(m)!" << endl;
        return;
    }

    // Procurando a aresta a ser removida no nó 1.
    Edge* current_edge = node_1->_first_edge;
    Edge* previous_edge = nullptr;
    while(current_edge != nullptr) {
        if(current_edge->_target_id == node_id_2) break;
        previous_edge = current_edge;
        current_edge = current_edge->_next_edge;
    }

    // Verificando se a aresta existe.
    if(current_edge == nullptr) {
        cout << "A aresta não existe para o nó " << node_id_1 << "!" << endl;
        return;
    }

    // Ajustando os ponteiros.
    if(current_edge->_next_edge == nullptr && previous_edge == nullptr) { // A aresta a ser removida é a única.
        node_1->_first_edge = nullptr;
    }
    else if(previous_edge == nullptr) { // A aresta a ser removida é a primeira.
        node_1->_first_edge = current_edge->_next_edge;
    }
    else { // A aresta a ser removida não é a primeira.
        previous_edge->_next_edge = current_edge->_next_edge;
    }

    // Deletando a aresta.
    delete current_edge;

    // Decrementando o número de arestas do primeiro nó.
    node_1->_number_of_edges--;

    if(!_directed) {
        // Procurando a aresta a ser removida no nó 2 se o grafo não é direcionado.
        current_edge = node_2->_first_edge;
        previous_edge = nullptr;
        while(current_edge != nullptr) {
            if(current_edge->_target_id == node_id_1) break;
            previous_edge = current_edge;
            current_edge = current_edge->_next_edge;
        }

        // Verificando se a aresta existe.
        if(current_edge == nullptr) {
            cout << "A aresta não existe para o nó " << node_id_2 << "!" << endl;
            return;
        }

        // Ajustando os ponteiros.
        if(current_edge->_next_edge == nullptr && previous_edge == nullptr) { // A aresta a ser removida é a única.
            node_2->_first_edge = nullptr;
        }
        else if(previous_edge == nullptr) { // A aresta a ser removida é a primeira.
            node_2->_first_edge = current_edge->_next_edge;
        }
        else { // A aresta a ser removida não é a primeira.
            previous_edge->_next_edge = current_edge->_next_edge;
        }

        // Deletando a aresta.
        delete current_edge;

        // Decrementando o número de arestas do segundo nó.
        node_2->_number_of_edges--;
    }

    // Decrementando o número de arestas do grafo.
    _number_of_edges--;
}

// Método que adiciona um novo nó ao grafo.
void Graph::add_node(size_t node_id, float weight) {
    // Verica se nó já existe.
    Node* node_verify = search_node_by_id(node_id);
    if(node_verify != nullptr) {
        cout << "Nó já existe no grafo!" << endl;
        return;
    }

    // Criando o novo nó.
    Node* new_node = new Node;
    new_node->_number_of_edges = 0;
    new_node->_id = node_id;
    new_node->_weight = weight;
    new_node->_first_edge = nullptr;
    new_node->_next_node = nullptr;
    new_node->_previous_node = nullptr;

    // Adicionando nó ao grafo.
    if(_number_of_nodes == 0) { // Grafo vazio.
        _first = new_node;
        _last = new_node;
    }
    else { // Grafo não vazio.
        _last->_next_node = new_node;
        new_node->_previous_node = _last;
        _last = new_node;
    }

    // Incrementa o número de nós do grafo.
    _number_of_nodes++;
}

// Metódo que adiciona uma nova aresta entre dois nós.
void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight) {
    // Procurando os nós alvos.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    if(node_1 != nullptr && node_2 != nullptr) {
        // Criando a nova aresta para o nó 1.
        Edge* new_edge = new Edge;
        new_edge->_next_edge = nullptr;
        new_edge->_weight = weight;
        new_edge->_target_id = node_id_2;

        // Adicionado a aresta para o nó 1.
        if(node_1->_first_edge == nullptr) { // Não existe lista de adjacências para o nó 1.
            node_1->_first_edge = new_edge;
        }
        else { // Existe lista de adjacências para o nó 1.
            Edge* current_edge = node_1->_first_edge;
            while(current_edge->_next_edge != nullptr) {
                current_edge = current_edge->_next_edge;
            }
            current_edge->_next_edge = new_edge;
        }
        // Incrementa o número de arestas do nó 1.
        node_1->_number_of_edges++;

        if(!_directed) {
            // Criando a nova aresta para o nó 2 se o grafo não for direcionado.
            Edge* new_edge2 = new Edge;
            new_edge2->_next_edge = nullptr;
            new_edge2->_weight = weight;
            new_edge2->_target_id = node_id_1;

            // Adicionando a aresta para o nó 2.
            if(node_2->_first_edge == nullptr) { // Não existe lista de adjacências para o nó 2.
                node_2->_first_edge = new_edge2;
            }
            else { // Existe lista de adjacências para o nó 2.
                Edge* current_edge = node_2->_first_edge;
                while(current_edge->_next_edge != nullptr) {
                    current_edge = current_edge->_next_edge;
                }
                current_edge->_next_edge = new_edge2;
            }
            // Incrementa o número de arestas do nó 2.
            node_2->_number_of_edges++;
        }

        // Incrementa o número de arestas do grafo.
        _number_of_edges++;
    }
    else {
        cout << "Nó(s) não existe(m) no grafo!" << endl;
    }
}

// Método para printar uma representação do grafo em um arquivo de saída.
void Graph::print_graph(ofstream& output_file) {
    if(_directed) {
        output_file << "diGraph G {" << endl;
        Node* node = _first;
        output_file << " start -> " << node->_id << ";" << endl;
        for(size_t i = 0; i < _number_of_nodes; i++) {
            Edge* edge = node->_first_edge;
            while(edge != nullptr) {
                output_file << " " << node->_id << " -> " << edge->_target_id << ";" << endl;
                edge = edge->_next_edge;
            }
            if(node->_next_node == nullptr) break;
            node = node->_next_node;
        }
        output_file << " " << node->_id << " -> end;" << endl;
        output_file << "}";
    }
    else {
        output_file << "graph G {" << endl;
        Node* node = _first;
        output_file << " start -- " << node->_id << ";" << endl;
        for(size_t i = 0; i < _number_of_nodes; i++) {
            Edge* edge = node->_first_edge;
            while(edge != nullptr) {
                output_file << " " << node->_id << " -- " << edge->_target_id << ";" << endl;
                edge = edge->_next_edge;
            }
            if(node->_next_node == nullptr) break;
            node = node->_next_node;
        }
        output_file << " " << node->_id << " -- end;" << endl;
        output_file << "}";
    }
    cout << endl << "Arquivo gerado!" << endl;
}

// Método para printar uma representação do grafo no console.
void Graph::print_graph() {
    Node* current_node = _first;
    if(current_node == nullptr) {
        cout << "Grafo vazio!" << endl;
        return;
    }
    cout << endl;
    while(current_node != nullptr) {
        cout << current_node->_id;
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
            cout << " -> " << current_edge->_target_id;
            current_edge = current_edge->_next_edge;
        }
        if(current_node->_next_node != nullptr) cout << endl << "|" << endl;
        else cout << endl;
        current_node = current_node->_next_node;
    }
}

// Método que procura um nó pelo id e o retorna.
Node* Graph::search_node_by_id(size_t node_id) {
    Node* current_node = _first;
    while(current_node != nullptr) {
        if(current_node->_id == node_id) return current_node;
        current_node = current_node->_next_node;
    }
    return nullptr;
}

// Imprimindo informações gerais sobre o grafo.
void Graph::print_graph_infos() {
    cout << endl << "Informações sobre o grafo: " << endl;
    cout << "Quantidade de nós: " << _number_of_nodes << endl;
    cout << "Quantidade de arestas: " << _number_of_edges << endl;
    cout << "Direcionado: " << _directed << endl;
}

// Imprimindo informações gerais sobre um nó específico.
void Graph::print_node_infos(size_t node_id) {
    Node* current_node = search_node_by_id(node_id);

    // Nó não existe no grafo.
    if(current_node == nullptr) {
        cout << endl << "Não existem informações sobre esse nó!" << endl;
        return;
    }

    cout << endl << "Informações sobre o nó " << current_node->_id << ": " << endl;

    // Lista de adjacências.
    if(current_node->_first_edge != nullptr) {
        cout << "Nó alvo da primeira aresta: " << current_node->_first_edge->_target_id << endl;
    }
    else {
        cout << "Nó sem lista de adjacências." << endl;
    }

    // Nó anterior na lista.
    if(current_node->_previous_node != nullptr) {
        cout << "Nó anterior na lista: " << current_node->_previous_node->_id << endl;
    }
    else {
        cout << "Esse é o primeiro nó da lista." << endl;
    }

    // Próximo nó na lista.
    if(current_node->_next_node != nullptr) {
        cout << "Próximo nó na lista: " << current_node->_next_node->_id << endl;
    }
    else {
        cout << "Esse é o último nó da lista." << endl;
    }

    // Quantidade de arestas.
    cout << "Quantidade de arestas: " << current_node->_number_of_edges << endl;
}

// Fecho Transitivo Direto.
vector<size_t> Graph::direct_transitive_closure(size_t node_id) {
    if(!_directed) {
        cout << endl << "O grafo não é direcionado!" << endl;
        return {};
    }
    Node* node = search_node_by_id(node_id);
    if(node == nullptr) {
        cout << endl << "O nó não existe no grafo!" << endl;
        return {};
    }
    vector<size_t> closure;
    Edge* current_edge = node->_first_edge;
    while(current_edge != nullptr) {
        closure.push_back(current_edge->_target_id);
        current_edge = current_edge->_next_edge;
    }
    return closure;
}

// Fecho Transitivo Indireto.
vector<size_t> Graph::indirect_transitive_closure(size_t node_id) {
    if(!_directed) {
        cout << endl << "O grafo não é direcionado!" << endl;
        return {};
    }
    Node* node = search_node_by_id(node_id);
    if(node == nullptr) {
        cout << endl << "O nó não existe no grafo!" << endl;
        return {};
    }
    vector<size_t> closure;
    Node* current_node = _first;
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        while(current_edge != nullptr) {
           if(current_edge->_target_id == node_id) {
                closure.push_back(current_node->_id);
                break;
           }
           current_edge = current_edge->_next_edge;
        }
        current_node = current_node->_next_node;
    }

    return closure;
}

// Algoritmo de Djkstra para encontrar o caminho mínimo entre dois vérticies.
vector<size_t> Graph::dijkstra(size_t node_id_1, size_t node_id_2) {
    // Verifica se o grafo é vazio.
    if(_number_of_nodes == 0) {
        cout << endl << "Grafo vazio!" << endl;
        return {};
    }

    // Verifica se o grafo é direcionado.
    if(!_directed) {
        cout << endl << "Grafo não direcionado!" << endl;
        return {};
    }

    // Busca os dois nós.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    if(node_1 == nullptr || node_2 == nullptr) {
        cout << endl << "Não é possível achar o caminho mínimo (algum(s) dos dois nós não existe(m))!" << endl;
        return {};
    }

    // Inicialização.
    const float INF = numeric_limits<float>::infinity(); // Define uma constante para representar o infinito.
    vector<float> dist(_number_of_nodes, INF);           // Vetor de distâncias pi. Todos os valores inicializados com infinito.
    vector<size_t> S;                                    // Vetor para armazenar o conjunto de nós cujas distâncias mínimas já foram determinadas.
    vector<size_t> S_bar;                                // Vetor que armazena o conjunto de nós cujas distâncias mínimas ainda não foram determinadas.
    vector<size_t> previous(_number_of_nodes, -1);       // Vetor para armazenar os predecessores.
    vector<size_t> path;                                 // Vetor para montar o caminho.

    // Adiciona o nó origem em S.
    S.push_back(node_id_1);

    // A distância do primeiro nó para ele mesmo é zero.
    dist[node_id_1-1] = 0;

    // Adiciona os outros nós em S_bar.
    Node* node = _first;
    while(node != nullptr) {
        if(node->_id != node_id_1) {
            S_bar.push_back(node->_id);
        }
        node = node->_next_node;
    }

    // Adiciona as distâncias para os nós com conexões diretas para o nó 1.
    Edge* edge = node_1->_first_edge;
    while(edge != nullptr) {
        dist[edge->_target_id-1] = edge->_weight;
        previous[edge->_target_id-1] = node_id_1;
        edge = edge->_next_edge;
    }

    while(!S_bar.empty()) {
        // Seleciona j de S_bar com a menor distância para o nó 1.
        size_t j = S_bar[0];
        for(size_t i : S_bar) {
            if(dist[i-1] < dist[j]) {
                j = i;
            }
        }
        // Remove j de S_bar e adiciona em S.
        auto it = remove(S_bar.begin(), S_bar.end(), j);
        S_bar.erase(it, S_bar.end());
        S.push_back(j);

        // Atualiza as distâncias para os vizinhos de j.
        Node* node_j = search_node_by_id(j);
        Edge* current_edge = node_j->_first_edge;
        while(current_edge != nullptr) {
            size_t k = current_edge->_target_id;
            float weight = current_edge->_weight;

            if(dist[j-1] + weight < dist[k - 1]) {
                dist[k - 1] = dist[j - 1] + weight;
                previous[k - 1] = j;
            }

            current_edge = current_edge->_next_edge;
        }
    }

    // Reconstroi o caminho mínimo a partir do nó de destino até a origem.
    for(size_t target = node_id_2; target != -1; target = previous[target - 1]) {
        path.push_back(target);
    }
    reverse(path.begin(), path.end());

    // Verifica se o primeiro nó do caminho é a origem, se não, caminho inválido.
    if (path[0] != node_id_1) {
        cout << endl << "Não existe conexão entre " << node_id_1 << " e " << node_id_2 << endl;
        path.clear();
    }

    return path;
}

// Algoritmo de Floyd para encontrar o caminho mínimo entre dois vérticies.
vector<size_t> Graph::floyd(size_t node_id_1, size_t node_id_2) {
    // Verifica se o grafo é vazio.
    if (_number_of_nodes == 0) {
        cout << "Grafo vazio!" << endl;
        return {};
    }

    // Verifica se o grafo é direcionado.
    if(!_directed) {
        cout << endl << "Grafo não direcionado!" << endl;
        return {};
    }

    if(node_id_1 == node_id_2) {
        return {node_id_1};
    }

    // Busca os dois nós.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    if(node_1 == nullptr || node_2 == nullptr) {
        cout << endl << "Não é possível achar o caminho mínimo (algum(s) dos dois nós não existe(m))!" << endl;
        return {};
    }

    // Inicialização
    const float INF = std::numeric_limits<float>::infinity();                               // Define uma constante para representar o infinito.
    vector<vector<float>> dist(_number_of_nodes, vector<float>(_number_of_nodes, INF));     // Inicializa a matriz de distâncias com infinito.
    vector<vector<size_t>> next(_number_of_nodes, vector<size_t>(_number_of_nodes, -1));    // Inicializa a matriz de predecessores com -1.

    // Preenche a matriz de distâncias com as distâncias diretas e a matriz de predecessores.
    Node* node = _first;
    while(node != nullptr) {
        Edge* edge = node->_first_edge;
        while(edge != nullptr) {
            size_t u = node->_id - 1; // Índice do nó atual.
            size_t v = edge->_target_id - 1; // Índice do nó destino.
            float weight = edge->_weight; // Peso da aresta.

            // Atualiza a distância e o próximo nó no caminho mais curto.
            dist[u][v] = weight;
            next[u][v] = v;
            edge = edge->_next_edge;
        }
        node = node->_next_node;
    }

    // A distância de um nó para ele mesmo é zero.
    for (size_t i = 0; i < _number_of_nodes; ++i) {
        dist[i][i] = 0;
    }

    // Aplica o algoritmo de Floyd para encontrar as distâncias mínimas entre todos os pares de vértices.
    for(size_t k = 0; k < _number_of_nodes; ++k) {
        for(size_t i = 0; i < _number_of_nodes; ++i) {
            for(size_t j = 0; j < _number_of_nodes; ++j) {
                // Atualiza a distância se uma rota mais curta for encontrada através do vértice k.
                if (dist[i][k] < INF && dist[k][j] < INF && dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    // Reconstruir o caminho mínimo do nó 1 para o nó 2.
    vector<size_t> path;
    size_t u = node_id_1 - 1;
    size_t v = node_id_2 - 1;

    // Verifica se existe um caminho entre os dois nós.
    if (next[u][v] == -1) {
        cout << "Não existe conexão entre " << node_id_1 << " e " << node_id_2 << endl;
        return {}; // Retorna um vetor vazio se não houver caminho.
    }

    // Adiciona o nó de origem ao caminho.
    path.push_back(node_id_1);

    // Adiciona os nós ao longo do caminho até alcançar o nó de destino.
    while (u != v) {
        u = next[u][v]; // Move para o próximo nó no caminho.
        path.push_back(u + 1); // Adiciona o próximo nó ao caminho.
    }

    return path;
}






