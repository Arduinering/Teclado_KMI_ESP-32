//Aba que contém as funções extras, utilizadas além do modo teclado e do modo partitura.

/*
Intercala o som do Buzzer entre 2 frequências (eu decidi utilizar a função de gerarNota() para utilizar uma nota em oitavas diferentes) para dar uma 
ilusão de vibrato, você pode definir a porta que ira vibrar a frequência máxima da mesma e a mínima, além de que, se quiser, pode definir um tempo de 
pausa entre ambas.
*/
void toneModulation(byte porta, float min, float max, byte Delay = 50){
  ledcWriteTone(porta, max);
  delay(Delay);
  ledcWriteTone(porta, min);
  delay(Delay);
}

/*
Gera o valor de qualquer nota com base na nota Dó da 1ª Oitava (32,7 Hz). 
Também consegue chegar em qualquer oitava com base no valor originado do cálculo anterior.
Dei a ela o apelido de: "Uma nota para todas alcançar."
*/
float gerarNota(byte semiTons, byte oitava){
  //                            -> Gera a nota <-          -> Alterna a Oitava <-
  float notaResultante = (32.7 * pow(2, semiTons / 12.0)) * (pow(2,(oitava -1)));
  return notaResultante;
}

//Faz a animação de introdução
void intro(){
  display.clearDisplay();
  display.drawBitmap(0, 2, logo, 128, 64, WHITE);
  display.display();
  delay(5000);

  display.setTextSize(1);
  display.clearDisplay();
  display.setCursor(5, 0);
  display.print("MECATRONICA JOVEM:");
  display.display();
  delay(2000);

  display.setCursor(32, 25);
  display.print("projeto da");
  display.setCursor(0, 32);
  display.print("temporada de musica!");
  display.display();
  delay(3000);

  display.clearDisplay();
  display.setCursor(30, 0);
  display.print("CREDITOS:");
  display.setCursor(0, 20);
  display.print("Kit Modular IOT por:");
  display.setCursor(20, 36);
  display.print("Mauro Miyashiro");
  display.display();
  delay(5000);

  display.clearDisplay();
  display.setCursor(30, 0);
  display.print("CREDITOS:");
  display.setCursor(2, 20);
  display.print("Piano no ESP32 por:");
  display.setCursor(20, 36);
  display.print("Nicolas Costa");
  display.display();
  delay(5000);

  display.clearDisplay();
  display.drawBitmap(0, 0, title_1, 128, 64, WHITE);
  display.display();

  //Roda a música de introdução
  for(byte x = 0; x <= 1; x++){
    ledcWriteTone(buzzer, gerarNota(0,3));
    delay(250);
    ledcWriteTone(buzzer, gerarNota(4,3));
    delay(250);
    ledcWriteTone(buzzer, gerarNota(7,3));
    delay(250);
  }

  for(byte x = 0; x <= 1; x++){
    ledcWriteTone(buzzer, gerarNota(2,3));
    delay(250);
    ledcWriteTone(buzzer, gerarNota(5,3));
    delay(250);
    ledcWriteTone(buzzer, gerarNota(9,3));
    delay(250);
  }

  for(byte x = 0; x <= 2; x++){
    ledcWriteTone(buzzer, gerarNota(4,3));
    delay(250);
    ledcWriteTone(buzzer, gerarNota(7,3));
    delay(250);
    ledcWriteTone(buzzer, gerarNota(11,3));
    delay(250);
  }

  ledcWriteTone(buzzer, gerarNota(0,4));
  delay(250);

  ledcWriteTone(buzzer, 0);
}

//Função que permite selecionar os diferentes Itens das Abas.
byte selecionar(byte item, byte min, byte max){
  //Volta para o item anterior
    if(!digitalRead(retroceder)){
      if(item > min){
        item--;
      }else
      if(item == min){
        item = max;
      }
      ledcWriteTone(buzzer, gerarNota(0,2));
      delay(100);
      ledcWriteTone(buzzer, 0);
    }

    //Vai para o próximo item do menu
    if(!digitalRead(avancar)){
      if(item < max){
        item++;
      }else
      if(item == max){
        item = min;
      }
      ledcWriteTone(buzzer, gerarNota(0,2));
      delay(100);
      ledcWriteTone(buzzer, 0);
    }
    return item;
}

//Item que faz o usuário voltar caso pressione o botão de confirmar, item anterior define o item do menu que será indicado ao voltar.
void displayVoltar(byte itemAnt){
      display.setTextSize(1);
      display.setCursor(0,0); //(X,Y)
      display.print("aperte a chave para:");
      display.setTextSize(2);
      display.setCursor(0,20); //(X,Y)
      display.print("VOLTAR AO");
      display.setCursor(25,40); //(X,Y)
      display.print("MENU");

      //Muda a aba ao apertar o botão do Encoder
      if(!digitalRead(confirmar)){
        aba = 1;
        itens = itemAnt;
        ledcWriteTone(buzzer, gerarNota(7,5));
        delay(100);
        ledcWriteTone(buzzer, gerarNota(0,5));
        delay(100);
        ledcWriteTone(buzzer, 0);
      }
}

//Função que evita que o potenciômetro fique oscilando, ela realiza uma média somando o valor de 64 estados diferentes do potenciômetro e dividindo-os por 64.
uint16_t AnalogReadFiltro(uint8_t porta){
  const uint16_t leituras = 64;
  long soma = 0;

  for(int i = 0; i < leituras; i++) {
    soma += analogRead(porta);
  }

  int media = soma / leituras;
  return media;
}