#include "Graph.hpp"
#include <locale.h>

using namespace std;

// Construtor a partir de uma inst�ncia lida de um arquivo.
Graph::Graph(ifstream& instance, bool isDirected, bool isWeightedEdges, bool isWeightedNodes) {
    if (!instance.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return;
    }

    // Inicializa as propriedades do grafo.
    _number_of_nodes = 0;
    _number_of_edges = 0;
    _directed = isDirected;
    _weighted_edges = isWeightedEdges;
    _weighted_nodes = isWeightedNodes;
    _first = nullptr;
    _last = nullptr;

    string line;

    // L� e processa o n�mero de n�s e arestas a partir do arquivo.
    while(getline(instance, line)) {
        if(line[0] == '#') continue;

        // Processa n�s com peso.
        if(isWeightedNodes && line.find("param w :=") != string::npos) {
            while(getline(instance, line) && line[0] != ';') {
                stringstream ss(line);
                size_t node_id;
                float weight;
                while(ss >> node_id >> weight) {
                    add_node(node_id, weight);
                }
            }
        }

        // Processa arestas.
        if(line.find("set E :=") != string::npos) {
            while(getline(instance, line) && line[0] != ';') {
                stringstream ss(line);
                size_t node_id1, node_id2;
                char comma, par1, par2;
                while(ss >> par1 >> node_id1 >> comma >> node_id2 >> par2) {
                    if(comma != ',') {
                        break;
                    }
                    add_edge(node_id1, node_id2);
                }
            }
        }
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
