#include "Graph.hpp"
#include "defines.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 6) {
        cerr << "Programa nao inicializado da forma correta!" << endl << "Execute: <arquivo_entrada> <arquivo_saida> <Op_Direc> <Op_PesoAresta> <Op_PesoNos>" << endl;
        return 1;
    }

    string inputFileName = argv[1];
    string outputFileName = argv[2];
    bool isDirected = stoi(argv[3]) == 1;
    bool isWeightedEdges = stoi(argv[4]) == 1;
    bool isWeightedNodes = stoi(argv[5]) == 1;

    // Criando um arquivo de entrada.
    ifstream input_file(inputFileName);
    if(!input_file) {
        cerr << "Erro ao abrir o arquivo de entrada." << endl;
        return 1;
    }

    // Criando o grafo.
    Graph grafo = Graph(input_file, isDirected, isWeightedEdges, isWeightedNodes);

    int choise = 0;
    while(choise != -1) {
        if(choise == 0) {
            cout << endl << "Grafo atual:" << endl;
            grafo.print_graph();
            cout << endl << "Escolha alguma das funcionalidades a seguir (aperte o numero):" << endl;
            cout << "[1] - Gerar arquivo de saida com grafo. [2] - Imprime informacoes do grafo. [3] Encontrar o fecho transitivo direto de um vertice." << endl;
            cout << "[4] - Fecho transitivo indireto de um vertice. [5] - Encontrar o caminho minimo entre dois vertices usando Djkstra." << endl;
            cout << "[6] - Encontrar o caminho minimo entre dois vertices usando Floyd. [7] - Encontrar a AGM usando Prim. [8] - Encontrar a AGM usando Kruskal" << endl;
            cout << "[9] - Gerar a arvore dada pela ordem de caminhamento em profundidade. [10] - Encontrar raio, diametro, centro e periferia do grafo." << endl;
            cout << "[11] - Encontrar vertices de articulacao. [-1 || any] - SAIR" << endl;
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
            }
            break;
            case 2:
                grafo.print_graph_infos();
                choise = 0;
                break;
            case 3: {
                cout << endl << "Escolha um vertice do grafo: ";
                size_t vertice;
                cin >> vertice;
                grafo.direct_transitive_closure(vertice);
                choise = 0;
                break;
            }
            break;
            case 4: {
                cout << endl << "Escolha um vertice do grafo: ";
                size_t vertice;
                cin >> vertice;
                grafo.indirect_transitive_closure(vertice);
                choise = 0;
                break;
            }
            case 5: {
                size_t vertice1;
                size_t vertice2;
                cout << endl << "Escolha o primeiro vertice: ";
                cin >> vertice1;
                cout << endl << "Escolha o segundo vertice: ";
                cin >> vertice2;
                grafo.dijkstra(vertice1, vertice2);
                choise = 0;
                break;
            }
            case 6: {
                size_t vertice1;
                size_t vertice2;
                cout << endl << "Escolha o primeiro vertice: ";
                cin >> vertice1;
                cout << endl << "Escolha o segundo vertice: ";
                cin >> vertice2;
                grafo.floyd(vertice1, vertice2);

                choise = 0;
                break;
            }
            case 7: {
                vector<size_t> values;
                size_t value;
                cout << endl << "Digite valores de size_t (pressione -1 para encerrar): " << endl;
                while (cin >> value) {
                    if(value == -1) {
                        choise = 0;
                        break;
                    }
                    values.push_back(value);
                }
                grafo.prim_minimum_spanning_tree(values);
                choise = 0;
                break;
            }
            case 8: {
                vector<size_t> values;
                size_t value;
                cout << endl << "Digite valores de size_t (pressione -1 para encerrar): " << endl;
                while (cin >> value) {
                    if(value == -1) {
                        choise = 0;
                        break;
                    }
                    values.push_back(value);
                }
                grafo.kruskal_minimum_spanning_tree(values);
                choise = 0;
                break;
            }
            case 9: {
                cout << endl << "Escolha um vertice do grafo: ";
                size_t vertice;
                cin >> vertice;
                grafo.dfs_tree_with_back_edges(vertice);
                choise = 0;
                break;
            }
            case 10: {
                grafo.calculate_radius_diameter_center_periphery();
                choise = 0;
                break;
            }
            case 11: {
                grafo.articulation_points();
                choise = 0;
                break;
            }

            default:
                choise = -1;
                break;
        }
    }
    return 0;
}
