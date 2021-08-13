// testshapes demo for RGBmatrixPanel library.
// Demonstrates the drawing abilities of the RGBmatrixPanel library.
// For 32x64 RGB LED matrix.

// WILL NOT FIT on ARDUINO UNO -- requires a Mega, M0 or M4 board

#include <RGBmatrixPanel.h>
#include "tanimlamalar.h"



// Most of the signal pins are configurable, but the CLK pin has some
// special constraints.  On 8-bit AVR boards it must be on PORTB...
// Pin 8 works on the Arduino Uno & compatibles (e.g. Adafruit Metro),
// Pin 11 works on the Arduino Mega.  On 32-bit SAMD boards it must be
// on the same PORT as the RGB data pins (D2-D7)...
// Pin 8 works on the Adafruit Metro M0 or Arduino Zero,
// Pin A4 works on the Adafruit Metro M4 (if using the Adafruit RGB
// Matrix Shield, cut trace between CLK pads and run a wire to A4).

//#define CLK  8   // USE THIS ON ADAFRUIT METRO M0, etc.
//#define CLK A4 // USE THIS ON METRO M4 (not M0)
#define CLK 11 // USE THIS ON ARDUINO MEGA  deðiþtirilmiyor
#define OE  34// 9		//calisti
#define LAT 32//10		//calisti
#define A   A0
#define B   A1
#define C   A2
#define D   A3

RGBmatrixPanel matrix(A, B, C, D, CLK, LAT, OE, false, 64);
// Similar to F(), but for PROGMEM string pointers rather than literals
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

//const char str[] PROGMEM = "Adafruit 16x32 RGB LED Matrix";
const char str[] PROGMEM = "YOYUNCAK HIZLI&PEDALLI";
int16_t    textX         = matrix.width(),
           textMin       = sizeof(str) * -5,
           hue           = 0;
int8_t textY = 1;
bool yon_y_eksen = false; //aþaðý


int8_t ball[3][4] = {
  {  3,  0,  1,  1 }, // Initial X,Y pos & velocity for 3 bouncy balls
  { 17, 15,  1, -1 },
  { 27,  4, -1,  1 }
};
static const uint16_t PROGMEM ballcolor[3] = {
  0x0080, // Green=1
  0x0002, // Blue=1
  0x1000  // Red=1
};

#define  matrix_clear()  matrix.fillScreen(0)
#define matrix_show()    matrix.swapBuffers(true);
#define Color(x,y,z)     matrix.Color444(x/16,y/16,z/16)

const float radius_of_wheel = 0.45; //45 cm

struct bisikletVeri
{
	byte tip;
	byte veri_low;
	byte veri_high;
};

bisikletVeri serialVeri;

const byte interruptPin = 2;
char buf[10];
byte mod = 0;

byte oyun_sira = 0;
byte oyun_mod_secim = 0;
unsigned long time_oyun = 90000;

//void giris_anim();
//void bosta_bekle_anim();

void setup() {

	Serial.begin(9600);
	Serial2.setTimeout(1000);
	Serial2.begin(9600);

	Serial.println("basla");
	//pinMode(interruptPin, INPUT_PULLUP); //jeton giriþi için deneme
	//attachInterrupt(digitalPinToInterrupt(interruptPin), ISR_RX, FALLING);
	matrix.begin();

	giris_anim();

	matrix.setTextWrap(false); // Allow text to run off right edge
	matrix.setTextSize(1);
	serialFlush2();
}

void loop() {
basla:
	bosta_bekle_anim();
	//Serial.write('A');
	if (Serial2.available()){
		
		serialVeriAl2();
	}

	if ( serialVeri.tip== 'J') //jeton bilgisi geldi.buf[0]
	{ 
		serialVeri.tip = 0;

		oyun_mod_secim=bisiklet_secim_ekrani();

		if (oyun_mod_secim == OYUNCU_BISIKLET_SECIMI)
		{
			mod = oyun_modu_secim();

			matrix_clear();
			matrix.setCursor(0, 0);
			matrix.setTextColor(LED_PURPLE_HIGH);
			if (mod == BAY_MOD){
				matrix.println("MOD:");
				matrix.println("   BAY");
			}
			if (mod == BAYAN_MOD){
				matrix.println("MOD:");
				matrix.println("   BAYAN");
			}
			if (mod == COCUK_MOD){
				matrix.println("MOD:");
				matrix.println("   COCUK");
			}
			delay(2000);

			oyun_skor_ekrani();
			serialVeri.tip = 0;
			goto basla;
		}

		if (oyun_mod_secim == SISTEM)
		{
			//sistemle yarýþýlacak.....
			oyun_skor_ekrani();
			serialVeri.tip = 0;
			goto basla;
		}
	
	}//jeton

}//main

//void serialEvent2(){  //sadece loop içersinden çaðrýldýðýnda iþ yapýyor....
//	byte i = 0;
//	if (Serial2.available())
//	{
//		//delay(100); //allows all serial sent to be received together
//		while (Serial2.available())// && i < 6) 
//		{
//			buf[i++] = Serial2.read();
//			
//			//Serial.println(buf[0]);	//Serial.println(str,4);
//		}
//		//Serial2.flush();
//		
//		serialFlush2();
//		
//		Serial.println(serialVeri.tip);
//
//	}
//}

void serialVeriAl2(){  //sadece loop içersinden çaðrýldýðýnda iþ yapýyor....
	byte i = 0;
	delay(100); //allows all serial sent to be received together
	memset(buf, '\0', sizeof(buf));
	String str;
	if (Serial2.available()){
		
		//Serial2.readBytesUntil('c', buf, 3);
		str=Serial2.readStringUntil('c');
		Serial.println(str);
		serialVeri.tip = (byte)str[0];// buf[0];
		//memcpy(&serialVeri, buf, sizeof(serialVeri)); //hedef, kaynak, uzunluk

		serialFlush2();  //kilitlenme oluyor...
		/*for (int i = 0; i < 3; i++){
			Serial.print((char)buf[i]); Serial.print(", "); 
		}
		Serial.println();*/
		/*Serial.print("copy:");
		Serial.print(serialVeri.tip ); Serial.print(serialVeri.veri_low ); Serial.print(serialVeri.veri_high);
		Serial.println();*/
		
		Serial2.flush();
	}
	
}

void oyun_skor_ekrani(){

	unsigned long time_oynan_sure = 0;;
	int hiz =-1;
	unsigned int donme_sayisi = 0;
	float yol = 0;
	unsigned int tur = 0;
	int x_fill = 1;
	matrix_clear();
	matrix.setCursor(0, 0);
	matrix.setTextSize(1);
	time_oynan_sure = millis();

	do{

		if (Serial2.available()) serialVeriAl2();

		if (millis() - time_oynan_sure >(time_oyun/60)){
			time_oynan_sure = millis();
			x_fill++;
		}

		if (serialVeri.tip == OYUNCU_HIZ_VERI){
			hiz = serialVeri.veri_low;
			if (hiz < 0) hiz = -hiz;
			//Serial.print("HIZ:"); Serial.println(hiz);
		}

		if (serialVeri.tip == OYUNCU_TEKERLEK_DONME_SAYISI){
			donme_sayisi = (serialVeri.veri_low) | (serialVeri.veri_high << 8);
			//Serial.print("R:"); Serial.println(donme_sayisi);
			yol = (donme_sayisi*radius_of_wheel);		//metre
			tur = (unsigned int)(yol / 8.51f);
		}

	/*	if (serialVeri.tip == OYUNCU_TUR_SAYISI){
			tur = (serialVeri.veri_low) | (serialVeri.veri_high << 8);
		}*/

		if (serialVeri.tip == OYUN_OYNANAN_SURE)
		{
			matrix_clear();
			matrix.setCursor(0, 0);
			matrix.setTextSize(1);

			matrix.setTextColor(LED_GREEN_HIGH);
			matrix.print(hiz); matrix.println(" km/Sa");

			matrix.setTextColor(LED_CYAN_HIGH);
			//matrix.print(donme_sayisi); matrix.println(" km");
			matrix.print(yol); matrix.println(" m");

			matrix.setTextColor(LED_ORANGE_HIGH);
			matrix.print(tur);matrix.println(" TUR");

			matrix.fillRect(2, 28, 60, 4, LED_GREEN_LOW);
			matrix.fillRect(2, 28, x_fill, 4, LED_RED_LOW);	//x_fill max 60

			//if (x_fill >= 60) break;
		}

		if (serialVeri.tip == OYUNCU_YARIS_SIRA1){
			matrix.setTextColor(LED_RED_HIGH);
			matrix.setTextSize(2);
			matrix.setCursor(52, 12);
			matrix.println("1");
			oyun_sira = 1;
		}
		else if (serialVeri.tip == OYUNCU_YARIS_SIRA2){
			matrix.setTextColor(LED_RED_HIGH);
			matrix.setTextSize(2);
			matrix.setCursor(52, 12);
			matrix.println("2");
			oyun_sira = 2;
		}
		else if (serialVeri.tip == OYUNCU_YARIS_SIRA0){
			matrix.setTextColor(LED_RED_HIGH);
			matrix.setTextSize(2);
			matrix.setCursor(52, 12);
			matrix.println("0");
			oyun_sira = 0;
		}

		if (serialVeri.tip == OYUN_BITTI){
			break;
		}


		matrix_show();
		serialVeri.tip = 0;

	} while (serialVeri.tip != OYUN_BITTI);// (1);

	delay(250);
	serialVeri.tip = 0;
	oyun_bitti_ekrani();

}

void oyun_bitti_ekrani()
{
	matrix_clear();
	matrix.setCursor(0, 0);
	matrix.setTextSize(1);

	//do{
	//	if (Serial2.available()) serialVeriAl2();

	//	if (serialVeri.tip == BOS_ANIM_EKRANI){
	//		break;
	//	}
	//	matrix_clear();

	//	matrix.setTextColor(LED_WHITE_HIGH);
	//	matrix.println("! BITTI !");
	//	matrix.println();

	//	if (oyun_sira == 1){
	//		matrix.setTextColor(LED_GREEN_HIGH);
	//		matrix.println("1. 'siniz.");

	//	}
	//	if (oyun_sira == 2){
	//		matrix.setTextColor(LED_GREEN_HIGH);
	//		matrix.println("2. 'siniz");

	//	}
	//	if (oyun_sira == 0){
	//		matrix.setTextColor(LED_GREEN_HIGH);
	//		matrix.println("BERABERE.");

	//	}
	//	delay(50);

	//} while (1);

	matrix_clear();

	matrix.setTextColor(LED_WHITE_HIGH);
	matrix.println("! BITTI !");
	matrix.println();

	if (oyun_sira == 1){
		matrix.setTextColor(LED_GREEN_HIGH);
		matrix.println("1. 'siniz.");

	}
	if (oyun_sira == 2){
		matrix.setTextColor(LED_GREEN_HIGH);
		matrix.println("2. 'siniz");

	}
	if (oyun_sira == 0){
		matrix.setTextColor(LED_GREEN_HIGH);
		matrix.println("BERABERE.");

	}
	while (1){
		if (Serial2.available()) serialVeriAl2();
		if (serialVeri.tip == BOS_ANIM_EKRANI){
			break;
		}
	}
	
	delay(5000);
}

byte  bisiklet_secim_ekrani()
{
	unsigned long time = 0;
	int zaman_say = 0;
	matrix_clear();
	matrix.setCursor(0, 0);
	matrix.setTextColor(LED_RED_HIGH);
	matrix.println("Bisiklet");
	//delay(BASLAMA_YAZI_SURE);
	matrix.println();
	matrix.setTextColor(LED_GREEN_HIGH);
	matrix.println("  SEC");

	time = millis();

	while(1){

		if (Serial2.available()) serialVeriAl2();

		if (serialVeri.tip == JETON_AT){
			matrix_clear();
			matrix.setCursor(0, 0);
			matrix.setTextColor(LED_BLUE_HIGH);
			matrix.println("2. OYUNCU:");
			matrix.println("JETON");
			matrix.println("gerekli...");
			//time = millis();
		}

		if (serialVeri.tip == OYUNCU_BISIKLET_SECIMI )	
			return OYUNCU_BISIKLET_SECIMI;

		if (serialVeri.tip == SISTEM) 
			return SISTEM;

		if (millis() - time > 1000)
		{
			zaman_say++;
			matrix.setCursor(55, 12);
			matrix.setTextSize(1);

			matrix.fillRect(55, 12, 8, 8, LED_BLACK);

			matrix.setTextColor(LED_RED_HIGH);
			matrix.print(((OYUNMOD_SECME_SURESI / 1000) - zaman_say));


			time = millis();
		}

		if (zaman_say > (OYUNMOD_SECME_SURESI / 1000)) return false;

		serialVeri.tip = 0;
		delay(10);
	}
	return 255;
}

byte oyun_modu_secim()
{
	int satir =-1;
	unsigned long time = 0;
	int zaman_say = 0;
	matrix_clear();
	mod_secim_ilkEkran();
	
	time = millis();

	while (1)
	{
		serialVeriAl2();

		if (serialVeri.tip == OYUNCU_MOD_SECIMI) //(buf[0] == OYUNCU1_MOD_SECIMI)
		{
			satir++;
			if (satir > 2)satir = 0;
			
			switch (satir)
			{
				case 0:
					matrix.drawRect(0, 24, 64, 8, LED_BLACK);
					matrix.drawRect(0, 8, 64, 8, LED_WHITE_HIGH);
					break;
				case 1:
					matrix.drawRect(0, 8, 64, 8, LED_BLACK);
					matrix.drawRect(0, 16, 64, 8, LED_WHITE_HIGH);
					break;
				case 2:
					matrix.drawRect(0, 16, 64, 8, LED_BLACK);
					matrix.drawRect(0, 24, 64, 8, LED_WHITE_HIGH);
					break;
			}
			mod_secim_ilkEkran();
			zaman_say = 0;
		}

		if (serialVeri.tip == OYUNCU_MOD_SECIM_SONU){
			if (satir == -1) return BAY_MOD;
			else  return satir;
		}

		serialVeri.tip = '0';
		if (millis() - time > 1000) 
		{
			zaman_say++;
			matrix.setCursor(54, 0);
			matrix.setTextSize(1);

			matrix.fillRect(54, 0, 8, 8, LED_BLACK);

			matrix.setTextColor(LED_RED_LOW);
			matrix.print(((OYUNMOD_SECME_SURESI/1000)- zaman_say));


			time = millis();
		}
	}

}//
void mod_secim_ilkEkran()
{
	//matrix_clear();
	matrix.setCursor(0, 0);
	matrix.setTextColor(LED_BLUE_HIGH);
	matrix.println(" MOD SEC:");
	//delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(LED_GREEN_HIGH);
	matrix.println("* Bay");
	//delay(BASLAMA_YAZI_SURE);
	matrix.println("* Bayan");
	//delay(BASLAMA_YAZI_SURE);
	matrix.println("* Cocuk");
	//delay(BASLAMA_YAZI_SURE);
	matrix_show();
	
}

void bosta_bekle_anim()
{
	byte i;
	// Clear background
	matrix.fillScreen(0);
	matrix.setTextSize(1);
	//    // Bounce three balls around
	for (i = 0; i<3; i++) {
		// Draw 'ball'
		matrix.fillCircle(ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));
		// Update X, Y position
		ball[i][0] += ball[i][2];
		ball[i][1] += ball[i][3];
		// Bounce off edges
		if ((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
			ball[i][2] *= -1;
		if ((ball[i][1] == 0) || (ball[i][1] == (matrix.height() - 1)))
			ball[i][3] *= -1;
	}

	// Draw big scrolly text on top
	matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
	matrix.setCursor(textX, textY);

	if (!yon_y_eksen)textY++;
	if (yon_y_eksen)textY--;
	if (textY > 24)yon_y_eksen = true;//yukarý
	if (textY<2) yon_y_eksen = false;//aþaðý
	matrix.print(F2(str));

	// Move text left (w/wrap), increase hue
	if ((--textX) < textMin) textX = matrix.width();
	hue += 7;
	if (hue >= 1536) hue -= 1536;

	delay(5);
	// Update display
	matrix.swapBuffers(false);
}

void giris_anim()
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	// draw some text!
	matrix.setTextSize(1);     // size 1 == 8 pixels high
	matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

	matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
	uint8_t w = 0;
	char *str = " YILDIZ   OYUNCAK ";
	for (w = 0; w<8; w++) {
		matrix.setTextColor(Wheel(w));
		matrix.print(str[w]);
		delay(BASLAMA_YAZI_SURE);
	}
	delay(BASLAMA_YAZI_SURE * 2);
	matrix.setCursor(2, 8);    // next line
	for (w = 8; w<18; w++) {
		matrix.setTextColor(Wheel(w));
		matrix.print(str[w]);
		delay(BASLAMA_YAZI_SURE);
	}
	delay(BASLAMA_YAZI_SURE * 2);
	matrix.println();

	matrix.setTextColor(matrix.Color333(7, 7, 7));
	matrix.println(" YOYUNCAK ");
	delay(BASLAMA_YAZI_SURE * 2);

	// print each letter with a rainbow color
	matrix.setTextColor(matrix.Color333(7, 0, 0));
	matrix.print('Y'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(7, 4, 0));
	matrix.print('O'); delay(BASLAMA_YAZI_SURE);
	matrix.setCursor(14, 24);
	matrix.setTextColor(matrix.Color333(7, 7, 0));
	matrix.print('Y'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(4, 7, 0));
	matrix.print('A'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(0, 7, 0));
	matrix.print('R'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(0, 7, 7));
	matrix.print('I'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(0, 4, 7));
	matrix.print('S'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(0, 0, 7));
	matrix.setCursor(46, 24);
	matrix.print('V'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(4, 0, 7));
	matrix.print('0'); delay(BASLAMA_YAZI_SURE);
	matrix.setTextColor(matrix.Color333(7, 0, 4));
	matrix.print('1'); delay(BASLAMA_YAZI_SURE);

	delay(3000);
	matrix.fillScreen(0);
}

void serialFlush2(){
	while (Serial2.available() > 0) {
		char t = Serial2.read();
	}
}

// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}
