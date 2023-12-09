#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

const int MAX_SOCIOS = 1000; //tamanho máximo que pode ter de sócios em meu clube, fora os independentes

struct Socio {
    int numeroTitulo;
    string nomeTitular;
    string nomeDependente1;
    string nomeDependente2;
    bool pagamentoMensalidade;
};

struct RegistroEntradaSaida {
    int numeroTitulo;
    bool entrou;
};

struct Clube {
    Socio socios[MAX_SOCIOS];
    int numSocios = 0;

    void inserirSocio(Socio novoSocio) {
        if (numSocios < MAX_SOCIOS) {
            socios[numSocios] = novoSocio;
            numSocios++;
            cout << "Socio inserido com sucesso." << endl;
        } else {
            cout << "Limite de socios atingido." << endl;
        }
    }

    void listarSocios() {
        cout << "Lista de socios do clube:" << endl;
        if (numSocios == 0) {
            cout << "Nenhum socio cadastrado." << endl;
        } else {
            for (int i = 0; i < numSocios; ++i) {
                cout << "Título: " << socios[i].numeroTitulo << ", Nome: " << socios[i].nomeTitular << endl;
                cout << "Dependente 1: " << socios[i].nomeDependente1 << ", Dependente 2: " << socios[i].nomeDependente2 << endl;
                cout << "Pagamento em dia: " << (socios[i].pagamentoMensalidade ? "Sim" : "Não") << endl << endl;
            }
        }
    }

    void removerSocio(int numeroTitulo) {
        for (int i = 0; i < numSocios; ++i) {
            if (socios[i].numeroTitulo == numeroTitulo) {
                for (int j = i; j < numSocios - 1; ++j) {
                    socios[j] = socios[j + 1];
                }
                numSocios--;
                cout << "Socio removido com sucesso." << endl;
                return;
            }
        }
        cout << "Socio com titulo " << numeroTitulo << " nao encontrado." << endl;
    }

    void carregarSociosArquivo() {
        const string nomeArquivo = "novos_socios.txt";
        ifstream arquivo(nomeArquivo);
        string linha;

        if (arquivo.is_open()) {
            while (getline(arquivo, linha)) {
                stringstream ss(linha);
                Socio novoSocio;
                ss >> novoSocio.numeroTitulo >> novoSocio.nomeTitular >> novoSocio.nomeDependente1 >> novoSocio.nomeDependente2 >> novoSocio.pagamentoMensalidade;

                inserirSocio(novoSocio);
            }
            arquivo.close();
        } else {
            cout << "Erro ao abrir o arquivo " << nomeArquivo << endl;
        }
    }
};

RegistroEntradaSaida registros[MAX_SOCIOS];
int numRegistros = 0;

bool verificarInadimplencia(const Socio socios[], int numSocios, int numeroTitulo) {
    for (int i = 0; i < numSocios; ++i) {
        if (socios[i].numeroTitulo == numeroTitulo && !socios[i].pagamentoMensalidade) {
            return true;
        }
    }
    return false;
}

void registrarEntrada(Clube& clube, int numeroTitulo) {
    if (verificarInadimplencia(clube.socios, clube.numSocios, numeroTitulo)) {
        cout << "Socio de numero " << numeroTitulo << " esta inadimplente." << endl;
        return;
    }

    if (numRegistros < MAX_SOCIOS) {
        registros[numRegistros].numeroTitulo = numeroTitulo;
        registros[numRegistros].entrou = true;
        numRegistros++;
        cout << "Entrada registrada com sucesso." << endl;
    } else {
        cout << "Limite de registros atingido." << endl;
    }
}

void registrarSaida(int numeroTitulo) {
    for (int i = 0; i < numRegistros; ++i) {
        if (registros[i].numeroTitulo == numeroTitulo && registros[i].entrou) {
            registros[i].entrou = false;
            cout << "Saida registrada com sucesso." << endl;
            return;
        }
    }
    cout << "Socio de numero " << numeroTitulo << " nao registrado na entrada." << endl;
}

void gerarRelatorioVisitasDia(const Socio socios[], int numSocios) {
    ofstream arquivo("relatorio_visitas.txt");
    if (arquivo.is_open()) {
        string data = "09-12-2023"; // Data estática do relatório
        arquivo << data << endl;
        arquivo << "========" << endl;

        for (int i = 0; i < numRegistros; ++i) {
            int numeroTitulo = registros[i].numeroTitulo;
            bool registrouEntrada = registros[i].entrou;

            bool encontrado = false;

            for (int j = 0; j < numSocios; ++j) {
                if (socios[j].numeroTitulo == numeroTitulo) {
                    arquivo << socios[j].numeroTitulo << " " << socios[j].nomeTitular << " ";

                    if (registrouEntrada) {
                        arquivo << "E ";
                    } else {
                        arquivo << "S ";
                    }

                    arquivo << endl;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                arquivo << "Socio com numero " << numeroTitulo << " nao encontrado." << endl;
            }
        }

        arquivo << "========" << endl;
        cout << "Relatorio gerado com sucesso no arquivo relatorio_visitas.txt" << endl;
        arquivo.close();
    } else {
        cout << "Erro ao criar o arquivo de relatorio_visitas.txt" << endl;
    }
}

int main() {
    Clube clube;

    int opcao;
    do {
        cout << "=================================================" << endl;
        cout << "|Menu:                                          |" << endl;
        cout << "|1. Inserir socio...............................|" << endl;
        cout << "|2. Listar socios...............................|" << endl;
        cout << "|3. Remover socio...............................|" << endl;
        cout << "|4. Carregar socios de arquivo..................|" << endl;
        cout << "|5. Registrar Entrada...........................|" << endl;
        cout << "|6. Registrar Saida.............................|" << endl;
        cout << "|7. Gerar Relatorio de Visitas..................|" << endl;
        cout << "|0. Sair........................................|" << endl;
        cout << "|Escolha uma opcao:                             |" << endl;
        cout << "=================================================" << endl;
        cin >> opcao;

        switch (opcao) {
            case 1: {
                Socio novoSocio;
                cout << "Numero do titulo: ";
                cin >> novoSocio.numeroTitulo;
                cin.ignore(); // Limpa o buffer (geralmente /n, espaços)
                cout << "Nome do titular: ";
                getline(cin, novoSocio.nomeTitular);
                cout << "Nome do dependente 1: ";
                getline(cin, novoSocio.nomeDependente1);
                cout << "Nome do dependente 2: ";
                getline(cin, novoSocio.nomeDependente2);
                cout << "Pagamento em dia? (1 para Sim, 0 para Nao): ";
                cin >> novoSocio.pagamentoMensalidade;
                clube.inserirSocio(novoSocio);
                break;
            }
            case 2:
                clube.listarSocios();
                break;
            case 3: {
                int numeroTitulo;
                cout << "Informe o numero do titulo do socio a ser removido: ";
                cin >> numeroTitulo;
                clube.removerSocio(numeroTitulo);
                break;
            }
            case 4:
                clube.carregarSociosArquivo();
                cout << "Quantidade de socios carregados: " << clube.numSocios << endl;
                clube.listarSocios();
                break;
            case 5: {
                int numeroTitulo;
                cout << "Informe o numero do titulo do socio que entrou: ";
                cin >> numeroTitulo;
                registrarEntrada(clube, numeroTitulo);
                break;
            }
            case 6: {
                int numeroTitulo;
                cout << "Informe o numero do titulo do socio que saiu: ";
                cin >> numeroTitulo;
                registrarSaida(numeroTitulo);
                break;
            }
            case 7: {
                gerarRelatorioVisitasDia(clube.socios, clube.numSocios);
                break;
            }
            case 0:
                cout << "Encerrando o programa." << endl;
                break;
            default:
                cout << "Opcao invalida." << endl;
                break;
        }
    } while (opcao != 0);

    return 0;
}

