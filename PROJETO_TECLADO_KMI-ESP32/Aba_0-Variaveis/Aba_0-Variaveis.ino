/* 
PROJETO PARA A TEMPORADA DE MÚSICA DA MECATRÔNICA JOVEM
*********TECLADO MUSICAL NO KMI ESP32***********
-Criador: Nicolas Costa;
-Data: 23/04/2026;
************************************************

-> Agradecimentos ao Prof.Mauro Miyashiro por me presentear o KMI-ESP32! <-

************************************************
COMPONENTES UTILIZADOS:

- 1 x Protoboard de 830 furos;
- 1 x Protoboard de 400 furos;
- 1 x ESP-32 Dev Kit (30 Portas);
- 1 x KMI (Kit Modular IOT) (caso não a tenha, você pode utilizar a protoboard ou uma Shield expansora para ESP32);
- 1 x Display OLED 128 x 64;
- 15 x Chaves táteis, 12 para as notas e 3 para controlar o menu (uma dica, se quiser, você pode comprar o botão com as cores que correspondam à cor das teclas do teclado);
- 1 x Alto-Falante (32 Ohms);
- 1 x Potenciômetro B10K (Linear);
- 1 x Capacitor 10 µF (a Tensão deve ser de no mínimo 6V);
- Fios de cabo Jumper (Dupont Macho-Macho);
- Jumper Rígidos para Protoboard;
*/

//Bibliotecas utilizadas:
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

uint8_t partituraFrequencia[4];//Guarda a frequência da nota tocada anteriormente.

#define larguraOLED 128 //Largura dos pixels do display OLED.
#define alturaOLED 64 //Altura dos pixels do display OLED.
byte partituraY = 42; //Posição no eixo vertical das notas da partitura.

//Os pinos do I2C utilizado pelo display são pré-definidos pela Wire.h (SDA = D21 & SCL = D22).

#define resetOLED     -1 //Utilizar a porta de RESET do ESP32 para reiniciar o display.
#define enderecoOLED 0x3C ///Ver o datasheet para adquirir o endereço do seu OLED; 0x3D para resolução de 128x64, 0x3C para resoluçõa de 128x32.

Adafruit_SSD1306 display(larguraOLED, alturaOLED, &Wire, resetOLED);//Cria o Objeto "display".

#define numBotoes 12 //Define o número de teclas utilizadas no projeto (nesse exemplo, estamos usando uma Oitava, do "Dó" até "Si" e estamos incluindo as teclas pretas).

#define maxEventos 8000 //Número máximo de notas que poderão ser gravadas pelo projeto no modo partitura.

//Struct para definir as carcaterísticas das notas e intervalos no modo Partitura.

struct Evento {
  unsigned long timestamp;  //Tempo em que a ação se iniciou;
  unsigned int frequencia;  //Frequência da nota ou intervalo;
  uint8_t portaIndice;      //Indica se o tipo da tecla (preta ou branca);
  bool acidente;            //Indica se a  tecla, se for preta, é Bemol (b) ou Sustenida(#).
};

Evento partitura[maxEventos]; //Array que vai guardar os eventos das notas, o seu número máximo de casas pode ser alterado usando o #define maxEventos.

//Struct para controle de Debounce no modo Partitura.

struct Botao {
  bool estadoEstabilizado;
  bool antEst;
  unsigned long tempoUltimaMudanca;
};

Botao botoes[numBotoes];  //Array que vai guardar os estados dos botões, para o controle do debounce.

uint32_t totalEventos = 0; //Múmero total de notas e interrupções gravadas.
uint32_t inicioGravacao = 0; //Guarda o tempo em que a gravação se iniciou.

uint8_t notaAnterior = 0; //Guarda a nota anterior para definir se a nota será bemol ou sustenida.
uint8_t notasSeguradas = 0; //Guarda o númeo de teclas pressionadas ao mesmo tempo.

#define tempoDebounce 50 //Tempo em que o projeto analisa se tem ou não debouncing.

bool gravando = false; //Começa a gravar ou não a música após o aperto da 1ª tecla.
bool ordem; //Define a ordem da progresão (cresente ou decresente) para definir se as teclas pretas serão Bemóis(b) ou Sustenidas(#).

byte aba = 0; //Alterna entre as diferentes abas do código.
byte itens = 0; //Alterna entre os diferentes itens do menu.
#define itensMax 4 //Número máximo de itens no menu.

const uint8_t pinos[] = {14, 12, 13, 15, 4, 16, 17, 5, 18, 19, 3, 1}; //Portas utilizadas para cada uma das 12 teclas utilizadas.

//                          {Dó,   Ré,   Mi,Fá,  Sol,   Lá,   Si}
const uint8_t conversor[] = {0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6}; //array para converter o índice de botões para conrresponder apena as 7 notas principais.

//Array com os nomes das notas
const char* notasSaxao[] = {"C", "D", "E", "F", "G", "A", "B"};
const char* notasLatin[] = {"Do", "Re", "Mi", "Fa", "Sol", "La", "Si"};

#define retroceder 33 //Porta Porta em que é usado o botão para controlar para voltar um item no menu.
#define confirmar 25 //Porta em que é usado o botão para confirmar um item, para controlar se tem ou não "vibrato" (oscilação quadrada) nas notas.
#define avancar 27 //Porta em que é usado o botão para controlar para avançar um item no menu.

#define buzzer 26 //Porta em que é utilizado o Buzzer ou o Alto-falante.

#define potenciometro 34 //Porta em que é utilizado o Potenciômetro.

byte partituraNotas[4]; //Notas pressionadas no modo Partitura.
byte ordemPatitura; //Posição atual da nota que será pressionada no modo paritura.

byte oitava; //Número da oitava definida pelo potenciômetro.
bool modulation = 0; //Define se as notas vão modular ou não.
bool modo = 0; //Alterna entre os modos de partitura ou piano.
bool nomeNota = 0; //Alterna entre os 2 sistemas de notas (Anglo-Saxônico e Latino).
bool rodarIntro = 1; //Roda a animação de introdução ou não.