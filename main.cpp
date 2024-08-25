#include "Graph.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,""); // Para permitir acentua��es nos prints.

    // Criando o grafo.
    Graph grafo = Graph(false, false, false);
    grafo.print_graph();

    // Adicionando n�s.
    cout << endl << "Adicionando n�s." << endl;
    grafo.add_node(0);
    grafo.add_node(1);
    grafo.add_node(2);
    grafo.add_node(3);
    grafo.print_graph();

    // Adicionar arestas.
    cout << endl << "Adicionando arestas." << endl;
    grafo.add_edge(0, 1);
    grafo.add_edge(1, 2);
    grafo.add_edge(1, 3);
    grafo.add_edge(2, 3);
    grafo.print_graph();

    // Informa��es.
    cout << endl << "Informa��es." << endl;
    grafo.print_graph_infos();
    grafo.print_node_infos(0);
    grafo.print_node_infos(1);
    grafo.print_node_infos(2);
    grafo.print_node_infos(3);

    // Removendo n�s.
//    cout << endl << "Removendo n�s." << endl;
//    grafo.remove_node(0);
//    grafo.remove_node(1);
//    grafo.remove_node(2);
//    grafo.remove_node(3);
//    grafo.print_graph();

    // Removendo arestas.
//    cout << endl << "Removendo arestas." << endl;
//    grafo.remove_edge(0, 1);
//    grafo.remove_edge(1, 2);
//    grafo.remove_edge(1, 3);
//    grafo.remove_edge(2, 3);
//    grafo.print_graph();

    // Informa��es.
    cout << endl << "Informa��es." << endl;
    grafo.print_graph_infos();
    grafo.print_node_infos(0);
    grafo.print_node_infos(1);
    grafo.print_node_infos(2);
    grafo.print_node_infos(3);

    /*ifstream input_file;
    input_file.open("instances_example/50nU.dat");
    Graph grafo(input_file);
    grafo.print_graph();

    ofstream output_file("grafo.dot");
    if(!output_file) {
        cout << "Erro ao criar o arquivo." << endl;
        return 1;
    }
    grafo.print_graph(output_file);
    */

    /*
    // Criando o grafo.
    Graph grafo;
    grafo.setDirected(1);
    grafo.print_graph();

    // Adicionando n�s.
    cout << endl << "Adicionando n�s." << endl;
    grafo.add_node(0);
    grafo.add_node(1);
    grafo.add_node(2);
    grafo.add_node(3);
    grafo.print_graph();

    // Adicionar arestas.
    cout << endl << "Adicionando arestas." << endl;
    grafo.add_edge(0, 0);
    grafo.add_edge(0, 1);
    grafo.add_edge(1, 2);
    grafo.add_edge(1, 3);
    grafo.add_edge(2, 3);
    grafo.add_edge(3, 3);
    grafo.print_graph();

    // Informa��es.
    cout << endl << "Informa��es." << endl;
    grafo.print_graph_infos();
    grafo.print_node_infos(0);
    grafo.print_node_infos(1);
    grafo.print_node_infos(2);
    grafo.print_node_infos(3);

    // Gerando o arquivo do grafo.
    ofstream output_file("grafo.dot");
    if(!output_file) {
        cout << "Erro ao criar o arquivo." << endl;
        return 1;
    }
    grafo.print_graph(output_file);

    // Removendo n�s.
    cout << endl << "Removendo n�s." << endl;
    grafo.remove_node(0);
    grafo.remove_node(1);
    grafo.remove_node(2);
    grafo.remove_node(3);
    grafo.print_graph();

    // Informa��es.
    cout << endl << "Informa��es." << endl;
    grafo.print_graph_infos();
    grafo.print_node_infos(0);
    grafo.print_node_infos(1);
    grafo.print_node_infos(2);
    grafo.print_node_infos(3);

    // Readicionando n�s e arestas.
    cout << endl << "Readicionando n�s e arestas." << endl;
    grafo.add_node(0);
    grafo.add_node(1);
    grafo.add_node(2);
    grafo.add_node(3);
    grafo.add_edge(0, 0);
    grafo.add_edge(0, 1);
    grafo.add_edge(1, 2);
    grafo.add_edge(1, 3);
    grafo.add_edge(2, 3);
    grafo.add_edge(3, 3);
    grafo.print_graph();

    // Removendo arestas.
    cout << endl << "Removendo arestas." << endl;
    grafo.remove_edge(0, 0);
    grafo.remove_edge(0, 1);
    grafo.remove_edge(1, 2);
    grafo.remove_edge(1, 3);
    grafo.remove_edge(2, 3);
    grafo.remove_edge(3, 3);
    grafo.print_graph();

    // Removendo n�s.
    cout << endl << "Removendo n�s novamente." << endl;
    grafo.remove_node(0);
    grafo.remove_node(1);
    grafo.remove_node(2);
    grafo.remove_node(3);
    grafo.print_graph();

    // Informa��es.
    cout << endl << "Informa��es." << endl;
    grafo.print_graph_infos();
    grafo.print_node_infos(0);
    grafo.print_node_infos(1);
    grafo.print_node_infos(2);
    grafo.print_node_infos(3);
    */

//    Graph grafo;
//    grafo.setDirected(1);
//    grafo.print_graph();
//
//    grafo.add_node(0);
//    grafo.add_node(1);
//    grafo.add_node(2);
//    grafo.add_node(3);
//    grafo.add_node(4);
//    grafo.print_graph();
//
//    grafo.add_edge(0, 0);
//    grafo.add_edge(0, 1);
//    grafo.add_edge(0, 1);
//    grafo.add_edge(0, 3);
//    grafo.add_edge(2, 1);
//    grafo.add_edge(3, 1);
//    grafo.add_edge(4, 1);
//    grafo.print_graph();
//
//    vector<size_t> direct_closure = grafo.direct_transitive_closure(0);
//    cout << endl << "Fecho transitivo direito do n� 0:" << endl << "[";
//    for(size_t i : direct_closure) {
//        cout << i << ",";
//    }
//    cout << "]" << endl;
//
//    vector<size_t> indirect_closure = grafo.indirect_transitive_closure(1);
//    cout << endl << "Fecho transitivo indireto do n� 1:" << endl << "[";
//    for(size_t i : indirect_closure) {
//        cout << i << ",";
//    }
//    cout << "]" << endl;

    return 0;
}
