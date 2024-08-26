#include "Graph.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,""); // Para permitir acentuações nos prints.

    // Criando o grafo.
    ifstream input_file;
    string filename = "instances_example/50nD.dat";
    input_file.open(filename);
    Graph grafo = Graph(input_file, filename);
    grafo.print_graph();

    // Gerando o arquivo do grafo.
    ofstream output_file("grafo.dot");
    if(!output_file) {
        cout << "Erro ao criar o arquivo." << endl;
        return 1;
    }
    grafo.print_graph(output_file);



//    // Caminho mínimo usando Djkstra;
//    vector<size_t> min_path_djkstra = grafo.dijkstra(1, 47);
//    cout << endl << "Caminho minimo." << endl << "[";
//    for(size_t i : min_path_djkstra) {
//        cout << i << ",";
//    }
//    cout << "]" << endl;
//
//    // Caminho mínimo usando Floyd;
//    vector<size_t> min_path_floyd = grafo.floyd(1, 47);
//    cout << endl << "Caminho minimo." << endl << "[";
//    for(size_t i : min_path_floyd) {
//        cout << i << ",";
//    }
//    cout << "]" << endl;

    // Fecho transitivo direto.
//    vector<size_t> direct_closure = grafo.direct_transitive_closure(1);
//    cout << endl << "Fecho transitivo direito do nó 1: " << endl << "[";
//    for(size_t i : direct_closure) {
//        cout << i << ",";
//    }
//    cout << "]" << endl;
//
//     Fecho transitivo indireto.
//    vector<size_t> indirect_closure = grafo.indirect_transitive_closure(3);
//    cout << endl << "Fecho transitivo indireto do nó 3:" << endl << "[";
//    for(size_t i : indirect_closure) {
//        cout << i << ",";
//    }
//    cout << "]" << endl;
    return 0;
}
