/**
 * @file Fabrica.cpp
 * @brief Implementacao dos metodos da classe Fabrica, gestao de dispositivos e simulacao.
 */

#include "Fabrica.h"

#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <vector>


// ==================== CONSTRUTOR E DESTRUTOR ====================

/**
 * @brief Construtor da classe Fabrica.
 * @param utilizadorInicial Utilizador com sessao iniciada (por defeito Admin/Gestor).
 */
Fabrica::Fabrica(User *utilizadorInicial)
    : nome("Fabrica"), horaAbertura(8), horaFecho(16), vizinhancaAviso(10),
      dimensaoX(100), dimensaoY(100), horaSimulada(8), minutoSimulado(0),
      fabricaAtiva(true), userAtual(utilizadorInicial) {
    if (utilizadorInicial != NULL) {
        utilizadores.push_back(utilizadorInicial);
    }
}

/**
 * @brief Destrutor virtual da Fabrica.
 * Liberta explicitamente toda a memoria alocada para motores, sensores e utilizadores (sem memory leaks).
 */
Fabrica::~Fabrica() {
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        delete *itMotor;
    }
    motores.clear();

    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        delete *itSensor;
    }
    sensores.clear();

    for (list<User*>::iterator itUser = utilizadores.begin(); itUser != utilizadores.end(); ++itUser) {
        delete *itUser;
    }
    utilizadores.clear();
}

// ==================== METODOS AUXILIARES PRIVADOS ====================

Motor* Fabrica::ProcurarMotor(int identificador) const {
    for (list<Motor*>::const_iterator it = motores.begin(); it != motores.end(); ++it) {
        if ((*it)->GetId() == identificador) {
            return *it;
        }
    }
    return NULL;
}

Sensor* Fabrica::ProcurarSensor(int identificador) const {
    for (list<Sensor*>::const_iterator it = sensores.begin(); it != sensores.end(); ++it) {
        if ((*it)->GetId() == identificador) {
            return *it;
        }
    }
    return NULL;
}

double Fabrica::CalcularDistancia(int x1, int y1, int x2, int y2) const {
    double diferencaX = x1 - x2;
    double diferencaY = y1 - y2;
    return sqrt(diferencaX * diferencaX + diferencaY * diferencaY);
}

string Fabrica::ExtrairTagXML(const string& textoCompleto, const string& nomeTag) const {
    string tagAbertura = "<" + nomeTag + ">";
    string tagFecho = "</" + nomeTag + ">";
    size_t posicaoInicio = textoCompleto.find(tagAbertura);
    if (posicaoInicio == string::npos) return "";
    posicaoInicio += tagAbertura.length();
    size_t posicaoFim = textoCompleto.find(tagFecho, posicaoInicio);
    if (posicaoFim == string::npos) return "";
    return textoCompleto.substr(posicaoInicio, posicaoFim - posicaoInicio);
}

void Fabrica::ExtrairParDouble(const string& texto, double& primeiroValor, double& segundoValor) const {
    size_t posicaoVirgula = texto.find(',');
    if (posicaoVirgula != string::npos) {
        primeiroValor = atof(texto.substr(0, posicaoVirgula).c_str());
        segundoValor = atof(texto.substr(posicaoVirgula + 1).c_str());
    }
}

void Fabrica::ExtrairParInt(const string& texto, int& primeiroValor, int& segundoValor) const {
    size_t posicaoVirgula = texto.find(',');
    if (posicaoVirgula != string::npos) {
        primeiroValor = atoi(texto.substr(0, posicaoVirgula).c_str());
        segundoValor = atoi(texto.substr(posicaoVirgula + 1).c_str());
    }
}

double Fabrica::NormalizarProbabilidade(double valorProbabilidade) const {
    // Se o valor vier em percentagem (ex.: 10 para 10%), converte para fracao [0.0, 1.0]
    if (valorProbabilidade >= 1.0) {
        return valorProbabilidade / 100.0;
    }
    return valorProbabilidade;
}

bool Fabrica::EstaAberta() const {
    return (horaSimulada >= horaAbertura && horaSimulada < horaFecho);
}

void Fabrica::AtualizarSensores() {
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        Sensor* sensorAtual = *itSensor;
        if (sensorAtual->EstaAvariado()) continue;

        // Sensores de temperatura detetam a temperatura maxima dos motores na sua vizinhanca
        if (sensorAtual->GetTipo() == "TEMPERATURA") {
            double maiorTemperaturaVizinhanca = 20.0;
            for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
                Motor* motorAtual = *itMotor;
                double distancia = CalcularDistancia(sensorAtual->GetX(), sensorAtual->GetY(),
                                                     motorAtual->GetX(), motorAtual->GetY());
                if (distancia <= vizinhancaAviso) {
                    if (motorAtual->GetTemperaturaAtual() > maiorTemperaturaVizinhanca) {
                        maiorTemperaturaVizinhanca = motorAtual->GetTemperaturaAtual();
                    }
                }
            }
            sensorAtual->SetValor(maiorTemperaturaVizinhanca);
        }
    }
}

void Fabrica::SimularAvarias() {
    // Como a probabilidade do XML e horaria e cada passo sao 10 minutos (1/6 de hora), dividimos por 6.0
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motorAtual = *itMotor;
        if (!motorAtual->EstaAvariado() && motorAtual->GetEstado() == ESTADO_MOTOR::RUN) {
            double sorteioAleatorio = (double)rand() / RAND_MAX;
            double probabilidadeAvariaPasso = motorAtual->GetProbabilidadeAvaria() / 6.0;
            if (sorteioAleatorio < probabilidadeAvariaPasso) {
                motorAtual->RegistarAvaria();
                motorAtual->STOP();
                cout << "[ALERTA GESTOR] Motor ID " << motorAtual->GetId()
                     << " (" << motorAtual->GetMarca() << ") AVARIOU!\n";
            }
        }
    }

    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        Sensor* sensorAtual = *itSensor;
        if (!sensorAtual->EstaAvariado()) {
            double sorteioAleatorio = (double)rand() / RAND_MAX;
            double probabilidadeAvariaPasso = sensorAtual->GetProbabilidadeAvaria() / 6.0;
            if (sorteioAleatorio < probabilidadeAvariaPasso) {
                sensorAtual->RegistarAvaria();
                cout << "[ALERTA GESTOR] Sensor ID " << sensorAtual->GetId()
                     << " (" << sensorAtual->GetMarca() << ") AVARIOU!\n";
            }
        }
    }
}

void Fabrica::AvancarRelogio(int minutos) {
    minutoSimulado += minutos;
    while (minutoSimulado >= 60) {
        minutoSimulado -= 60;
        horaSimulada++;
    }
    // Ao virar da meia-noite (novo dia), reinicia o consumo diario de todos os motores
    if (horaSimulada >= 24) {
        horaSimulada %= 24;
        for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
            (*itMotor)->ReiniciarConsumoDia();
        }
        cout << "[SISTEMA] Novo dia simulado: consumo diario dos motores reiniciado a zero.\n";
    }
}

// ==================== CARREGAMENTO XML (Load) ====================

bool Fabrica::Load(const string &caminhoFicheiroXML) {
    ifstream ficheiro(caminhoFicheiroXML.c_str());
    if (!ficheiro.is_open()) {
        return false;
    }

    stringstream bufferTexto;
    bufferTexto << ficheiro.rdbuf();
    string conteudoXML = bufferTexto.str();
    ficheiro.close();

    // 1. Ler o bloco de <DEFINICOES>
    string blocoDefinicoes = ExtrairTagXML(conteudoXML, "DEFINICOES");
    if (blocoDefinicoes.empty()) {
        cout << "XML invalido: seccao <DEFINICOES> nao encontrada.\n";
        return false;
    }

    string nomeEmpresaLido = ExtrairTagXML(blocoDefinicoes, "NOME_EMPRESA");
    if (!nomeEmpresaLido.empty()) nome = nomeEmpresaLido;

    string horaAberturaStr = ExtrairTagXML(blocoDefinicoes, "HORA_INICIO");
    if (!horaAberturaStr.empty()) {
        horaAbertura = atoi(horaAberturaStr.c_str());
        horaSimulada = horaAbertura;
        minutoSimulado = 0;
    }

    string horaFechoStr = ExtrairTagXML(blocoDefinicoes, "HORA_FECHO");
    if (!horaFechoStr.empty()) {
        horaFecho = atoi(horaFechoStr.c_str());
    }

    string raioVizinhancaStr = ExtrairTagXML(blocoDefinicoes, "VIZINHANCA_AVISO");
    if (!raioVizinhancaStr.empty()) {
        vizinhancaAviso = atoi(raioVizinhancaStr.c_str());
    }

    string dimensoesStr = ExtrairTagXML(blocoDefinicoes, "DIMENSAO_FABRICA");
    if (!dimensoesStr.empty()) {
        ExtrairParInt(dimensoesStr, dimensaoX, dimensaoY);
    }

    // Carregar limiares e probabilidades de avaria por tipo de motor
    string tiposMotores[3] = { "MELETRICO", "MCOMBUSTAO", "MINDUCAO" };
    map<string, double> limiarAvisoPorTipo;
    map<string, double> limiarParagemPorTipo;
    map<string, double> probAvariaPorTipo;

    for (int i = 0; i < 3; i++) {
        string tipoMotorNome = tiposMotores[i];
        string blocoTipo = ExtrairTagXML(blocoDefinicoes, tipoMotorNome);

        string intervaloVerde = ExtrairTagXML(blocoTipo, "VERDE");
        string intervaloAmarelo = ExtrairTagXML(blocoTipo, "AMARELO");
        string intervaloVermelho = ExtrairTagXML(blocoTipo, "VERMELHO");
        string probAvariaStr = ExtrairTagXML(blocoTipo, "PROB_AVARIA");

        defVerde[tipoMotorNome] = intervaloVerde;
        defAmarelo[tipoMotorNome] = intervaloAmarelo;
        defVermelho[tipoMotorNome] = intervaloVermelho;
        defProbAvaria[tipoMotorNome] = probAvariaStr;

        double minAmarelo = 60.0, maxAmarelo = 80.0;
        double minVermelho = 81.0, maxVermelho = 200.0;
        ExtrairParDouble(intervaloAmarelo, minAmarelo, maxAmarelo);
        ExtrairParDouble(intervaloVermelho, minVermelho, maxVermelho);

        limiarAvisoPorTipo[tipoMotorNome] = minAmarelo;
        limiarParagemPorTipo[tipoMotorNome] = minVermelho;
        probAvariaPorTipo[tipoMotorNome] = NormalizarProbabilidade(atof(probAvariaStr.c_str()));
    }

    // 2. Ler o bloco de <MOTORES>
    string blocoMotores = ExtrairTagXML(conteudoXML, "MOTORES");
    for (int i = 0; i < 3; i++) {
        string tipoMotorNome = tiposMotores[i];
        string tagAbertura = "<" + tipoMotorNome + ">";
        string tagFecho = "</" + tipoMotorNome + ">";
        size_t posicaoInicio = 0;

        while ((posicaoInicio = blocoMotores.find(tagAbertura, posicaoInicio)) != string::npos) {
            size_t posicaoFim = blocoMotores.find(tagFecho, posicaoInicio);
            if (posicaoFim == string::npos) break;

            string dadosMotor = blocoMotores.substr(posicaoInicio + tagAbertura.length(),
                                                   posicaoFim - (posicaoInicio + tagAbertura.length()));

            int idMotor = atoi(ExtrairTagXML(dadosMotor, "ID").c_str());
            string marcaMotor = ExtrairTagXML(dadosMotor, "MARCA");
            double temperaturaInicial = atof(ExtrairTagXML(dadosMotor, "VALOR_TEMP_ATUAL").c_str());
            string estadoInicial = ExtrairTagXML(dadosMotor, "ESTADO_ATUAL");
            double consumoPorHora = atof(ExtrairTagXML(dadosMotor, "CONSUMO_HORA").c_str());

            int posX = 0, posY = 0;
            ExtrairParInt(ExtrairTagXML(dadosMotor, "POSICAO"), posX, posY);

            if (posX >= 0 && posX <= dimensaoX && posY >= 0 && posY <= dimensaoY && ProcurarMotor(idMotor) == NULL) {
                Motor* novoMotor = NULL;
                double tempAviso = limiarAvisoPorTipo[tipoMotorNome];
                double tempParagem = limiarParagemPorTipo[tipoMotorNome];
                double probAvaria = probAvariaPorTipo[tipoMotorNome];

                if (tipoMotorNome == "MCOMBUSTAO") {
                    novoMotor = new MotorCombustao(idMotor, marcaMotor, consumoPorHora, tempAviso, tempParagem, probAvaria, posX, posY);
                } else if (tipoMotorNome == "MINDUCAO") {
                    novoMotor = new MotorInducao(idMotor, marcaMotor, consumoPorHora, tempAviso, tempParagem, probAvaria, posX, posY);
                } else {
                    novoMotor = new MotorEletrico(idMotor, marcaMotor, consumoPorHora, tempAviso, tempParagem, probAvaria, posX, posY);
                }

                if (novoMotor != NULL) {
                    novoMotor->SetTemperaturaAtual(temperaturaInicial);
                    if (estadoInicial == "RUN") novoMotor->SetEstado(ESTADO_MOTOR::RUN);
                    else if (estadoInicial == "START") novoMotor->SetEstado(ESTADO_MOTOR::START);
                    else if (estadoInicial == "AVARIADO") novoMotor->SetEstado(ESTADO_MOTOR::AVARIADO);
                    else novoMotor->SetEstado(ESTADO_MOTOR::STOP);

                    motores.push_back(novoMotor);
                }
            }
            posicaoInicio = posicaoFim + tagFecho.length();
        }
    }

    // 3. Ler o bloco de <SENSORES>
    string blocoSensores = ExtrairTagXML(conteudoXML, "SENSORES");
    string tagsSensores[6] = { "SHUMIDADE", "SLUZ", "SFUMO", "SMISSIL", "STEMPERATURA", "SFOGO" };

    for (int i = 0; i < 6; i++) {
        string tagSensorNome = tagsSensores[i];
        string tagAbertura = "<" + tagSensorNome + ">";
        string tagFecho = "</" + tagSensorNome + ">";
        size_t posicaoInicio = 0;

        while ((posicaoInicio = blocoSensores.find(tagAbertura, posicaoInicio)) != string::npos) {
            size_t posicaoFim = blocoSensores.find(tagFecho, posicaoInicio);
            if (posicaoFim == string::npos) break;

            string dadosSensor = blocoSensores.substr(posicaoInicio + tagAbertura.length(),
                                                     posicaoFim - (posicaoInicio + tagAbertura.length()));

            int idSensor = atoi(ExtrairTagXML(dadosSensor, "ID").c_str());
            string marcaSensor = ExtrairTagXML(dadosSensor, "MARCA");
            double valorLimiarAviso = atof(ExtrairTagXML(dadosSensor, "VALOR_AVISO").c_str());
            double probabilidadeAvaria = NormalizarProbabilidade(atof(ExtrairTagXML(dadosSensor, "PROB_AVARIA").c_str()));
            double valorLeituraAtual = atof(ExtrairTagXML(dadosSensor, "VALOR_ATUAL").c_str());

            int posX = 0, posY = 0;
            ExtrairParInt(ExtrairTagXML(dadosSensor, "POSICAO"), posX, posY);

            if (posX >= 0 && posX <= dimensaoX && posY >= 0 && posY <= dimensaoY && ProcurarSensor(idSensor) == NULL) {
                Sensor* novoSensor = NULL;
                if (tagSensorNome == "SHUMIDADE") {
                    novoSensor = new SensorHumidade(idSensor, marcaSensor, valorLeituraAtual, probabilidadeAvaria, posX, posY, valorLimiarAviso, vizinhancaAviso);
                } else if (tagSensorNome == "SLUZ") {
                    novoSensor = new SensorLuz(idSensor, marcaSensor, valorLeituraAtual, probabilidadeAvaria, posX, posY, valorLimiarAviso);
                } else if (tagSensorNome == "SFUMO") {
                    novoSensor = new SensorFumo(idSensor, marcaSensor, valorLeituraAtual, probabilidadeAvaria, posX, posY, valorLimiarAviso);
                } else if (tagSensorNome == "SMISSIL") {
                    novoSensor = new SensorMissel(idSensor, marcaSensor, valorLeituraAtual, probabilidadeAvaria, posX, posY);
                } else if (tagSensorNome == "SFOGO") {
                    novoSensor = new SensorFogo(idSensor, marcaSensor, valorLeituraAtual, probabilidadeAvaria, posX, posY, valorLimiarAviso);
                } else {
                    novoSensor = new SensorTemperatura(idSensor, marcaSensor, valorLeituraAtual, probabilidadeAvaria, posX, posY, valorLimiarAviso);
                }

                if (novoSensor != NULL) {
                    sensores.push_back(novoSensor);
                }
            }
            posicaoInicio = posicaoFim + tagFecho.length();
        }
    }

    return true;
}

// ==================== GESTAO DE UTILIZADORES E DISPOSITIVOS ====================

bool Fabrica::Add(User *novoUtilizador) {
    if (novoUtilizador == NULL) return false;
    if (userAtual != NULL && !userAtual->PodeAdicionar()) {
        cout << "Permissao negada: o utilizador atual nao tem permissao para adicionar utilizadores.\n";
        return false;
    }
    for (list<User*>::iterator it = utilizadores.begin(); it != utilizadores.end(); ++it) {
        if ((*it)->GetId() == novoUtilizador->GetId()) return false;
    }
    utilizadores.push_back(novoUtilizador);
    return true;
}

bool Fabrica::Add(Motor *novoMotor) {
    if (novoMotor == NULL) return false;
    if (userAtual != NULL && !userAtual->PodeAdicionar()) {
        cout << "Permissao negada: o utilizador atual nao tem permissao para adicionar motores.\n";
        return false;
    }
    if (novoMotor->GetX() < 0 || novoMotor->GetX() > dimensaoX ||
        novoMotor->GetY() < 0 || novoMotor->GetY() > dimensaoY) return false;
    if (ProcurarMotor(novoMotor->GetId()) != NULL) return false;

    motores.push_back(novoMotor);
    return true;
}

bool Fabrica::Add(Sensor *novoSensor) {
    if (novoSensor == NULL) return false;
    if (userAtual != NULL && !userAtual->PodeAdicionar()) {
        cout << "Permissao negada: o utilizador atual nao tem permissao para adicionar sensores.\n";
        return false;
    }
    if (novoSensor->GetX() < 0 || novoSensor->GetX() > dimensaoX ||
        novoSensor->GetY() < 0 || novoSensor->GetY() > dimensaoY) return false;
    if (ProcurarSensor(novoSensor->GetId()) != NULL) return false;

    sensores.push_back(novoSensor);
    return true;
}

bool Fabrica::Remover(int identificador) {
    if (userAtual != NULL && !userAtual->PodeRemover()) {
        cout << "Permissao negada: apenas utilizadores ADMIN podem remover dispositivos.\n";
        return false;
    }
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        if ((*itMotor)->GetId() == identificador) {
            delete *itMotor;
            motores.erase(itMotor);
            return true;
        }
    }
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        if ((*itSensor)->GetId() == identificador) {
            delete *itSensor;
            sensores.erase(itSensor);
            return true;
        }
    }
    return false;
}

void Fabrica::SetUserAtual(User *novoUtilizadorAtual) {
    userAtual = novoUtilizadorAtual;
}

User* Fabrica::GetUserAtual() const {
    return userAtual;
}

// ==================== LISTAGEM E COMANDOS DE MOTORES ====================

void Fabrica::Listar(ostream &streamSaida) {
    if (userAtual != NULL && !userAtual->PodeListar()) {
        streamSaida << "Permissao negada: o utilizador atual nao pode listar o estado da fabrica.\n";
        return;
    }
    streamSaida << "\n=== ESTADO DA FABRICA: " << nome << " (" << horaAbertura << "h - " << horaFecho
                << "h | Hora Atual: " << horaSimulada << ":";
    if (minutoSimulado < 10) streamSaida << "0";
    streamSaida << minutoSimulado << " | Area: " << dimensaoX << "x" << dimensaoY << ") ===\n";

    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motor = *itMotor;
        string textoEstado = "STOP";
        if (motor->GetEstado() == ESTADO_MOTOR::START) textoEstado = "START";
        else if (motor->GetEstado() == ESTADO_MOTOR::RUN) textoEstado = "RUN";
        else if (motor->GetEstado() == ESTADO_MOTOR::AVARIADO) textoEstado = "AVARIADO";

        streamSaida << "[MOTOR] ID: " << motor->GetId() << " | Tipo: " << motor->GetTipo()
                    << " | Marca: " << motor->GetMarca() << " | Pos: (" << motor->GetX() << "," << motor->GetY() << ")"
                    << " | Estado: " << textoEstado << " | Temp: " << motor->GetTemperaturaAtual() << " C"
                    << " | ConsumoDia: " << motor->GetConsumoDia() << " kWh\n";
    }

    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        Sensor* sensor = *itSensor;
        string textoAviso = sensor->EmAviso() ? "SIM" : "NAO";
        string textoAvariado = sensor->EstaAvariado() ? "SIM" : "NAO";

        streamSaida << "[SENSOR] ID: " << sensor->GetId() << " | Tipo: " << sensor->GetTipo()
                    << " | Marca: " << sensor->GetMarca() << " | Pos: (" << sensor->GetX() << "," << sensor->GetY() << ")"
                    << " | Leitura: " << sensor->GetValor() << " | Aviso: " << textoAviso
                    << " | Avariado: " << textoAvariado << "\n";
    }
}

bool Fabrica::ExisteMotor(int idMotor) const {
    return ProcurarMotor(idMotor) != NULL;
}

bool Fabrica::Ligar(int id_motor) {
    if (userAtual != NULL && !userAtual->PodeRun()) {
        cout << "Permissao negada para ligar motores.\n";
        return false;
    }
    if (!fabricaAtiva || !EstaAberta()) {
        cout << "Nao e possivel ligar o motor: a fabrica encontra-se encerrada ou inativa.\n";
        return false;
    }
    Motor* motorAlvo = ProcurarMotor(id_motor);
    if (motorAlvo == NULL) {
        cout << "Erro: Motor com ID " << id_motor << " nao existe.\n";
        return false;
    }

    motorAlvo->START();
    cout << "Comando START enviado ao motor " << id_motor << ".\n";
    return true;
}

void Fabrica::Desligar(int id_motor) {
    if (userAtual != NULL && !userAtual->PodeRun()) {
        cout << "Permissao negada para desligar motores.\n";
        return;
    }
    Motor* motorAlvo = ProcurarMotor(id_motor);
    if (motorAlvo == NULL) {
        cout << "Erro: Motor com ID " << id_motor << " nao existe.\n";
        return;
    }

    motorAlvo->STOP();
    cout << "Comando STOP enviado ao motor " << id_motor << ".\n";
}

ESTADO_MOTOR Fabrica::Get_Estado(int id_motor) {
    Motor* motorAlvo = ProcurarMotor(id_motor);
    if (motorAlvo == NULL) {
        return ESTADO_MOTOR::STOP;
    }
    if (motorAlvo->EstaAvariado()) return ESTADO_MOTOR::AVARIADO;
    return motorAlvo->GetEstado();
}

list<Motor*> *Fabrica::Listar_Tipo(string Tipo, ostream &streamSaida) {
    list<Motor*> *listaMotoresDoTipo = new list<Motor*>();
    if (userAtual != NULL && !userAtual->PodeListar()) {
        return listaMotoresDoTipo;
    }
    streamSaida << "\n--- MOTORES DO TIPO: " << Tipo << " ---\n";
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motor = *itMotor;
        if (motor->GetTipo() == Tipo) {
            listaMotoresDoTipo->push_back(motor);
            streamSaida << "ID: " << motor->GetId() << " | Marca: " << motor->GetMarca()
                        << " | Temp: " << motor->GetTemperaturaAtual() << " C\n";
        }
    }
    return listaMotoresDoTipo;
}

// ==================== SIMULACAO E MANUTENCAO ====================

void Fabrica::StepSimulacao() {
    if (!fabricaAtiva) {
        cout << "[SISTEMA] Fabrica encerrada devido a alerta critico de emergencia.\n";
        return;
    }

    bool fabricaEstaAberta = EstaAberta();

    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motorAtual = *itMotor;

        // Se a fabrica estiver fechada, os motores devem estar parados e arrefecem
        if (!fabricaEstaAberta) {
            if (motorAtual->GetEstado() == ESTADO_MOTOR::START || motorAtual->GetEstado() == ESTADO_MOTOR::RUN) {
                motorAtual->STOP();
            }
            motorAtual->Arrefecer(2.0);
            continue;
        }

        // Se a fabrica esta aberta:
        if (motorAtual->GetEstado() == ESTADO_MOTOR::STOP) {
            motorAtual->Arrefecer(1.0);
        } else if (motorAtual->GetEstado() == ESTADO_MOTOR::START || motorAtual->GetEstado() == ESTADO_MOTOR::RUN) {
            CLASSE_TEMPERATURA classeAntes = motorAtual->ESTOU_QUENTE();
            motorAtual->RUN(); // Aquece 2 graus e acumula consumo e minutos de trabalho
            CLASSE_TEMPERATURA classeDepois = motorAtual->ESTOU_QUENTE();

            // Notifica o gestor quando transita para AMARELO ou VERMELHO (conforme o PDF)
            if (classeDepois == AMARELO && classeAntes != AMARELO) {
                cout << "[AVISO GESTOR] Motor ID " << motorAtual->GetId() << " entrou na zona AMARELA!\n";
            } else if (classeDepois == VERMELHO) {
                cout << "[ALERTA GESTOR] Motor ID " << motorAtual->GetId() << " atingiu zona VERMELHA -> Paragem de seguranca (STOP)!\n";
            }
        }
    }

    AtualizarSensores();
    SimularAvarias();

    list<Motor*> motoresDesligados;
    Aviso_Humidade(motoresDesligados);
    Aviso_Fumo(motoresDesligados, "aviso_fumo.mp4");
    Aviso_Luz("aviso_luz.mp4");
    Aviso_Missel("aviso_missel.mp4", "Estado.txt");

    AvancarRelogio(10);
}

bool Fabrica::Manutencao() {
    if (userAtual != NULL && !userAtual->PodeManutencao()) {
        cout << "Permissao negada: utilizador atual nao pode efetuar manutencao.\n";
        return false;
    }

    // Repoe temperatura de cada motor aleatoriamente dentro da zona VERDE do XML (conforme o PDF)
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motor = *itMotor;
        motor->Reparar();
        motor->STOP();

        string intervaloVerdeTexto = defVerde[motor->GetTipo()];
        double minVerde = 0.0, maxVerde = motor->GetTemperaturaAviso() - 1.0;
        if (!intervaloVerdeTexto.empty()) {
            ExtrairParDouble(intervaloVerdeTexto, minVerde, maxVerde);
        }
        double amplitude = maxVerde - minVerde;
        if (amplitude <= 0) amplitude = 10.0;

        double temperaturaSorteada = minVerde + ((rand() % 10001) / 10000.0) * amplitude;
        motor->SetTemperaturaAtual(temperaturaSorteada);
    }

    // Repara todos os sensores avariados
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        (*itSensor)->Reparar();
    }

    fabricaAtiva = true;
    cout << "[MANUTENCAO] Manutencao concluida: todos os motores e sensores reparados e repostos.\n";
    return true;
}

list<string> *Fabrica::Ranking_Dos_Fracos() {
    if (userAtual != NULL && !userAtual->PodeListar()) {
        cout << "Permissao negada: o utilizador atual nao pode consultar rankings.\n";
        return NULL;
    }

    // Acumula o total de avarias por marca entre motores e sensores
    map<string, int> totalAvariasPorMarca;
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        totalAvariasPorMarca[(*itMotor)->GetMarca()] += (*itMotor)->GetTotalAvarias();
    }
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        totalAvariasPorMarca[(*itSensor)->GetMarca()] += (*itSensor)->GetTotalAvarias();
    }

    // Transfere para vetor para ordenar por ordem decrescente de avarias (mais avarias -> mais confiavel)
    vector<pair<string, int> > marcasOrdenadas(totalAvariasPorMarca.begin(), totalAvariasPorMarca.end());
    for (size_t i = 0; i < marcasOrdenadas.size(); i++) {
        for (size_t j = i + 1; j < marcasOrdenadas.size(); j++) {
            if (marcasOrdenadas[j].second > marcasOrdenadas[i].second) {
                pair<string, int> temporario = marcasOrdenadas[i];
                marcasOrdenadas[i] = marcasOrdenadas[j];
                marcasOrdenadas[j] = temporario;
            }
        }
    }

    list<string> *listaRanking = new list<string>();
    for (size_t i = 0; i < marcasOrdenadas.size(); i++) {
        listaRanking->push_back(marcasOrdenadas[i].first);
    }
    return listaRanking;
}

list<Motor*> *Fabrica::Ranking_Dos_Mais_Trabalhadores() {
    if (userAtual != NULL && !userAtual->PodeListar()) {
        cout << "Permissao negada: o utilizador atual nao pode consultar rankings.\n";
        return NULL;
    }

    list<Motor*> *rankingOrdenado = new list<Motor*>();
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motorAtual = *itMotor;
        list<Motor*>::iterator posicao = rankingOrdenado->begin();

        // Insercao ordenada decrescente por tempo de trabalho acumulado
        while (posicao != rankingOrdenado->end() &&
               (*posicao)->GetTempoTrabalho() >= motorAtual->GetTempoTrabalho()) {
            ++posicao;
        }
        rankingOrdenado->insert(posicao, motorAtual);
    }
    return rankingOrdenado;
}

// ==================== RELATORIOS E AVISOS ====================

void Fabrica::Relatorio(string caminhoFicheiroXML) {
    ofstream ficheiroXML(caminhoFicheiroXML.c_str());
    if (!ficheiroXML.is_open()) {
        cout << "Erro: Nao foi possivel gravar o ficheiro " << caminhoFicheiroXML << ".\n";
        return;
    }

    ficheiroXML << "<DADOS>\n";
    ficheiroXML << "<DEFINICOES>\n";
    ficheiroXML << "<NOME_EMPRESA>" << nome << "</NOME_EMPRESA>\n";
    ficheiroXML << "<HORA_INICIO>" << horaAbertura << "</HORA_INICIO>\n";
    ficheiroXML << "<HORA_FECHO>" << horaFecho << "</HORA_FECHO>\n";
    ficheiroXML << "<VIZINHANCA_AVISO>" << vizinhancaAviso << "</VIZINHANCA_AVISO>\n";
    ficheiroXML << "<DIMENSAO_FABRICA>" << dimensaoX << "," << dimensaoY << "</DIMENSAO_FABRICA>\n";

    string tiposMotores[3] = { "MELETRICO", "MCOMBUSTAO", "MINDUCAO" };
    for (int i = 0; i < 3; i++) {
        string tipoNome = tiposMotores[i];
        if (!defVerde[tipoNome].empty()) {
            ficheiroXML << "<" << tipoNome << ">\n";
            ficheiroXML << "<VERDE>" << defVerde[tipoNome] << "</VERDE>\n";
            ficheiroXML << "<AMARELO>" << defAmarelo[tipoNome] << "</AMARELO>\n";
            ficheiroXML << "<VERMELHO>" << defVermelho[tipoNome] << "</VERMELHO>\n";
            ficheiroXML << "<PROB_AVARIA>" << defProbAvaria[tipoNome] << "</PROB_AVARIA>\n";
            ficheiroXML << "</" << tipoNome << ">\n";
        }
    }
    ficheiroXML << "</DEFINICOES>\n";

    ficheiroXML << "<MOTORES>\n";
    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motor = *itMotor;
        string textoEstado = "STOP";
        if (motor->GetEstado() == ESTADO_MOTOR::START) textoEstado = "START";
        else if (motor->GetEstado() == ESTADO_MOTOR::RUN) textoEstado = "RUN";
        else if (motor->GetEstado() == ESTADO_MOTOR::AVARIADO) textoEstado = "AVARIADO";

        string textoCor = "VERDE";
        CLASSE_TEMPERATURA classe = motor->ESTOU_QUENTE();
        if (classe == AMARELO) textoCor = "AMARELO";
        else if (classe == VERMELHO) textoCor = "VERMELHO";

        ficheiroXML << "<" << motor->GetTipo() << ">\n";
        ficheiroXML << "<ID>" << motor->GetId() << "</ID>\n";
        ficheiroXML << "<MARCA>" << motor->GetMarca() << "</MARCA>\n";
        ficheiroXML << "<VALOR_TEMP_ATUAL>" << motor->GetTemperaturaAtual() << "</VALOR_TEMP_ATUAL>\n";
        ficheiroXML << "<ESTADO_ATUAL>" << textoEstado << "</ESTADO_ATUAL>\n";
        ficheiroXML << "<ESTADO_COR>" << textoCor << "</ESTADO_COR>\n";
        ficheiroXML << "<CONSUMO_HORA>" << motor->GetConsumoHora() << "</CONSUMO_HORA>\n";
        ficheiroXML << "<CONSUMO_DIA>" << motor->GetConsumoDia() << "</CONSUMO_DIA>\n";
        ficheiroXML << "<TEMPO_TRABALHO>" << motor->GetTempoTrabalho() << "</TEMPO_TRABALHO>\n";
        ficheiroXML << "<POSICAO>" << motor->GetX() << "," << motor->GetY() << "</POSICAO>\n";
        ficheiroXML << "</" << motor->GetTipo() << ">\n";
    }
    ficheiroXML << "</MOTORES>\n";

    ficheiroXML << "<SENSORES>\n";
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        Sensor* sensor = *itSensor;
        string tagSensor = "S" + sensor->GetTipo();
        double probPercentagem = sensor->GetProbabilidadeAvaria() * 100.0;

        ficheiroXML << "<" << tagSensor << ">\n";
        ficheiroXML << "<ID>" << sensor->GetId() << "</ID>\n";
        ficheiroXML << "<MARCA>" << sensor->GetMarca() << "</MARCA>\n";
        ficheiroXML << "<VALOR_AVISO>" << sensor->GetLimiar() << "</VALOR_AVISO>\n";
        ficheiroXML << "<POSICAO>" << sensor->GetX() << "," << sensor->GetY() << "</POSICAO>\n";
        ficheiroXML << "<PROB_AVARIA>" << probPercentagem << "</PROB_AVARIA>\n";
        ficheiroXML << "<VALOR_ATUAL>" << sensor->GetValor();
        if (tagSensor == "SHUMIDADE") ficheiroXML << "%";
        ficheiroXML << "</VALOR_ATUAL>\n";
        ficheiroXML << "</" << tagSensor << ">\n";
    }
    ficheiroXML << "</SENSORES>\n";
    ficheiroXML << "</DADOS>\n";
    ficheiroXML.close();
}

void Fabrica::Listagem_HTML(string caminhoFicheiroHTML) {
    ofstream ficheiroHTML(caminhoFicheiroHTML.c_str());
    if (!ficheiroHTML.is_open()) return;

    ficheiroHTML << "<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\"><title>" << nome << "</title></head><body>\n";
    ficheiroHTML << "<h1>Estado Atual da Fabrica: " << nome << "</h1>\n";
    ficheiroHTML << "<table border=\"1\" cellpadding=\"6\">\n";
    ficheiroHTML << "<tr><th>Categoria</th><th>ID</th><th>Tipo</th><th>Marca</th><th>Posicao</th><th>Detalhes</th></tr>\n";

    for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
        Motor* motor = *itMotor;
        ficheiroHTML << "<tr><td>Motor</td><td>" << motor->GetId() << "</td><td>" << motor->GetTipo()
                     << "</td><td>" << motor->GetMarca() << "</td><td>(" << motor->GetX() << "," << motor->GetY() << ")</td>"
                     << "<td>Temp: " << motor->GetTemperaturaAtual() << " C | Consumo Dia: " << motor->GetConsumoDia() << " kWh</td></tr>\n";
    }
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        Sensor* sensor = *itSensor;
        string textoAviso = sensor->EmAviso() ? "SIM" : "NAO";
        ficheiroHTML << "<tr><td>Sensor</td><td>" << sensor->GetId() << "</td><td>" << sensor->GetTipo()
                     << "</td><td>" << sensor->GetMarca() << "</td><td>(" << sensor->GetX() << "," << sensor->GetY() << ")</td>"
                     << "<td>Leitura: " << sensor->GetValor() << " | Em Aviso: " << textoAviso << "</td></tr>\n";
    }
    ficheiroHTML << "</table>\n</body></html>\n";
    ficheiroHTML.close();
}

int Fabrica::Aviso_Humidade(list<Motor*> &lm) {
    lm.clear();
    int totalMotoresDesligados = 0;

    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        Sensor* sensor = *itSensor;
        if (sensor->GetTipo() == "HUMIDADE" && sensor->EmAviso()) {
            int raioAlvo = sensor->GetRaioVizinhanca();
            if (raioAlvo <= 0) raioAlvo = vizinhancaAviso;

            for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
                Motor* motor = *itMotor;
                double distancia = CalcularDistancia(sensor->GetX(), sensor->GetY(), motor->GetX(), motor->GetY());
                if (distancia <= raioAlvo) {
                    if (motor->GetEstado() != ESTADO_MOTOR::STOP) {
                        motor->STOP();
                        lm.push_back(motor);
                        totalMotoresDesligados++;
                    }
                }
            }
        }
    }
    if (totalMotoresDesligados > 0) {
        cout << "[AVISO GESTOR] Humidade elevada! Motores desligados na vizinhanca: " << totalMotoresDesligados << "\n";
    }
    return totalMotoresDesligados;
}

int Fabrica::Aviso_Fumo(list<Motor*> &lm, string fich_video) {
    lm.clear();
    int totalMotoresDesligados = 0;
    bool existeFumoOuFogo = false;

    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        if (((*itSensor)->GetTipo() == "FUMO" || (*itSensor)->GetTipo() == "FOGO") && (*itSensor)->EmAviso()) {
            existeFumoOuFogo = true;
            break;
        }
    }

    if (existeFumoOuFogo) {
        for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
            Motor* motor = *itMotor;
            if (motor->GetEstado() != ESTADO_MOTOR::STOP) {
                motor->STOP();
                lm.push_back(motor);
                totalMotoresDesligados++;
            }
        }
        cout << "[AVISO GESTOR] Fumo/Fogo detetado! Motores desligados: " << totalMotoresDesligados
             << " | Video de saidas invocado: " << fich_video << "\n";
    }
    return totalMotoresDesligados;
}

int Fabrica::Aviso_Luz(string fich_video) {
    int totalSensoresLuzEmAviso = 0;
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        if ((*itSensor)->GetTipo() == "LUZ" && (*itSensor)->EmAviso()) {
            totalSensoresLuzEmAviso++;
        }
    }
    if (totalSensoresLuzEmAviso > 0) {
        cout << "[AVISO GESTOR] Luminosidade baixa! Video de bom regresso a casa invocado: "
             << fich_video << "\n";
    }
    return totalSensoresLuzEmAviso;
}

void Fabrica::Aviso_Missel(string fvideo, string festado) {
    bool alertaMisselAtivo = false;
    for (list<Sensor*>::iterator itSensor = sensores.begin(); itSensor != sensores.end(); ++itSensor) {
        if ((*itSensor)->GetTipo() == "MISSEL" && (*itSensor)->EmAviso()) {
            alertaMisselAtivo = true;
            break;
        }
    }

    if (alertaMisselAtivo) {
        ofstream ficheiroEstado(festado.c_str());
        if (ficheiroEstado.is_open()) {
            Listar(ficheiroEstado);
            ficheiroEstado.close();
        }
        for (list<Motor*>::iterator itMotor = motores.begin(); itMotor != motores.end(); ++itMotor) {
            (*itMotor)->STOP();
        }
        fabricaAtiva = false;
        cout << "[ALERTA CRITICO] Missel detetado! Fabrica encerrada, estado guardado em: "
             << festado << " | Video de abrigos invocado: " << fvideo << "\n";
    }
}
