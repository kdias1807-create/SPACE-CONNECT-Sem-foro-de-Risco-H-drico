/*
 * SPACE CONNECT — Semáforo de Risco Hídrico
 * Edge Computing & Computer Systems — FIAP Global Solution
 *
 * Integrantes:
 *   Kaue Fernando Jaques Lopes     — RM 571353
 *   Matheus Martins Santos         — RM 570884
 *   Leonardo Gonçalves Cardoso     — RM 569475
 *   Kaio Nincao Maia Dias          — RM 568834
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ── Pinos ─────────────────────────────────────────────────────────────────
#define RAIN_PIN      A0
#define DHT_PIN       2
#define DHT_TYPE      DHT22
#define LED_VERDE     3
#define LED_AMARELO   4
#define LED_LARANJA   5
#define LED_VERMELHO  6
#define BTN_NEXT      7

// ── Limiares ──────────────────────────────────────────────────────────────
#define LIMIAR_ATENCAO  750
#define LIMIAR_ALERTA   500
#define LIMIAR_CRITICO  250

// ── Objetos ───────────────────────────────────────────────────────────────
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);

// ── Estado anterior (para só redesenhar quando mudar) ─────────────────────
int   nivelAnterior = -1;
int   pctAnterior   = -1;
int   telaAnterior  = -1;
int   tempAnterior  = -999;
int   umidAnterior  = -999;
int   telaAtual     = 0;
bool  blinkState    = false;

unsigned long ultimoBlink = 0;
unsigned long ultimoLog   = 0;

// ── Textos por nível ──────────────────────────────────────────────────────
const char* nomeNivel[4] = { "NORMAL  ", "ATENCAO ", "ALERTA  ", "CRITICO!" };
const char* acao1[4]     = { "Monitorar       ", "Verificar setores", "Acionar equipes ", "EVACUAR zonas   " };
const char* acao2[4]     = { "rotineiramente  ", "vulneraveis      ", "de resposta!    ", "de risco AGORA! " };

// ── Funções ───────────────────────────────────────────────────────────────
int calcNivel(int leitura) {
  if (leitura > LIMIAR_ATENCAO) return 0;
  if (leitura > LIMIAR_ALERTA)  return 1;
  if (leitura > LIMIAR_CRITICO) return 2;
  return 3;
}

void atualizaLEDs(int nivel) {
  digitalWrite(LED_VERDE,   nivel >= 0 ? HIGH : LOW);
  digitalWrite(LED_AMARELO, nivel >= 1 ? HIGH : LOW);
  digitalWrite(LED_LARANJA, nivel >= 2 ? HIGH : LOW);
  digitalWrite(LED_VERMELHO, nivel == 3 ? (blinkState ? HIGH : LOW) : LOW);
}

// Escreve no LCD sem usar clear() — sobrescreve posição a posição
void lcdPrint(int col, int row, String texto, int largura) {
  lcd.setCursor(col, row);
  // Preenche com espaços até a largura para apagar texto anterior
  String linha = texto;
  while ((int)linha.length() < largura) linha += " ";
  lcd.print(linha.substring(0, largura));
}

void atualizaLCD(int nivel, int pct, int temp, int umid, bool dhtOk) {
  // Só redesenha se algo mudou
  bool mudou = (nivel    != nivelAnterior) ||
               (pct      != pctAnterior)   ||
               (telaAtual != telaAnterior) ||
               (temp     != tempAnterior)  ||
               (umid     != umidAnterior);

  if (!mudou) return;

  // Salva estado atual
  nivelAnterior = nivel;
  pctAnterior   = pct;
  telaAnterior  = telaAtual;
  tempAnterior  = temp;
  umidAnterior  = umid;

  switch (telaAtual) {

    case 0: // Chuva + nível
      lcdPrint(0, 0, "Chuva: " + String(pct) + "%", 16);
      lcdPrint(0, 1, "Nivel: " + String(nomeNivel[nivel]), 16);
      break;

    case 1: // Temperatura + umidade
      if (dhtOk) {
        lcdPrint(0, 0, "Temp:  " + String(temp) + "C", 16);
        lcdPrint(0, 1, "Umid:  " + String(umid) + "%", 16);
      } else {
        lcdPrint(0, 0, "Temp:  --C", 16);
        lcdPrint(0, 1, "Umid:  --%", 16);
      }
      break;

    case 2: // Ação recomendada
      lcdPrint(0, 0, String(acao1[nivel]), 16);
      lcdPrint(0, 1, String(acao2[nivel]), 16);
      break;
  }
}

// ── Setup ─────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  pinMode(LED_VERDE,    OUTPUT);
  pinMode(LED_AMARELO,  OUTPUT);
  pinMode(LED_LARANJA,  OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BTN_NEXT,     INPUT_PULLUP);

  dht.begin();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("SPACE CONNECT   ");
  lcd.setCursor(0, 1);
  lcd.print("Iniciando...    ");
  delay(2500);
  lcd.clear();

  Serial.println("============================================");
  Serial.println(" SPACE CONNECT - Semaforo de Risco Hidrico");
  Serial.println("============================================");
}

// ── Loop ──────────────────────────────────────────────────────────────────
void loop() {
  // Leituras
  int   leitura = analogRead(RAIN_PIN);
  int   nivel   = calcNivel(leitura);
  int   pct     = map(leitura, 0, 1023, 100, 0); // 0=seco→0%, 1023=molhado→100%
  float tempF   = dht.readTemperature();
  float umidF   = dht.readHumidity();
  bool  dhtOk   = !isnan(tempF) && !isnan(umidF);
  int   temp    = dhtOk ? (int)tempF : -999;
  int   umid    = dhtOk ? (int)umidF : -999;

  // Botão NEXT com debounce
  if (digitalRead(BTN_NEXT) == LOW) {
    telaAtual = (telaAtual + 1) % 3;
    nivelAnterior = -1; // força redesenho da nova tela
    delay(300);
  }

  // Blink LED vermelho
  if (millis() - ultimoBlink > 400) {
    blinkState  = !blinkState;
    ultimoBlink = millis();
  }

  // Atualiza LEDs e LCD
  atualizaLEDs(nivel);
  atualizaLCD(nivel, pct, temp, umid, dhtOk);

  // Log serial a cada 2s
  if (millis() - ultimoLog > 2000) {
    ultimoLog = millis();
    Serial.print("Chuva: "); Serial.print(pct);
    Serial.print("% | Nivel: "); Serial.print(nomeNivel[nivel]);
    if (dhtOk) {
      Serial.print(" | Temp: "); Serial.print(tempF, 1);
      Serial.print("C | Umid: "); Serial.print(umidF, 1); Serial.print("%");
    }
    Serial.println();
  }

  delay(100);
}
