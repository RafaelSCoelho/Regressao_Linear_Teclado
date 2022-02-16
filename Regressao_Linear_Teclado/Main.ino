#include <LiquidCrystal.h>
#include <Keypad.h>

LiquidCrystal lcd(12,13,14,15,16,17);
const byte fil=4;
const byte col=4;

byte pin_fil[]={11,10,9,8};
byte pin_col[]={7,6,5,4};

//Mapeando o Teclado 4x4
char teclas[fil][col] =
{
  {'1', '2', '3', '-'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'.', '0', '=', 'D'}
};

Keypad teclado4x4 = Keypad(makeKeymap(teclas), pin_fil, pin_col, fil, col);
char tecla, copia[5];

float *x, *y, x_vetor[4], y_vetor[4], M = 0.0, B = 0.0;
float *x_ptr = x_vetor;
float *y_ptr = y_vetor;

void pipelineVetor(float *vetor){
	vetor[0] = vetor[1];
  	vetor[1] = vetor[2];
  	vetor[2] = vetor[3];
  	vetor[3] = 0;
}

void insereVetor(float *vetor, int flag)
{
  lcd.setCursor(0, 0);
  String x[4];
  int vetor_ok = 0, pos, i;
  int linha = 0, coluna = 0;
  if (flag == 0){
  	pos = 0;
    i = 0;
  }else if(flag==1){
  	pos = 3;
    i = 3;
  }
  tecla = teclado4x4.getKey();
  while (tecla != NO_KEY or vetor_ok == 0)
  {
    if (tecla == '0' or tecla == '1' or tecla == '2' or tecla == '3' or
        tecla == '4' or tecla == '5' or tecla == '6' or tecla == '7' or
        tecla == '8' or tecla == '9' or tecla == '-' or tecla == '.')
    {
      lcd.setCursor(coluna, 1);
      lcd.print(tecla);
      x[i] += tecla;
      vetor[i] = x[i].toFloat();
      coluna++;
    }
    else if (tecla == '=')
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("[");
      if(flag == 0){
        pos = 0;
        while (pos <= 3)
        {
          lcd.print(x[pos]);
          if (pos < 3)
          {
            lcd.print(",");
          }
          pos++;
        }
      }else if(flag == 1){
      	lcd.print(x[pos]);
      }
      lcd.print("]");
      if (i < 3)
      {
        coluna = 0;
        lcd.setCursor(coluna, 1);
        lcd.print("                ");
        i++;
      }
      else
      {
        vetor_ok = 1;
        coluna = 0;
        lcd.setCursor(coluna, 1);
        lcd.print("Vetor completo!");
      }
    }
    delay(30);
    tecla = teclado4x4.getKey();
  }
}


void
ImprimeVetor (float *x, float *y)
{
  int i = 0;
  lcd.setCursor (0, 0);
  lcd.print ("T[");
  for (i = 0; i < 4; i++)
    {
      lcd.print (x[i], 1);
      if (i < 3)
	{
	  lcd.print ("|");
	}
    }
  lcd.print ("]");
  lcd.setCursor (0, 1);
  lcd.print ("t[");
  for (i = 0; i < 4; i++)
    {
      lcd.print (y[i], 1);
      if (i < 3)
	{
	  lcd.print ("|");
	}
    }
  lcd.print ("]");
  delay (2000);
}


void
regLinearXY (float x[4], float y[4])
{
  int i;
  float x1 = 0.0, y1 = 0.0, xy = 0.0, x2 = 0.0;
  for (int i = 0; i < 4; i++)
    {
      x1 = x1 + x[i];
      y1 = y1 + y[i];
      xy = xy + x[i] * y[i];
      x2 = x2 + x[i] * x[i];
    }
  float J = 0.0;
  J = 4 * x2 - (x1 * x1);
  if (J != 0.0)
    {
      M = ((4 * xy) - (x1 * y1)) / J;
      B = ((y1 * x2) - (x1 * xy)) / J;
    }

  int alert = M;
  lcd.clear ();
  lcd.setCursor (0, 0);
  lcd.print ("f(x)=");
  lcd.print (M);
  lcd.print ("x+");
  lcd.print (B);
  delay (2000);

}
void setup()
{
  pipelineVetor(x_ptr);
  pipelineVetor(y_ptr);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Digite o vetor X: ");
  insereVetor(x_ptr, 0);
  delay(2000);
  lcd.clear();
  lcd.print("Digite o vetor Y: ");
  insereVetor(y_ptr, 0);
  lcd.clear();
  regLinearXY(x_ptr, y_ptr);
  ImprimeVetor (x_ptr, y_ptr);
  delay(2000);
  lcd.clear();
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("Digite o vetor X: ");
  insereVetor(x_ptr, 1);
  delay(2000);
  lcd.clear();
  lcd.print("Digite o vetor Y: ");
  insereVetor(y_ptr, 1);
  lcd.clear();
  regLinearXY(x_ptr, y_ptr);
  ImprimeVetor (x_ptr, y_ptr);
  delay(2000);
  lcd.clear();
}