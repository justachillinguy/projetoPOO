/**
 * @file main.cpp
 * @brief Ponto de entrada do simulador da Fábrica com controlo de tempo e menu interativo.
 */

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <conio.h>
#include <windows.h>
#include "Fabrica.h"
#include "Relogio.h"

using namespace std;

/**
 * @brief Apresenta o menu principal e lê a opção do utilizador.
 * @return Número inteiro correspondente à opção escolhida.
 */
int MostrarMenu() {
    int op = -1;
    cout << "\n================ MENU DA FABRICA ================\n";
    cout << "1  - Listar estado atual (Listar)\n";
    cout << "2  - Ligar motor por ID (Ligar)\n";
    cout << "3  - Desligar motor por ID (Desligar)\n";
    cout << "4  - Consultar estado de um motor (Get_Estado)\n";
    cout << "5  - Listar motores por Tipo (Listar_Tipo)\n";
    cout << "6  - Manutencao geral (Manutencao)\n";
    cout << "7  - Ranking dos Fracos (Marcas que mais avariam)\n";
    cout << "8  - Ranking dos Mais Trabalhadores\n";
    cout << "9  - Mudar Velocidade da Simulacao\n";
    cout << "10 - Gerar Relatorio XML\n";
    cout << "11 - Gerar Listagem HTML\n";
    cout << "13 - Demo Aviso Fumo (com Video)\n";
    cout << "14 - Demo Aviso Luz (com Video)\n";
    cout << "15 - Demo Aviso Missel (com Video)\n";
    cout << "12 - Retomar Simulacao\n";
    cout << "0  - Fechar Programa\n";
    cout << "=================================================\n";
    cout << "Opcao: ";

    if (!(cin >> op)) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    return op;
}

/**
 * @brief Executa o ciclo do menu enquanto o utilizador não retomar a simulação ou sair.
 * @param fab Referência para o objeto Fabrica.
 * @param relogio Referência para o Relogio da simulação.
 * @return true para continuar a simulação; false para terminar o programa.
 */
bool ExecutaAcoesMenu(Fabrica& fab, Relogio& relogio) {
    int op;
    do {
        op = MostrarMenu();
        if (op == 0) return false;
        if (op == 12) {
            cout << "A retomar simulacao... (Prima 'M' para abrir o menu)\n";
            return true;
        }

        switch (op) {
            case 1:
                fab.Listar();
                break;
            case 2: {
                int id;
                cout << "ID do motor a ligar: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "ID invalido.\n";
                } else {
                    fab.Ligar(id);
                }
                break;
            }
            case 3: {
                int id;
                cout << "ID do motor a desligar: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "ID invalido.\n";
                } else {
                    fab.Desligar(id);
                }
                break;
            }
            case 4: {
                int id;
                cout << "ID do motor: ";
                if (!(cin >> id)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "ID invalido.\n";
                } else if (!fab.ExisteMotor(id)) {
                    cout << "Erro: Motor com ID " << id << " nao existe.\n";
                } else {
                    ESTADO_MOTOR est = fab.Get_Estado(id);
                    string texto = "STOP";
                    if (est == ESTADO_MOTOR::START) texto = "START";
                    else if (est == ESTADO_MOTOR::RUN) texto = "RUN";
                    else if (est == ESTADO_MOTOR::AVARIADO) texto = "AVARIADO";
                    cout << "Estado do motor " << id << ": " << texto << "\n";
                }
                break;
            }
            case 5: {
                string tipo;
                cout << "Tipo (MCOMBUSTAO / MELETRICO / MINDUCAO): ";
                cin >> tipo;
                for (size_t i = 0; i < tipo.length(); i++) {
                    tipo[i] = toupper(tipo[i]);
                }

                if (tipo != "MCOMBUSTAO" && tipo != "MELETRICO" && tipo != "MINDUCAO") {
                    cout << "Erro: Tipo invalido. Escolha MCOMBUSTAO, MELETRICO ou MINDUCAO.\n";
                } else {
                    list<Motor*> *lista = fab.Listar_Tipo(tipo);
                    delete lista;
                }
                break;
            }
            case 6:
                fab.Manutencao();
                break;
            case 7: {
                list<string> *fracos = fab.Ranking_Dos_Fracos();
                if (fracos != NULL) {
                    cout << "\n--- RANKING DOS FRACOS (Mais avarias -> Mais confiaveis) ---\n";
                    for (list<string>::iterator it = fracos->begin(); it != fracos->end(); ++it) {
                        cout << "- " << *it << "\n";
                    }
                    delete fracos;
                }
                break;
            }
            case 8: {
                list<Motor*> *trab = fab.Ranking_Dos_Mais_Trabalhadores();
                if (trab != NULL) {
                    cout << "\n--- RANKING DOS MAIS TRABALHADORES ---\n";
                    for (list<Motor*>::iterator it = trab->begin(); it != trab->end(); ++it) {
                        cout << "ID " << (*it)->GetId() << " (" << (*it)->GetMarca()
                             << ") - " << (*it)->GetTempoTrabalho() << " min | Consumo: "
                             << (*it)->GetConsumoDia() << " kWh\n";
                    }
                    delete trab;
                }
                break;
            }
            case 9: {
                int vel = 0;
                do {
                    cout << "Velocidade atual: " << relogio.GetVelocidade() << "x\n";
                    cout << "Nova velocidade (minimo 1, ex: 600 = 10min/seg): ";
                    if (!(cin >> vel)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        vel = 0;
                        cout << "Valor invalido.\n";
                    }
                } while (vel < 1);
                relogio.MudarVelocidade(vel);
                cout << "Velocidade alterada para " << vel << "x.\n";
                break;
            }
            case 10:
                fab.Relatorio("relatorio_fabrica.xml");
                cout << "Ficheiro relatorio_fabrica.xml gravado.\n";
                break;
            case 11:
                fab.Listagem_HTML("estado_fabrica.html");
                cout << "Ficheiro estado_fabrica.html gravado.\n";
                break;
            case 13: {
                cout << "[DEMO] A testar Aviso de Fumo...\n";
                list<Motor*> lm;
                fab.Aviso_Fumo(lm, "videos/aviso_fumo.mp4");
                break;
            }
            case 14: {
                cout << "[DEMO] A testar Aviso de Luz...\n";
                fab.Aviso_Luz("videos/aviso_luz.mp4");
                break;
            }
            case 15: {
                cout << "[DEMO] A testar Aviso de Missel...\n";
                fab.Aviso_Missel("videos/aviso_missel.mp4", "Estado.txt");
                break;
            }
            default:
                cout << "Opcao invalida.\n";
                break;
        }
    } while (true);
}

int main() {
    srand((unsigned int)time(NULL));

    Admin* gestor = new Admin(1, "Gestor");
    Fabrica fab(gestor);

    // Velocidade 600: 1 segundo real = 600 segundos simulados (10 minutos na fabrica)
    Relogio relogio(600);
    long ultimoPassoSimulado = 0;

    // Procura o XML na pasta atual ou na raiz do projeto
    bool carregou = fab.Load("fabrica.xml");
    if (!carregou) carregou = fab.Load("ExemploParaSimuladorXML.xml");
    if (!carregou) carregou = fab.Load("../../fabrica.xml");
    if (!carregou) carregou = fab.Load("../../ExemploParaSimuladorXML.xml");

    if (!carregou) {
        cout << "Aviso: Ficheiro XML nao encontrado. A carregar configuracao de teste...\n";
        Motor* m1 = new MotorCombustao(101, "Caterpillar", 15.5, 75.0, 95.0, 0.02, 10, 20);
        Motor* m2 = new MotorEletrico(102, "Siemens", 8.2, 65.0, 85.0, 0.01, 30, 40);
        m1->START();
        m2->START();
        fab.Add(m1);
        fab.Add(m2);
        fab.Add(new SensorLuz(201, "Philips", 300.0, 0.01, 15, 15, 100.0));
        fab.Add(new SensorHumidade(202, "Bosch", 45.0, 0.01, 12, 22, 80.0, 15));
        fab.Add(new SensorFumo(203, "Kidde", 5.0, 0.01, 40, 40, 30.0));
        fab.Add(new SensorMissel(204, "Raytheon", 0.0, 0.001, 80, 80));
    } else {
        cout << "Configuracao XML carregada com sucesso!\n";
    }

    cout << "Simulador em execucao! Prima 'M' a qualquer momento para abrir o menu.\n";

    bool aCorrer = true;
    bool avisoParadaMostrado = false;

    while (aCorrer) {
        if (_kbhit()) {
            char tecla = _getch();
            if (tecla == 'm' || tecla == 'M') {
                if (!ExecutaAcoesMenu(fab, relogio)) {
                    aCorrer = false;
                    break;
                }
                ultimoPassoSimulado = relogio.GetSegundosSimulados();
                if (fab.EstaAtiva()) {
                    avisoParadaMostrado = false;
                }
            }
        }

        long segundosAtuais = relogio.GetSegundosSimulados();
        while (segundosAtuais - ultimoPassoSimulado >= 600) {
            ultimoPassoSimulado += 600;

            if (fab.EstaAtiva()) {
                fab.StepSimulacao();
                int h = fab.GetHoraSimulada();
                int m = fab.GetMinutoSimulado();

                cout << "[TICK ";
                if (h < 10) cout << "0";
                cout << h << ":";
                if (m < 10) cout << "0";
                cout << m << " | Vel: " << relogio.GetVelocidade() << "x] Prima 'M' para Menu\n";
            } else {
                if (!avisoParadaMostrado) {
                    cout << "[PARADA] Fabrica encerrada por emergencia. Prima 'M' para Menu (Manutencao reativa a fabrica)\n";
                    avisoParadaMostrado = true;
                }
                ultimoPassoSimulado = segundosAtuais;
                break;
            }
        }

        Sleep(100);
    }

    cout << "Simulacao terminada.\n";
    return 0;
}
