/**
 * @file main.cpp
 * @brief Ponto de entrada do simulador da Fabrica com controlo de tempo e menu interativo.
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
 * @brief Apresenta o menu principal e devolve a opcao escolhida.
 */
int MostrarMenuPrincipal() {
    int opcao = -1;
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
    cout << "13 - Testar Aviso de Fumo (Aviso_Fumo)\n";
    cout << "14 - Testar Aviso de Luz (Aviso_Luz)\n";
    cout << "15 - Demo Aviso Missel (com Video)\n";
    cout << "12 - Retomar Simulacao\n";
    cout << "0  - Fechar Programa\n";
    cout << "=================================================\n";
    cout << "Opcao: ";

    if (!(cin >> opcao)) {
        cin.clear();
        cin.ignore(10000, '\n');
        return -1;
    }
    return opcao;
}

/**
 * @brief Executa as acoes correspondentes a opcao escolhida pelo utilizador.
 * @param fabrica Referencia para o objeto Fabrica.
 * @param relogio Referencia para o Relogio da simulacao.
 * @return true para continuar a simulacao; false para sair do programa.
 */
bool ExecutarAcaoEscolhida(Fabrica& fabrica, Relogio& relogio) {
    int opcao;
    do {
        opcao = MostrarMenuPrincipal();
        if (opcao == 0) return false;
        if (opcao == 12) {
            cout << "A retomar simulacao... (Prima 'M' para abrir o menu)\n";
            return true;
        }

        switch (opcao) {
            case 1:
                fabrica.Listar();
                break;

            case 2: {
                int idMotor;
                cout << "ID do motor a ligar: ";
                if (!(cin >> idMotor)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "ID invalido.\n";
                } else {
                    fabrica.Ligar(idMotor);
                }
                break;
            }

            case 3: {
                int idMotor;
                cout << "ID do motor a desligar: ";
                if (!(cin >> idMotor)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "ID invalido.\n";
                } else {
                    fabrica.Desligar(idMotor);
                }
                break;
            }

            case 4: {
                int idMotor;
                cout << "ID do motor: ";
                if (!(cin >> idMotor)) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "ID invalido.\n";
                } else if (!fabrica.ExisteMotor(idMotor)) {
                    cout << "Erro: Motor com ID " << idMotor << " nao existe.\n";
                } else {
                    ESTADO_MOTOR estado = fabrica.Get_Estado(idMotor);
                    string textoEstado = "STOP";
                    if (estado == ESTADO_MOTOR::START) textoEstado = "START";
                    else if (estado == ESTADO_MOTOR::RUN) textoEstado = "RUN";
                    else if (estado == ESTADO_MOTOR::AVARIADO) textoEstado = "AVARIADO";
                    cout << "Estado do motor " << idMotor << ": " << textoEstado << "\n";
                }
                break;
            }

            case 5: {
                string tipoMotorNome;
                cout << "Tipo de motor (MCOMBUSTAO / MELETRICO / MINDUCAO): ";
                cin >> tipoMotorNome;
                for (size_t i = 0; i < tipoMotorNome.length(); i++) {
                    tipoMotorNome[i] = static_cast<char>(toupper(static_cast<unsigned char>(tipoMotorNome[i])));
                }

                if (tipoMotorNome != "MCOMBUSTAO" && tipoMotorNome != "MELETRICO" && tipoMotorNome != "MINDUCAO") {
                    cout << "Erro: Tipo invalido. Escolha MCOMBUSTAO, MELETRICO ou MINDUCAO.\n";
                } else {
                    list<Motor*> *listaResultado = fabrica.Listar_Tipo(tipoMotorNome);
                    delete listaResultado;
                }
                break;
            }

            case 6:
                fabrica.Manutencao();
                break;

            case 7: {
                list<string> *rankingMarcas = fabrica.Ranking_Dos_Fracos();
                if (rankingMarcas != NULL) {
                    cout << "\n--- RANKING DOS FRACOS (Mais avarias -> Mais confiaveis) ---\n";
                    for (list<string>::iterator it = rankingMarcas->begin(); it != rankingMarcas->end(); ++it) {
                        cout << "- " << *it << "\n";
                    }
                    delete rankingMarcas;
                }
                break;
            }

            case 8: {
                list<Motor*> *rankingTrabalhadores = fabrica.Ranking_Dos_Mais_Trabalhadores();
                if (rankingTrabalhadores != NULL) {
                    cout << "\n--- RANKING DOS MAIS TRABALHADORES ---\n";
                    for (list<Motor*>::iterator it = rankingTrabalhadores->begin(); it != rankingTrabalhadores->end(); ++it) {
                        cout << "ID " << (*it)->GetId() << " (" << (*it)->GetMarca()
                             << ") - " << (*it)->GetTempoTrabalho() << " min | Consumo: "
                             << (*it)->GetConsumoDia() << " kWh\n";
                    }
                    delete rankingTrabalhadores;
                }
                break;
            }

            case 9: {
                int novaVelocidade = 0;
                do {
                    cout << "Velocidade atual: " << relogio.GetVelocidade() << "x\n";
                    cout << "Nova velocidade (minimo 1, ex.: 600 = 10 min simulados por segundo real): ";
                    if (!(cin >> novaVelocidade)) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        novaVelocidade = 0;
                        cout << "Valor invalido.\n";
                    }
                } while (novaVelocidade < 1);
                relogio.MudarVelocidade(novaVelocidade);
                cout << "Velocidade alterada para " << novaVelocidade << "x.\n";
                break;
            }

            case 10:
                fabrica.Relatorio("relatorio_fabrica.xml");
                cout << "Ficheiro relatorio_fabrica.xml gravado com sucesso.\n";
                break;

            case 11:
                fabrica.Listagem_HTML("estado_fabrica.html");
                cout << "Ficheiro estado_fabrica.html gravado com sucesso.\n";
                break;

            case 13: {
                cout << "[TESTE] A testar Aviso de Fumo...\n";
                list<Motor*> motoresDesligados;
                fabrica.Aviso_Fumo(motoresDesligados, "aviso_fumo.mp4");
                break;
            }

            case 14: {
                cout << "[TESTE] A testar Aviso de Luz...\n";
                fabrica.Aviso_Luz("aviso_luz.mp4");
                break;
            }

            case 15: {
                cout << "[TESTE] A testar Aviso de Missel...\n";
                fabrica.Aviso_Missel("aviso_missel.mp4", "Estado.txt");
                break;
            }

            default:
                cout << "Opcao invalida. Tente novamente.\n";
                break;
        }
    } while (true);
}

int main() {
    srand((unsigned int)time(NULL));

    Admin* gestorFabrica = new Admin(1, "Gestor");
    Fabrica fabrica(gestorFabrica);

    Relogio relogioSimulacao(600);
    long segundosUltimoPassoSimulado = 0;

    bool carregouConfiguracao = fabrica.Load("fabrica.xml");
    if (!carregouConfiguracao) carregouConfiguracao = fabrica.Load("ExemploParaSimuladorXML.xml");
    if (!carregouConfiguracao) carregouConfiguracao = fabrica.Load("../../fabrica.xml");

    if (!carregouConfiguracao) {
        cout << "Aviso: fabrica.xml nao encontrado. A criar configuracao de demonstracao...\n";
        Motor* m1 = new MotorCombustao(101, "Caterpillar", 15.5, 75.0, 95.0, 0.02, 10, 20);
        Motor* m2 = new MotorEletrico(102, "Siemens", 8.2, 65.0, 85.0, 0.01, 30, 40);
        m1->START();
        m2->START();
        fabrica.Add(m1);
        fabrica.Add(m2);
        fabrica.Add(new SensorLuz(201, "Philips", 300.0, 0.01, 15, 15, 100.0));
        fabrica.Add(new SensorHumidade(202, "Bosch", 45.0, 0.01, 12, 22, 80.0, 15));
        fabrica.Add(new SensorFumo(203, "Kidde", 5.0, 0.01, 40, 40, 30.0));
        fabrica.Add(new SensorMissel(204, "Raytheon", 0.0, 0.001, 80, 80));
    } else {
        cout << "Configuracao XML carregada com sucesso!\n";
    }

    cout << "Simulador em execucao! Prima 'M' a qualquer momento para abrir o menu.\n";

    bool simuladorAtivo = true;
    bool avisoParadaJaMostrado = false;

    while (simuladorAtivo) {
        if (_kbhit()) {
            int teclaPressionada = _getch();
            if (teclaPressionada == 'm' || teclaPressionada == 'M') {
                if (!ExecutarAcaoEscolhida(fabrica, relogioSimulacao)) {
                    simuladorAtivo = false;
                    break;
                }
                segundosUltimoPassoSimulado = relogioSimulacao.GetSegundosSimulados();
                if (fabrica.EstaAtiva()) {
                    avisoParadaJaMostrado = false;
                }
            }
        }

        long segundosSimuladosAtuais = relogioSimulacao.GetSegundosSimulados();
        while (segundosSimuladosAtuais - segundosUltimoPassoSimulado >= 600) {
            segundosUltimoPassoSimulado += 600;

            if (fabrica.EstaAtiva()) {
                fabrica.StepSimulacao();
                int hora = fabrica.GetHoraSimulada();
                int minuto = fabrica.GetMinutoSimulado();

                cout << "[TICK ";
                if (hora < 10) cout << "0";
                cout << hora << ":";
                if (minuto < 10) cout << "0";
                cout << minuto << " | Vel: " << relogioSimulacao.GetVelocidade()
                     << "x] Prima 'M' para abrir o Menu\n";
            } else {
                if (!avisoParadaJaMostrado) {
                    cout << "[PARADA] Fabrica encerrada por emergencia. Prima 'M' para Menu (Opcao 6 Manutencao reativa a fabrica)\n";
                    avisoParadaJaMostrado = true;
                }
                segundosUltimoPassoSimulado = segundosSimuladosAtuais;
                break;
            }
        }

        Sleep(100);
    }

    cout << "Simulacao terminada.\n";
    return 0;
}
