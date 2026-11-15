// Reencontro - robo estacionario de acolhimento / CitizenTech
// IA: arvore treinada em dados sinteticos e exportada em modelo.h.
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "modelo.h"

constexpr int TRIG = 18, ECHO = 19, PIR = 27;
constexpr int AZUL = 25, VERDE = 26, VERMELHO = 14;
constexpr int FALHA = 32; // botao de injecao de falha, INPUT_PULLUP
constexpr bool VERSAO_EVOLUIDA = true; // false: baseline criado nesta entrega
LiquidCrystal_I2C lcd(0x27, 20, 4);
enum Estado { ESPERA, APROXIMACAO, ACOLHIMENTO, MUITO_PERTO, ERRO_SENSOR };
bool viuMovimento = false;
unsigned long ultimoMovimento = 0, ultimaLeitura = 0;
int estadoAtual = -1;

void linha(int n, const char* texto) {
  lcd.setCursor(0, n);
  lcd.print("                    ");
  lcd.setCursor(0, n);
  lcd.print(texto);
}

float medirDistancia() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  unsigned long pulso = pulseIn(ECHO, HIGH, 30000UL);
  return pulso == 0 ? -1.0f : pulso / 58.0f;
}

void apresentar(int estado) {
  digitalWrite(AZUL, estado == ESPERA || estado == APROXIMACAO);
  digitalWrite(VERDE, estado == ACOLHIMENTO);
  digitalWrite(VERMELHO, estado == MUITO_PERTO || estado == ERRO_SENSOR);
  linha(0, "REENCONTRO | MAKER");
  switch (estado) {
    case ESPERA:
      linha(1,"Em espera"); linha(2,"Aproxime e mova-se");
      Serial.println("ESPERA: aguardando aproximacao com movimento recente."); break;
    case APROXIMACAO:
      linha(1,"Movimento detectado"); linha(2,"Aproxime-se do robo");
      Serial.println("APROXIMACAO: movimento distante; aproxime-se para atendimento."); break;
    case ACOLHIMENTO:
      linha(1,"Ola! Posso ajudar?"); linha(2,"Menu serial: 1 2 3");
      Serial.println("ACOLHIMENTO: Bem-vindo ao Reencontro. Menu: 1=buscar; 2=informar; 3=apoio humano.");
      Serial.println("Portal: https://reencontro-site.vercel.app/ (abrir no navegador)"); break;
    case MUITO_PERTO:
      linha(1,"Muito perto!"); linha(2,"Afaste-se um pouco");
      Serial.println("MUITO_PERTO: mantenha espaco para utilizar o ponto de acolhimento."); break;
    case ERRO_SENSOR:
      linha(1,"Falha de leitura"); linha(2,"Procure a equipe");
      Serial.println("ERRO_SENSOR: atendimento automatico suspenso; procure a equipe."); break;
  }
}

void menuSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r' || c == ' ') continue;
    if (estadoAtual != ACOLHIMENTO) {
      Serial.println("Menu indisponivel: aproxime-se e ative o PIR."); continue;
    }
    // Conteudo editorial fixo. Nao e geracao de texto por IA.
    if (c == '1') Serial.println("BUSCA: abra o portal Reencontro e utilize os campos disponiveis. O robo nao confirma identidades.");
    else if (c == '2') Serial.println("INFORMACAO: encaminhe a informacao a equipe responsavel para revisao humana. Nao digite dados pessoais neste simulador.");
    else if (c == '3') Serial.println("APOIO: solicite atendimento a equipe do ponto de acolhimento. Este prototipo nao aciona servicos externos.");
    else Serial.println("Escolha 1, 2 ou 3.");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG, OUTPUT); pinMode(ECHO, INPUT); pinMode(PIR, INPUT);
  pinMode(AZUL, OUTPUT); pinMode(VERDE, OUTPUT); pinMode(VERMELHO, OUTPUT);
  pinMode(FALHA, INPUT_PULLUP);
  Wire.begin(21, 22); lcd.init(); lcd.backlight();
  Serial.println("REENCONTRO - simulacao de robo estacionario de acolhimento");
  Serial.println(VERSAO_EVOLUIDA ? "V2: ultrassom + PIR + arvore de decisao treinada" : "V1: ultrassom + regra simples (baseline desta entrega)");
  Serial.println("IA sensorial local; sem LLM, sem reconhecimento facial e sem consulta automatica ao portal.");
}

void loop() {
  menuSerial();
  unsigned long agora = millis();
  if (agora - ultimaLeitura < 300UL) return;
  ultimaLeitura = agora;
  int pir = digitalRead(PIR);
  if (pir == HIGH) { viuMovimento = true; ultimoMovimento = agora; }
  bool recente = viuMovimento && (agora - ultimoMovimento < 10000UL);
  float distancia = medirDistancia();
  bool falhaInjetada = digitalRead(FALHA) == LOW;
  int estado;
  if (falhaInjetada || distancia < 2 || distancia > 400.5f) estado = ERRO_SENSOR;
  else if (VERSAO_EVOLUIDA) estado = prever(distancia, recente ? 1.0f : 0.0f);
  else estado = distancia <= 150 ? ACOLHIMENTO : ESPERA;
  if (estado != estadoAtual) { estadoAtual = estado; apresentar(estado); }
  char dados[21];
  if (falhaInjetada) snprintf(dados, sizeof(dados), "Falha injetada");
  else snprintf(dados, sizeof(dados), "D:%3.0fcm M:%d PIR:%d", distancia, recente, pir);
  linha(3, dados);
  Serial.print("SENSOR distancia_cm="); Serial.print(distancia,1);
  Serial.print(" pir="); Serial.print(pir);
  Serial.print(" movimento_recente="); Serial.print(recente);
  Serial.print(" falha_injetada="); Serial.print(falhaInjetada);
  Serial.print(" classe="); Serial.println(estado);
}
