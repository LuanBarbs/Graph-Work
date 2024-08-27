#include "Graph.hpp"
#include <locale.h>

using namespace std;

// Construtor a partir de uma inst�ncia lida de um arquivo.
Graph::Graph(ifstream& instance, bool isDirected, bool isWeightedEdges, bool IsWeightedNodes) {
    if (!instance.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return;
    }

    // Inicializa as propriedades do grafo.
    size_t numberOfNodes;
    instance >> numberOfNodes;
    _number_of_nodes = 0;
    _number_of_edges = 0;
    _directed = isDirected;
    _weighted_edges = isWeightedEdges;
    _weighted_nodes = IsWeightedNodes;
    _first = nullptr;
    _last = nullptr;

    size_t node_id, target_id;
    float edge_weight;

    // Adiciona todos os n�s ao grafo.
    for (size_t i = 1; i <= numberOfNodes; ++i) {
        add_node(i);
    }

    // Adiciona as arestas ao grafo.
    while(instance >> node_id >> target_id >> edge_weight) {
        add_edge(node_id, target_id, edge_weight);
    }
}

// Construtor padr�o.
Graph::Graph(bool isDirected, bool isWeightedEdges, bool IsWeightedNodes) {
    // Inicializa��o do grafo vazio (sem n�s ou arestas).
    _number_of_nodes = 0;
    _number_of_edges = 0;
    _directed = isDirected;
    _weighted_edges = isWeightedEdges;
    _weighted_nodes = IsWeightedNodes;
    _first = nullptr;
    _last = nullptr;
}

// Destrutor padr�o.
Graph::~Graph() {
    // Liberando a mem�ria alocada para o grafo.
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

// M�todo que remove um n� do grafo.
void Graph::remove_node(size_t node_id) {
    // Procura o n� a ser removido.
    Node* remove_node = search_node_by_id(node_id);

    // N� a ser removido n�o existe no grafo.
    if(remove_node == nullptr) {
        cout << "Na a ser removido nao encontrado" << endl;
        return;
    }

    // Removendo as arestas da lista de adjac�ncias do n�.
    Edge* remove_node_edge = remove_node->_first_edge;
    while(remove_node_edge != nullptr) {
        Edge* next_edge = remove_node_edge->_next_edge;
        delete remove_node_edge;
        _number_of_edges--;
        remove_node_edge = next_edge;
    }

    // Ajustando os ponteiros.
    if(_number_of_nodes == 1) { // N� a ser removido � o �nico.
        _first = nullptr;
        _last = nullptr;
    }
    else if(remove_node == _first) { // N� a ser removido � o primerio mas n�o o �nico.
        remove_node->_next_node->_previous_node = nullptr;
        _first = remove_node->_next_node;
    }
    else if(remove_node == _last) { // N� a ser removido � o �ltimo.
        remove_node->_previous_node->_next_node = nullptr;
        _last = remove_node->_previous_node;
    }
    else { //N� a ser removido n�o � o primeiro nem o �ltimo.
        remove_node->_previous_node->_next_node = remove_node->_next_node;
        remove_node->_next_node->_previous_node = remove_node->_previous_node;
    }

    // Removendo o n�.
    delete remove_node;

    // Decrementando a quantidade de n�s do grafo.
    _number_of_nodes--;

    // Removendo as arestas que tem o n� como alvo.
    Node* current_node = _first;
    while(current_node != nullptr) {
        Edge* current_edge = current_node->_first_edge;
        Edge* previous_edge = nullptr;
        while(current_edge != nullptr) {
            if(current_edge->_target_id == node_id) {
                if(previous_edge == nullptr) { // A aresta a ser removida � a primeira da lista de adjac�ncias.
                    current_node->_first_edge = current_edge->_next_edge;
                }
                else { // A aresta a ser removido n�o � a primeira (n�o importa se � a �ltima).
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

// M�todo que remove uma aresta entre dois n�s.
void Graph::remove_edge(size_t node_id_1, size_t node_id_2) {
    // Procurando os n�s pelo id.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    // Verificando se os n�s existem.
    if(node_1 == nullptr || node_2 == nullptr) {
        cout << "O(s) no(s) nao existe(m)!" << endl;
        return;
    }

    // Procurando a aresta a ser removida no n� 1.
    Edge* current_edge = node_1->_first_edge;
    Edge* previous_edge = nullptr;
    while(current_edge != nullptr) {
        if(current_edge->_target_id == node_id_2) break;
        previous_edge = current_edge;
        current_edge = current_edge->_next_edge;
    }

    // Verificando se a aresta existe.
    if(current_edge == nullptr) {
        cout << "A aresta nao existe para o no " << node_id_1 << "!" << endl;
        return;
    }

    // Ajustando os ponteiros.
    if(current_edge->_next_edge == nullptr && previous_edge == nullptr) { // A aresta a ser removida � a �nica.
        node_1->_first_edge = nullptr;
    }
    else if(previous_edge == nullptr) { // A aresta a ser removida � a primeira.
        node_1->_first_edge = current_edge->_next_edge;
    }
    else { // A aresta a ser removida n�o � a primeira.
        previous_edge->_next_edge = current_edge->_next_edge;
    }

    // Deletando a aresta.
    delete current_edge;

    // Decrementando o n�mero de arestas do primeiro n�.
    node_1->_number_of_edges--;

    if(!_directed) {
        // Procurando a aresta a ser removida no n� 2 se o grafo n�o � direcionado.
        current_edge = node_2->_first_edge;
        previous_edge = nullptr;
        while(current_edge != nullptr) {
            if(current_edge->_target_id == node_id_1) break;
            previous_edge = current_edge;
            current_edge = current_edge->_next_edge;
        }

        // Verificando se a aresta existe.
        if(current_edge == nullptr) {
            cout << "A aresta nao existe para o no " << node_id_2 << "!" << endl;
            return;
        }

        // Ajustando os ponteiros.
        if(current_edge->_next_edge == nullptr && previous_edge == nullptr) { // A aresta a ser removida � a �nica.
            node_2->_first_edge = nullptr;
        }
        else if(previous_edge == nullptr) { // A aresta a ser removida � a primeira.
            node_2->_first_edge = current_edge->_next_edge;
        }
        else { // A aresta a ser removida n�o � a primeira.
            previous_edge->_next_edge = current_edge->_next_edge;
        }

        // Deletando a aresta.
        delete current_edge;

        // Decrementando o n�mero de arestas do segundo n�.
        node_2->_number_of_edges--;
    }

    // Decrementando o n�mero de arestas do grafo.
    _number_of_edges--;
}

// M�todo que adiciona um novo n� ao grafo.
void Graph::add_node(size_t node_id, float weight) {
    // Verica se n� j� existe.
    Node* node_verify = search_node_by_id(node_id);
    if(node_verify != nullptr) {
        cout << "No ja existe no grafo!" << endl;
        return;
    }

    // Criando o novo n�.
    Node* new_node = new Node;
    new_node->_number_of_edges = 0;
    new_node->_id = node_id;
    new_node->_weight = weight;
    new_node->_first_edge = nullptr;
    new_node->_next_node = nullptr;
    new_node->_previous_node = nullptr;

    // Adicionando n� ao grafo.
    if(_number_of_nodes == 0) { // Grafo vazio.
        _first = new_node;
        _last = new_node;
    }
    else { // Grafo n�o vazio.
        _last->_next_node = new_node;
        new_node->_previous_node = _last;
        _last = new_node;
    }

    // Incrementa o n�mero de n�s do grafo.
    _number_of_nodes++;
}

// Met�do que adiciona uma nova aresta entre dois n�s.
void Graph::add_edge(size_t node_id_1, size_t node_id_2, float weight) {
    // Procurando os n�s alvos.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    if(node_1 != nullptr && node_2 != nullptr) {
        // Criando a nova aresta para o n� 1.
        Edge* new_edge = new Edge;
        new_edge->_next_edge = nullptr;
        new_edge->_weight = weight;
        new_edge->_target_id = node_id_2;

        // Adicionado a aresta para o n� 1.
        if(node_1->_first_edge == nullptr) { // N�o existe lista de adjac�ncias para o n� 1.
            node_1->_first_edge = new_edge;
        }
        else { // Existe lista de adjac�ncias para o n� 1.
            Edge* current_edge = node_1->_first_edge;
            while(current_edge->_next_edge != nullptr) {
                current_edge = current_edge->_next_edge;
            }
            current_edge->_next_edge = new_edge;
        }
        // Incrementa o n�mero de arestas do n� 1.
        node_1->_number_of_edges++;

        if(!_directed) {
            // Criando a nova aresta para o n� 2 se o grafo n�o for direcionado.
            Edge* new_edge2 = new Edge;
            new_edge2->_next_edge = nullptr;
            new_edge2->_weight = weight;
            new_edge2->_target_id = node_id_1;

            // Adicionando a aresta para o n� 2.
            if(node_2->_first_edge == nullptr) { // N�o existe lista de adjac�ncias para o n� 2.
                node_2->_first_edge = new_edge2;
            }
            else { // Existe lista de adjac�ncias para o n� 2.
                Edge* current_edge = node_2->_first_edge;
                while(current_edge->_next_edge != nullptr) {
                    current_edge = current_edge->_next_edge;
                }
                current_edge->_next_edge = new_edge2;
            }
            // Incrementa o n�mero de arestas do n� 2.
            node_2->_number_of_edges++;
        }

        // Incrementa o n�mero de arestas do grafo.
        _number_of_edges++;
    }
    else {
        cout << "No(s) nao existe(m) no grafo!" << endl;
    }
}

// M�todo para printar uma representa��o do grafo em um arquivo de sa�da.
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

// M�todo para printar uma representa��o do grafo no console.
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

// M�todo que procura um n� pelo id e o retorna.
Node* Graph::search_node_by_id(size_t node_id) {
    Node* current_node = _first;
    while(current_node != nullptr) {
        if(current_node->_id == node_id) return current_node;
        current_node = current_node->_next_node;
    }
    return nullptr;
}

// Imprimindo informa��es gerais sobre o grafo.
void Graph::print_graph_infos() {
    cout << endl << "Informacoees sobre o grafo: " << endl;
    cout << "Quantidade de nos: " << _number_of_nodes << endl;
    cout << "Quantidade de arestas: " << _number_of_edges << endl;
    cout << "Direcionado: " << _directed << endl;
}

// Imprimindo informa��es gerais sobre um n� espec�fico.
void Graph::print_node_infos(size_t node_id) {
    Node* current_node = search_node_by_id(node_id);

    // N� n�o existe no grafo.
    if(current_node == nullptr) {
        cout << endl << "Nao existem informa�oes sobre esse no!" << endl;
        return;
    }

    cout << endl << "Informa�oes sobre o no " << current_node->_id << ": " << endl;

    // Lista de adjac�ncias.
    if(current_node->_first_edge != nullptr) {
        cout << "No alvo da primeira aresta: " << current_node->_first_edge->_target_id << endl;
    }
    else {
        cout << "No sem lista de adjacencias." << endl;
    }

    // N� anterior na lista.
    if(current_node->_previous_node != nullptr) {
        cout << "No anterior na lista: " << current_node->_previous_node->_id << endl;
    }
    else {
        cout << "Esse eh o primeiro no da lista." << endl;
    }

    // Pr�ximo n� na lista.
    if(current_node->_next_node != nullptr) {
        cout << "Proximo no na lista: " << current_node->_next_node->_id << endl;
    }
    else {
        cout << "Esse eh o ultimo no da lista." << endl;
    }

    // Quantidade de arestas.
    cout << "Quantidade de arestas: " << current_node->_number_of_edges << endl;
}

// Fecho Transitivo Direto.
void Graph::direct_transitive_closure(size_t node_id) {
    if(!_directed) {
        cout << endl << "O grafo nao eh direcionado!" << endl;
        return;
    }
    Node* node = search_node_by_id(node_id);
    if(node == nullptr) {
        cout << endl << "O no nao existe no grafo!" << endl;
        return;
    }
    vector<size_t> closure;
    Edge* current_edge = node->_first_edge;
    while(current_edge != nullptr) {
        closure.push_back(current_edge->_target_id);
        current_edge = current_edge->_next_edge;
    }

    cout << endl << "Fecho transitivo direto do no " << node_id << ":" << endl;
    for(size_t i : closure) {
        cout << i << " ";
    }
    cout << endl;
}

// Fecho Transitivo Indireto.
void Graph::indirect_transitive_closure(size_t node_id) {
    if(!_directed) {
        cout << endl << "O grafo nao e direcionado!" << endl;
        return;
    }
    Node* node = search_node_by_id(node_id);
    if(node == nullptr) {
        cout << endl << "O no nao existe no grafo!" << endl;
        return;
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

    cout << endl << "Fecho transitivo indireto do no " << node_id << ":" << endl;
    for(size_t i : closure) {
        cout << i << " ";
    }
    cout << endl;
}

// Algoritmo de Djkstra para encontrar o caminho m�nimo entre dois v�rticies.
void Graph::dijkstra(size_t node_id_1, size_t node_id_2) {
    // Verifica se o grafo � vazio.
    if(_number_of_nodes == 0) {
        cout << endl << "Grafo vazio!" << endl;
        return;
    }

    // Busca os dois n�s.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    if(node_1 == nullptr || node_2 == nullptr) {
        cout << endl << "Nao eh possivel achar o caminho minimo (algum(s) dos dois nos n�o existe(m))!" << endl;
        return;
    }

    // Inicializa��o.
    const float INF = numeric_limits<float>::infinity(); // Define uma constante para representar o infinito.
    vector<float> dist(_number_of_nodes, INF);           // Vetor de dist�ncias pi. Todos os valores inicializados com infinito.
    vector<size_t> S;                                    // Vetor para armazenar o conjunto de n�s cujas dist�ncias m�nimas j� foram determinadas.
    vector<size_t> S_bar;                                // Vetor que armazena o conjunto de n�s cujas dist�ncias m�nimas ainda n�o foram determinadas.
    vector<size_t> previous(_number_of_nodes, -1);       // Vetor para armazenar os predecessores.
    vector<size_t> path;                                 // Vetor para montar o caminho.

    // Adiciona o n� origem em S.
    S.push_back(node_id_1);

    // A dist�ncia do primeiro n� para ele mesmo � zero.
    dist[node_id_1-1] = 0;

    // Adiciona os outros n�s em S_bar.
    Node* node = _first;
    while(node != nullptr) {
        if(node->_id != node_id_1) {
            S_bar.push_back(node->_id);
        }
        node = node->_next_node;
    }

    // Adiciona as dist�ncias para os n�s com conex�es diretas para o n� 1.
    Edge* edge = node_1->_first_edge;
    while(edge != nullptr) {
        dist[edge->_target_id-1] = edge->_weight;
        previous[edge->_target_id-1] = node_id_1;
        edge = edge->_next_edge;
    }

    while(!S_bar.empty()) {
        // Seleciona j de S_bar com a menor dist�ncia para o n� 1.
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

        // Atualiza as dist�ncias para os vizinhos de j.
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

    // Reconstroi o caminho m�nimo a partir do n� de destino at� a origem.
    for(size_t target = node_id_2; target != -1; target = previous[target - 1]) {
        path.push_back(target);
    }
    reverse(path.begin(), path.end());

    // Verifica se o primeiro n� do caminho � a origem, se n�o, caminho inv�lido.
    if (path[0] != node_id_1) {
        cout << endl << "Nao existe conexao entre " << node_id_1 << " e " << node_id_2 << endl;
        return;
    }

    cout << endl << "Caminho minimo entre " << node_id_1 << " e " << node_id_2 << " usando o algoritmo de Djkstra: " << endl;
    for(size_t i : path) {
        cout << i << " ";
    }
    cout << endl;
}

// Algoritmo de Floyd para encontrar o caminho m�nimo entre dois v�rticies.
void Graph::floyd(size_t node_id_1, size_t node_id_2) {
    // Verifica se o grafo � vazio.
    if (_number_of_nodes == 0) {
        cout << "Grafo vazio!" << endl;
        return;
    }

    // Busca os dois n�s.
    Node* node_1 = search_node_by_id(node_id_1);
    Node* node_2 = search_node_by_id(node_id_2);

    if(node_1 == nullptr || node_2 == nullptr) {
        cout << endl << "Nao eh possivel achar o caminho minimo (algum(s) dos dois nos n�o existe(m))!" << endl;
        return;
    }

    // Inicializa��o
    const float INF = numeric_limits<float>::infinity();                                  // Define uma constante para representar o infinito.
    vector<vector<float>> dist(_number_of_nodes, vector<float>(_number_of_nodes, INF));   // Inicializa a matriz de dist�ncias com infinito.
    vector<vector<size_t>> next(_number_of_nodes, vector<size_t>(_number_of_nodes, -1));  // Inicializa a matriz de predecessores com -1.

    // Preenche a matriz de dist�ncias com as dist�ncias diretas e a matriz de predecessores.
    Node* node = _first;
    while(node != nullptr) {
        Edge* edge = node->_first_edge;
        while(edge != nullptr) {
            size_t u = node->_id - 1; // �ndice do n� atual.
            size_t v = edge->_target_id - 1; // �ndice do n� destino.
            float weight = edge->_weight; // Peso da aresta.

            // Atualiza a dist�ncia e o pr�ximo n� no caminho mais curto.
            dist[u][v] = weight;
            next[u][v] = v;
            edge = edge->_next_edge;
        }
        node = node->_next_node;
    }

    // A dist�ncia de um n� para ele mesmo � zero.
    for (size_t i = 0; i < _number_of_nodes; ++i) {
        dist[i][i] = 0;
    }

    // Aplica o algoritmo de Floyd para encontrar as dist�ncias m�nimas entre todos os pares de v�rtices.
    for(size_t k = 0; k < _number_of_nodes; ++k) {
        for(size_t i = 0; i < _number_of_nodes; ++i) {
            for(size_t j = 0; j < _number_of_nodes; ++j) {
                // Atualiza a dist�ncia se uma rota mais curta for encontrada atrav�s do v�rtice k.
                if (dist[i][k] < INF && dist[k][j] < INF && dist[i][j] > dist[i][k] + dist[k][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    // Reconstruir o caminho m�nimo do n� 1 para o n� 2.
    vector<size_t> path;
    size_t u = node_id_1 - 1;
    size_t v = node_id_2 - 1;

    // Verifica se existe um caminho entre os dois n�s.
    if (next[u][v] == -1 && node_id_1 != node_id_2) {
        cout << "Nao existe conexao entre " << node_id_1 << " e " << node_id_2 << endl;
        return;
    }

    // Adiciona o n� de origem ao caminho.
    path.push_back(node_id_1);

    // Adiciona os n�s ao longo do caminho at� alcan�ar o n� de destino.
    while (u != v) {
        u = next[u][v]; // Move para o pr�ximo n� no caminho.
        path.push_back(u + 1); // Adiciona o pr�ximo n� ao caminho.
    }

    cout << endl << "Caminho minimo entre " << node_id_1 << " e " << node_id_2 << " usando o algoritmo de Floyd: " << endl;
    for(size_t i : path) {
        cout << i << " ";
    }
    cout << endl;
}

// Raio, di�metro, centro e periferia do grafo.
void Graph::calculate_radius_diameter_center_periphery() {
    // Verifica se o grafo � vazio.
    if (_number_of_nodes == 0) {
        cout << "Grafo vazio!" << endl;
    }

    const float INF = numeric_limits<float>::infinity();

    // Matriz de dist�ncias inicializada com infinito.
    vector<vector<float>> dist(_number_of_nodes, vector<float>(_number_of_nodes, INF));

    /* Utiliza��o do algoritmo de Floyd para calcular as dist�ncias entre todos os pares de v�rtices do grafo. */

    // Inicializa as dist�ncias baseadas nas arestas do grafo.
    Node* node = _first;
    while(node != nullptr) {
        dist[node->_id - 1][node->_id - 1] = 0;
        Edge* edge = node->_first_edge;
        while(edge != nullptr) {
            dist[node->_id - 1][edge->_target_id - 1] = edge->_weight;
            if(!_directed) {
                dist[edge->_target_id - 1][node->_id - 1] = edge->_weight;
            }
            edge = edge->_next_edge;
        }
        node = node->_next_node;
    }

    // Calculando as dist�ncias.
    for(size_t k = 0; k < _number_of_nodes; k++) {
        for(size_t i = 0; i < _number_of_nodes; i++) {
            for(size_t j = 0; j < _number_of_nodes; j++) {
                if(dist[i][k] < INF && dist[k][j] < INF) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    // Vetor de excentricidades (inicializado com 0).
    vector<float> eccentricities(_number_of_nodes, 0);

    // Calcula as excentricidades de cada v�rtice.
    for(size_t i = 0; i < _number_of_nodes; i++) {
        for(size_t j = 0; j < _number_of_nodes; j++) {
            if(dist[i][j] < INF) {
                eccentricities[i] = max(eccentricities[i], dist[i][j]);
            }
        }
    }

    // Calcula raio e di�metro.
    float radius = INF;
    float diameter = 0;
    for(float ecc : eccentricities) {
        radius = min(radius, ecc);
        diameter = max(diameter, ecc);
    }

    // Determina o centro e a periferia do grafo.
    vector<size_t> center;
    vector<size_t> periphery;
    for(size_t i = 0; i < _number_of_nodes; i++) {
        if(eccentricities[i] == radius) {
            center.push_back(i + 1);
        }
        if(eccentricities[i] == diameter) {
            periphery.push_back(i + 1);
        }
    }

    // Imprime os resultados
    cout << endl << "Raio do grafo: " << radius << endl;
    cout << "Diametro do grafo: " << diameter << endl;

    cout << "Centro do grafo: ";
    for (size_t v : center) {
        cout << v << " ";
    }
    cout << endl;

    cout << "Periferia do grafo: ";
    for (size_t v : periphery) {
        cout << v << " ";
    }
    cout << endl;

}

// Algoritmo recursivo auxiliar para encontrar os v�rticies de articula��o.
void Graph::DFSArticulation(size_t u, vector<bool> &visited, vector<int> &disc, vector<int> &low, vector<int> &parent, vector<bool> &ap) {
    static int time = 0;
    int children = 0; // Contador de filhos.

    // Marca o n� atual como visitado.
    visited[u] = true;

    // Inicializar o tempo de descoberta e o valor baixo.
    disc[u] = low[u] = ++time;

    // Percorre todos os n�s adjacentes de u.
    Node* node = search_node_by_id(u);
    Edge* edge = node->_first_edge;
    while(edge != nullptr) {
        size_t v = edge->_target_id;

        // Se v n�o foi visitado, ele se torna um filho de u no grafo DFS.
        if(!visited[v]) {
            children++;
            parent[v] = u;

             // Fazer uma chamada recursiva para v.
            DFSArticulation(v, visited, disc, low, parent, ap);

            // Checar se a sub�rvore enraizada em v tem uma conex�o com um ancestral de u.
            low[u] = min(low[u], low[v]);

            // (1) u � um ponto de articula��o se for a raiz e tem dois ou mais filhos.
            if (parent[u] == -1 && children > 1) {
                ap[u] = true;
            }

            // (2) Se u n�o � raiz e low[v] >= disc[u], ent�o u � um ponto de articula��o.
            if (parent[u] != -1 && low[v] >= disc[u]) {
                ap[u] = true;
            }
        }
        // Atualizar low[u] para o valor mais baixo de descoberta de n�s adjacentes.
        else if (v != parent[u]) {
            low[u] = min(low[u], disc[v]);
        }
        edge = edge->_next_edge;
    }
}

// Algoritmo para encontrar os v�rtices de articula��o do grafo.
void Graph::articulation_points() {
    if(_number_of_nodes == 0) {
        cout << endl << "Grafo vazio!" << endl;
        return;
    }
    if(_directed) {
        cout << endl << "Grafo direcionado!" << endl;
        return;
    }
    vector<bool> visited(_number_of_nodes, false);  // Array de v�rtices visitado.
    vector<int> disc(_number_of_nodes, -1);         // Tempo de descoberta dos n�s.
    vector<int> low(_number_of_nodes, -1);          // Valor mais baixo de descoberta.
    vector<int> parent(_number_of_nodes, -1);       // Pais dos n�s.
    vector<bool> ap(_number_of_nodes, false);       // Array para armazenar pontos de articula��o.

    // Chama a fun��o DFSArticulation para cada n�.
    for(size_t i = 1; i <= _number_of_nodes; i++) {
        if(!visited[i]) {
            DFSArticulation(i, visited, disc, low, parent, ap);
        }
    }

    // Imprimir os pontos de articula��o
    cout << endl << "Pontos de Articulacao no grafo:" << endl;
    for (size_t i = 1; i <= _number_of_nodes; i++) {
        if (ap[i]) {
            cout << i << " ";
        }
    }
    cout << endl;
}

// Algoritmo de Prim para encontrar a �rvore Geradora M�nima.
void Graph::prim_minimum_spanning_tree(vector<size_t>& subset_X) {
    // Verifica se o grafo est� vazio.
    if(_number_of_nodes == 0) {
        cout << endl << "Grafo Vazio!" << endl;
        return;
    }
    // Verifica se subSet_X est�o vazio.
    if (subset_X.empty()) {
        cout << endl << "O conjunto de verticies X esta vazio!" << endl;
        return;
    }

    size_t n = subset_X.size();
    vector<bool> inMST(n, false);                            // Marca os v�rtices que j� est�o na AGM
    vector<float> key(n, numeric_limits<float>::max());      // Menor peso para conectar cada v�rtice � AGM
    vector<size_t> parent(n, -1);                            // Armazena o pai de cada v�rtice na AGM

    key[0] = 0;  // Come�a com o primeiro v�rtice de subset_X
    parent[0] = -1;  // O primeiro v�rtice n�o tem pai

    for (size_t count = 0; count < n - 1; ++count) {
        // Encontra o v�rtice com a menor chave que ainda n�o est� na AGM
        float minKey = numeric_limits<float>::max();
        size_t u = -1;

        for (size_t v = 0; v < n; ++v) {
            if (!inMST[v] && key[v] < minKey) {
                minKey = key[v];
                u = v;
            }
        }

        inMST[u] = true;  // Adiciona o v�rtice u � AGM

        Node* node = search_node_by_id(subset_X[u]);
        Edge* edge = node->_first_edge;
        while (edge) {
            size_t v = distance(subset_X.begin(), find(subset_X.begin(), subset_X.end(), edge->_target_id));

            // Se o grafo � direcionado, considera apenas as arestas saindo de `u` para `v`.
            // Se o grafo n�o � direcionado, considera arestas em ambas as dire��es.
            if (v < n && !inMST[v] && edge->_weight < key[v] &&
               (!_directed || (_directed && edge->_target_id != subset_X[u]))) {
                key[v] = edge->_weight;
                parent[v] = u;
            }
            edge = edge->_next_edge;
        }
    }

    cout << endl << "Arestas da Arvore Geradora M�nima usando o algoritmo de Prim:" << endl;
    for (size_t i = 1; i < n; ++i) {
        cout << "(" << subset_X[parent[i]] << " - " << subset_X[i] << ")\n";
    }
}

// Fun��o auxiliar ao algoritmo de Kruskal para encontrar o representante de um conjunto usando o caminho comprimido.
size_t find_set(size_t node, vector<size_t>& parent) {
    if (parent[node] != node) {
        parent[node] = find_set(parent[node], parent);
    }
    return parent[node];
}

// Fun��o auxiliar ao algoritmo de Kruskal para unir dois conjuntos.
void union_sets(size_t u, size_t v, vector<size_t>& parent, vector<size_t>& rank) {
    size_t root_u = find_set(u, parent);
    size_t root_v = find_set(v, parent);

    if (root_u != root_v) {
        if (rank[root_u] < rank[root_v]) {
            parent[root_u] = root_v;
        } else if (rank[root_u] > rank[root_v]) {
            parent[root_v] = root_u;
        } else {
            parent[root_v] = root_u;
            rank[root_u]++;
        }
    }
}

// Algoritmo de Kruskal para encontrar a �rvore Geradora M�nima.
void Graph::kruskal_minimum_spanning_tree(vector<size_t> &subset_X) {
    // Verifica se o grafo est� vazio.
    if(_number_of_nodes == 0) {
        cout << endl << "Grafo vazio!" << endl;
        return;
    }
    // Verifica se o conjunto X est� vazio.
    if (subset_X.empty()) {
        cout << endl << "O conjunto X esta vazio!" << endl;
        return;
    }

    vector<pair<float, pair<size_t, size_t>>> edges; // Vetor para armazenar as arestas do subgrafo.
    vector<size_t> parent(_number_of_nodes + 1);     // Vetor para armazenar o conjunto de pais dos n�s.
    vector<size_t> rank(_number_of_nodes + 1, 0);    // Vetor para armazenar a altura das �rvores de conjuntos.

    // Inicializar os conjuntos disjuntos, cada n� � seu pr�prio pai.
    for (size_t i = 0; i <= _number_of_nodes; ++i) {
        parent[i] = i;
    }

    // Percorrer o subconjunto X e coletar as arestas do subgrafo v�rtice-induzido.
    for (size_t node_id : subset_X) {
        Node* node = search_node_by_id(node_id);
        Edge* edge = node->_first_edge;
        while (edge != nullptr) {
            if (find(subset_X.begin(), subset_X.end(), edge->_target_id) != subset_X.end()) {
                edges.push_back({edge->_weight, {node->_id, edge->_target_id}});
            }
            edge = edge->_next_edge;
        }
    }

    // Ordenar as arestas em ordem crescente de pesos.
    sort(edges.begin(), edges.end());

    vector<pair<size_t, size_t>> mst_edges; // Armazena as arestas da AGM.
    size_t count = 0; // Contador de arestas adicionadas � AGM.

    // Processar as arestas na ordem crescente de pesos.
    for (const auto& edge : edges) {
        size_t u = edge.second.first;
        size_t v = edge.second.second;

        // Se u e v n�o est�o na mesma sub�rvore, unir as sub�rvores.
        if (find_set(u, parent) != find_set(v, parent)) {
            mst_edges.push_back({u, v});
            union_sets(u, v, parent, rank);
            count++;
        }

        // Se o n�mero de arestas na AGM � igual a |V|-1, parar o loop.
        if (count == subset_X.size() - 1) {
            break;
        }
    }

    // Imprimir as arestas da AGM.
    cout << endl << "Arestas da Arvore Geradora M�nima usando o algoritmo de Kruskal:" << endl;
    for (const auto& edge : mst_edges) {
        cout << "(" << edge.first << " - " << edge.second << ")" << endl;
    }
}

// M�todo auxiliar para encontrar a �rvore dada pela ordem de caminhamento em profundidade.
void Graph::dfs_tree_with_back_edges_recursive(size_t u, vector<bool> &visited, vector<size_t> &parent, vector<pair<size_t, size_t>> &tree_edges, vector<pair<size_t, size_t>> &back_edges) {
    visited[u] = true;
    Node* node = search_node_by_id(u);
    Edge* edge = node->_first_edge;

    while (edge != nullptr) {
        size_t v = edge->_target_id;
        if (!visited[v]) {
            parent[v] = u;
            tree_edges.emplace_back(u, v);
            dfs_tree_with_back_edges_recursive(v, visited, parent, tree_edges, back_edges);
        } else if (v != parent[u]) {
            back_edges.emplace_back(u, v);  // Aresta de retorno encontrada.
        }
        edge = edge->_next_edge;
    }
}

// M�todo para encontrar �rvore dada pela ordem de caminhamento em profundidade.
void Graph::dfs_tree_with_back_edges(size_t start_node_id) {
    vector<bool> visited(_number_of_nodes + 1, false); // Marca os v�rtices visitados.
    vector<size_t> parent(_number_of_nodes + 1, -1);   // Armazena o pai de cada n�.
    vector<pair<size_t, size_t>> tree_edges;           // Armazena as arestas da �rvore DFS.
    vector<pair<size_t, size_t>> back_edges;           // Armazena as arestas de retorno (back edges).

    // Inicia a DFS a partir do n� fornecido.
    dfs_tree_with_back_edges_recursive(start_node_id, visited, parent, tree_edges, back_edges);

    // Impress�o dos resultados.
    cout << endl << "Arestas da arvore DFS a partir do vertice " << start_node_id << ":" << endl;
    for (const auto& edge : tree_edges) {
        cout << "(" << edge.first << " - " << edge.second << ")" << endl;
    }

    cout << endl << "Arestas de retorno:" << endl;
    for (const auto& edge : back_edges) {
        cout << "(" << edge.first << " - " << edge.second << ")" << endl;
    }
}




