#include "Graph.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    setlocale(LC_ALL,""); // Para permitir acentuações nos prints.

    // Criando o grafo.
    ifstream input_file;
    input_file.open("instances_example/5nD.dat");
    Graph grafo = Graph(input_file, true, true, false);
    grafo.print_graph();

    // Gerando o arquivo do grafo.
    ofstream output_file("grafo.dot");
    if(!output_file) {
        cout << "Erro ao criar o arquivo." << endl;
        return 1;
    }
    grafo.print_graph(output_file);

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
