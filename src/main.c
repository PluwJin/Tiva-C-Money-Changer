
// ERHAN ÖZDOĞAN 160201039
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/tm4c123gh6pm.h"
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/systick.h"
#include "string.h"

// PORTLARI BAŞLATMA FONKSİYONLARI PROTOTİPLERİ
void init_port_A();
void init_port_D();
void init_port_C();

// 3SN ARALIKLARLA HESAPLAYAN FONKSİYON PROTOTİPİ
void bozdur(int sayi);

// LCD FONKSİYONLARI PROTOTİPLERİ

void LCD_KOMUT_ISLE(unsigned char);  //Lcd ye komut göndermeye yarar
void KARAKTER_ISLE(char*);                   //karakter yazdırır
void STRING_ISLE(char*);                   // String yazdırır
void LCD_BASLAT(void);                    //Lcd başlangıç ayarları
void LCD_VERI_ISLE(unsigned char);  //Tek karekter yazdırır



char remp[5];
char adet[2];
char str[5]="00.00";
int button=0,reset=0;
int digit1=0,digit2=0,digit3=0,digit4=0,degis=0;
short x=0,x1=0,y=0,y1=0,z=0,z1=0,t=0,t1=0,k=0,k1=0;
int digit_son=0;
volatile unsigned delay=0;


void main(){



 	SysCtlClockSet( SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ| SYSCTL_OSC_MAIN);


	init_port_D();
	init_port_C();
	init_port_A();
	LCD_BASLAT();




	while(1){

 button= GPIO_PORTA_DATA_R & 0b01111100;// Buton basılmışmı kontrol eder (bit maskeleme)

 if(button!=degis){

	if(button==4){ // Eğer Port A Pın 2 deki butona basılırsa 00000010 = 4 olacağından port D nin Pın 0 daki led yanar
		 digit1=(digit1+1)%10; // her basışta sayı bir artar maks 9 olabilir
		str[0]=digit1+'0';     // Sayıyı char olarak string e atar

		    x=1;              // Bonus 1 için gerekli
		    x1=1;             // Lcd ye yazdırma için gerekli
		    delay=0;
		    reset=0;
		    GPIO_PORTC_DATA_R |= 0b00010000; // Ledi Yak


	}
	else if(button==8){

		GPIO_PORTC_DATA_R |= 0b00100000;
		digit2=(digit2+1)%10;
		str[1]=digit2+'0';

	        y=1;
	        y1=1;
	        delay=0;
	        reset=0;

	}
	else if(button==16){   // ' . ' ve Reset butonu

		GPIO_PORTD_DATA_R |= 0b00000100;
		str[2]='.';
			z=1;
			z1=1;
			delay=0;
			reset=0;


	}
	else if(button==32){

		GPIO_PORTD_DATA_R |= 0b00001000;
		digit3=(digit3+1)%10;
		    str[3]=digit3+'0';

	        t=1;
	        t1=1;
			delay=0;
			reset=0;

	}
	else if(button==64){
		GPIO_PORTC_DATA_R |= 0b01000000;
		 digit4=(digit4+1)%10;
			str[4]=digit4+'0';

	        k=1;
	        k1=1;
		    delay=0;
		    reset=0;

	}
	else{
 // Ledleri Söndür

    GPIO_PORTC_DATA_R &= ~(0b00010000);
	GPIO_PORTC_DATA_R &= ~(0b00100000);
	GPIO_PORTD_DATA_R &= ~(0b00000100);
	GPIO_PORTD_DATA_R &= ~(0b00001000);
    GPIO_PORTC_DATA_R &= ~(0b01000000);
    SysCtlDelay(50000);



// HANGİ BUTONA BASILDIYSA O BASAMAKTAKİ SAYI EKRANA YAZDIRILIR ------------------------------------------------------------------------------

    if(x1==1 || y1==1 || z1==1 || k1==1 || t1==1){


    	if(x1==1){
    		LCD_KOMUT_ISLE(0x80+11);
    		KARAKTER_ISLE(&str[0]);}

    	else if(y1==1){
    		LCD_KOMUT_ISLE(0x80+12);
    		KARAKTER_ISLE(&str[1]);}

    	else if(z1==1){
    		LCD_KOMUT_ISLE(0x80+13);
    		KARAKTER_ISLE(&str[2]);}
        else if(t1==1){
        	LCD_KOMUT_ISLE(0x80+14);
        	KARAKTER_ISLE(&str[3]);}

    	else if(k1==1){
    		LCD_KOMUT_ISLE(0x80+15);
    		KARAKTER_ISLE(&str[4]);}

    	x1=0;y1=0;z1=0;t1=0;k1=0;
    	}


	} //else

    } //if

 degis=button;
 // BONUS 1: 3. BUTON HARİÇ HERHANGİ BİR BUTONA BASILDIKTAN 5 SANİYE SONRA HESAPLAMA KISMINA GEÇER----------------------------------------------

 if( (x==1 || y==1 || t==1 || k==1) && (x1!=1 && y1!=1 && z1!=1 && t1!=1 && k1!=1) ){ // Program ilk Başladığında girilmesini engeller ve basılı tutulduğunda girmeyi engeller
      	delay++;
      	if(delay==2500000){
      		x=0;y=0;z=0;t=0;k=0;
      		digit_son=digit1*1000+digit2*100+digit3*10+digit4;// Son oluşan sayı
      		GPIO_PORTC_DATA_R |= 0b00010000;
      		for(int i=0;i<600000;i++){}
      		 GPIO_PORTC_DATA_R &=~(0b00010000);
      		 GPIO_PORTA_DATA_R &= (0b00000000);
      		bozdur(digit_son);

  	}
  	}

 //3. BUTONA BASILI TUTULURSA LCD VE SAYILAR RESETLENİR ----------------------------------------------------------------------------------------

 else if(button == 16){
	 reset++;
	 if(reset==1700000){
		 GPIO_PORTD_DATA_R |= 0b00001000;
		 for(int i=0;i<600000;i++){}
		 GPIO_PORTD_DATA_R &=~(0b00001000);
		 str[0]=0+'0'; str[1]=0+'0'; str[2]=0+'0'; str[3]=0+'0'; str[4]=0+'0';
		 	reset=0;
		 	digit1=0;digit2=0;digit3=0;digit4=0;
		 	x=0;x1=0;y=0;y1=0;z=0;z1=0;t=0;t1=0;k=0;k1=0;
		 	digit_son=0;
		    delay=0;
		    x=0;x1=0;y=0;y1=0;z=0;z1=0;t=0;t1=0;k=0;k1=0;
		    LCD_KOMUT_ISLE(0x01);

	 }
 }



  }// while
} // main

// PORTA BAŞLANGIÇ AYARLARI

void init_port_A() { //Buton Portu
 volatile unsigned long tmp;
 SYSCTL_RCGCGPIO_R |= 0x01; //Port A nın Saati başlar
 tmp = SYSCTL_RCGCGPIO_R;   // Delay
 GPIO_PORTA_DIR_R = 0b10000000; // Butonlar  için pinler giriş olarak ayarlandı
 GPIO_PORTA_DEN_R = 0xFF;	    // Pinler aktif hale getirildi
}

// PORTD BAŞLANGIÇ AYARLARI

void init_port_D() {
volatile unsigned long delay;
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
delay = SYSCTL_RCGC2_R;
GPIO_PORTD_DIR_R |= 0b00011100;
GPIO_PORTD_AFSEL_R &= ~(0b00011100);
GPIO_PORTD_DEN_R |= 0b00011100;

}

// PORTC BAŞLANGIÇ AYARLARI

void init_port_C() {
volatile unsigned long delay;
SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC;
delay = SYSCTL_RCGC2_R;
GPIO_PORTC_DIR_R |= 0x70;
GPIO_PORTC_AFSEL_R &= ~(0x70);
GPIO_PORTC_DEN_R |= 0x70;

}

//-----------------------------------------------------------------------------------------------------------------------------------
//------------------------// Bozdurma işlemini yapan ve 3 Sn Aralıkla Ekranda gösteren Fonksiyon //----------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------
void bozdur(int sayi){
	LCD_KOMUT_ISLE(0x01); // Ekranı temizle
	int on=0,bir=0,sifirbir=0,sifir2bir=0; //Sayıyı yazdırmak için basamaklar
	int yirmilik=0,onluk=0,beslik=0,birlik=0,yarimlik=0,ceyreklik=0,metelik=0,kurusluk=0; //Banknot sayıları
	int temp;

	yirmilik=sayi/2000; // Banknot sayısı hesaplandı sayı XX.XX şekli yerine XXXX şeklinde gönderildi doubledan kanyaklı sorunlar önlendi
	temp=sayi-yirmilik*2000;  // Kalan sayı temp e atandı
	sayi=sayi%2000;           // sayı güncellendi

	on=temp/1000; temp=temp-(on*1000); //Sayının onlar basamağı hesaplandı
	bir=temp/100; temp=temp-(bir*100);  //Sayının birler basamağı hesaplandı
	sifirbir=temp/10; temp=temp-(sifirbir*10); //Sayının ondabirler basamağı hesaplandı
	sifir2bir=temp;                           //Sayının yüzdebirler basamağı hesaplandı
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0'; // kalan sayının basamakları char a dönüştürüldü ve diziye atıldı
	adet[0]=yirmilik+'0';adet[1]='-';  // Banknot sayısı chara dönüştürüldü ve diziye atıldı


	LCD_KOMUT_ISLE(0x80+(11)); // 1. satır 12. sütuna git
	STRING_ISLE(remp);         // Kalan parayı yazdır
    LCD_KOMUT_ISLE(0xC0);      // 2. Satırın başına git
    STRING_ISLE(adet);         // Bozuk parayı yazdır
    STRING_ISLE("Yirmilik");    // Paranın cinsini yazdır.

   // SysCtlDelay(SysCtlClockGet());
    // 3 Saniye Beklendi bu arada resetleme için butona basılmışmı kontrol edildi.
    for(int i=0;i<6000000;i++){
    	if((GPIO_PORTA_DATA_R & 0b00010000)==16)
    		goto Loop;
    }

    //----------------------------------------------------------------
	LCD_KOMUT_ISLE(0x01);


	onluk=sayi/1000;
	temp=sayi-onluk*1000;
	sayi=sayi%1000;

	on=temp/1000; temp=temp-(on*1000);
	bir=temp/100; temp=temp-(bir*100);
	sifirbir=temp/10; temp=temp-(sifirbir*10);
	sifir2bir=temp;
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0';
	adet[0]=onluk+'0';adet[1]='-';

	LCD_KOMUT_ISLE(0x80+(11));
	STRING_ISLE(remp);
	LCD_KOMUT_ISLE(0xC0);
	STRING_ISLE(adet);
	STRING_ISLE("Onluk");
	//SysCtlDelay(SysCtlClockGet());
    for(int i=0;i<6000000;i++){
    	if((GPIO_PORTA_DATA_R & 0b00010000)==16)
    		goto Loop;
    }
	//---------------------------------------------------------------
	LCD_KOMUT_ISLE(0x01);

	beslik=sayi/500;
	temp=sayi-beslik*500;
	sayi=sayi%500;

	on=temp/1000; temp=temp-(on*1000);
	bir=temp/100; temp=temp-(bir*100);
	sifirbir=temp/10; temp=temp-(sifirbir*10);
	sifir2bir=temp;
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0';
	adet[0]=beslik+'0';adet[1]='-';


	LCD_KOMUT_ISLE(0x80+(11));
	STRING_ISLE(remp);
	LCD_KOMUT_ISLE(0xC0);
	STRING_ISLE(adet);
	STRING_ISLE("Beslik");
	//SysCtlDelay(SysCtlClockGet());
    for(int i=0;i<6000000;i++){
    	if((GPIO_PORTA_DATA_R & 0b00010000)==16)
    		goto Loop;
    }
	//---------------------------------------------------------------
	LCD_KOMUT_ISLE(0x01);

	birlik=sayi/100;
	temp=sayi-birlik*100;
	sayi=sayi%100;

	on=temp/1000; temp=temp-(on*1000);
	bir=temp/100; temp=temp-(bir*100);
	sifirbir=temp/10; temp=temp-(sifirbir*10);
	sifir2bir=temp;
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0';
	adet[0]=birlik+'0';adet[1]='-';

	LCD_KOMUT_ISLE(0x80+(11));
	STRING_ISLE(remp);
	LCD_KOMUT_ISLE(0xC0);
	STRING_ISLE(adet);
	STRING_ISLE("Birlik");
	//SysCtlDelay(SysCtlClockGet());
    for(int i=0;i<6000000;i++){
    	if((GPIO_PORTA_DATA_R & 0b00010000)==16)
    		goto Loop;
    }
	//---------------------------------------------------------------
	LCD_KOMUT_ISLE(0x01);

	yarimlik=sayi/50;
	temp=sayi-yarimlik*50;
	sayi=sayi%50;

	on=temp/1000; temp=temp-(on*1000);
	bir=temp/100; temp=temp-(bir*100);
	sifirbir=temp/10; temp=temp-(sifirbir*10);
	sifir2bir=temp;
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0';
	adet[0]=yarimlik+'0';adet[1]='-';

	LCD_KOMUT_ISLE(0x80+(11));
	STRING_ISLE(remp);
	LCD_KOMUT_ISLE(0xC0);
	STRING_ISLE(adet);
	STRING_ISLE("Yarimlik");
	//SysCtlDelay(SysCtlClockGet());
    for(int i=0;i<6000000;i++){
    	if((GPIO_PORTA_DATA_R & 0b00010000)==16)
    		goto Loop;
    }
	//---------------------------------------------------------------
	LCD_KOMUT_ISLE(0x01);

	ceyreklik=sayi/25;
	temp=sayi-ceyreklik*25;
	sayi=sayi%25;

	on=temp/1000; temp=temp-(on*1000);
	bir=temp/100; temp=temp-(bir*100);
	sifirbir=temp/10; temp=temp-(sifirbir*10);
	sifir2bir=temp;
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0';
	adet[0]=ceyreklik+'0';adet[1]='-';

	LCD_KOMUT_ISLE(0x80+(11));
	STRING_ISLE(remp);
	LCD_KOMUT_ISLE(0xC0);
	STRING_ISLE(adet);
	STRING_ISLE("Ceyreklik");
	//SysCtlDelay(SysCtlClockGet());
    for(int i=0;i<6000000;i++){
    	if((GPIO_PORTA_DATA_R & 0b00010000)==16)
    		goto Loop;
    }

	//---------------------------------------------------------------
	LCD_KOMUT_ISLE(0x01);

	metelik=sayi/10;
	temp=sayi-metelik*10;
	sayi=sayi%10;

	on=temp/1000; temp=temp-(on*1000);
	bir=temp/100; temp=temp-(bir*100);
	sifirbir=temp/10; temp=temp-(sifirbir*10);
	sifir2bir=temp;
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0';
	adet[0]=metelik+'0';adet[1]='-';

	LCD_KOMUT_ISLE(0x80+(11));
	STRING_ISLE(remp);
	LCD_KOMUT_ISLE(0xC0);
	STRING_ISLE(adet);
	STRING_ISLE("Metelik");
	//SysCtlDelay(SysCtlClockGet());
    for(int i=0;i<6000000;i++){
    	if((GPIO_PORTA_DATA_R & 0b00010000)==16)
    		goto Loop;
    }

	//---------------------------------------------------------------
	LCD_KOMUT_ISLE(0x01);

	kurusluk=sayi;
	temp=sayi-kurusluk*1;
	sayi=sayi%1;

	on=temp/1000; temp=temp-(on*1000);
	bir=temp/100; temp=temp-(bir*100);
	sifirbir=temp/10; temp=temp-(sifirbir*10);
	sifir2bir=temp;
	remp[0]=on+'0';remp[1]=bir+'0';remp[2]='.';remp[3]=sifirbir+'0';remp[4]=sifir2bir+'0';
	adet[0]=kurusluk+'0';adet[1]='-';

	LCD_KOMUT_ISLE(0x80+(11));
	STRING_ISLE(remp);
	LCD_KOMUT_ISLE(0xC0);
	STRING_ISLE(adet);
	STRING_ISLE("Kurusluk");
	SysCtlDelay(SysCtlClockGet());

	//---------------------------------------------------------------
Loop:
	 str[0]=0+'0'; str[1]=0+'0'; str[2]=0+'0'; str[3]=0+'0'; str[4]=0+'0';
	 	reset=0;
	 	digit1=0;digit2=0;digit3=0;digit4=0;
	 	x=0;x1=0;y=0;y1=0;z=0;z1=0;t=0;t1=0;k=0;k1=0;
	 	digit_son=0;
	    delay=0;
	    x=0;x1=0;y=0;y1=0;z=0;z1=0;t=0;t1=0;k=0;k1=0;
	    degis=16;
	    LCD_KOMUT_ISLE(0x01);




}

void LCD_BASLAT() {

        SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
        GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, 0xFF);

        SysCtlDelay(5000);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0,  0 );


       GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,  0x30 ); //8 BİT 1 SATIR 5X8 NOKTA
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);

        SysCtlDelay(5000);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,  0x30 ); //8 BİT 1 SATIR 5X8 NOKTA
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0);

        SysCtlDelay(5000);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,  0x30 ); //8 BİT 1 SATIR 5X8 NOKTA
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1, 0);

        SysCtlDelay(5000);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,  0x20 ); //4 BİT 1 SATIR 5X8 NOKTA
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE,GPIO_PIN_1, 0);

        SysCtlDelay(5000);

        LCD_KOMUT_ISLE(0x28); // 5X8 2 SATIR 4 BİT
        LCD_KOMUT_ISLE(0x0C); // DİSPLAY AÇIK İŞARETÇİ KAPALI
        LCD_KOMUT_ISLE(0x01); // EKRANI TEMİZLE
}
void LCD_KOMUT_ISLE(unsigned char c) {
        // 8 BİT GÖNDERİLDİĞİ İÇİN 0xF0 VE 0x0F İLE MASKELENDİ
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,(c & 0xf0) ); // UPPER NİBBLE OKUNDU
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0); // KOMUT İÇİN RS 0 YAPILDI
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1); // DATALARI DATA PİNLERİNE GÖNDERMESİ İÇİN E HİGHDAN LOW A GEÇTİ
        SysCtlDelay(50000);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0); //DATALARI ALMASI İÇİN TEKRAR 0 YAPILDI

        SysCtlDelay(50000);

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,(c & 0x0f) << 4 ); // LOW NİBBLE OKUNDU
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0); // KOMUT İÇİN RS 0 YAPILDI
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1); // DATALARI DATA PİNLERİNE GÖNDERMESİ İÇİN E HİGHDAN LOW A GEÇTİ
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0); //DATALARI ALMASI İÇİN TEKRAR 0 YAPILDI

        SysCtlDelay(50000);

}

void LCD_VERI_ISLE(unsigned char d) {

        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, (d & 0xf0) ); // UPPER NİBBLE OKUNDU
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 1); // VERİ YAZDIRMAK İÇİN RS 1 YAPILDI
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1);  // DATALARI DATA PİNLERİNE GÖNDERMESİ İÇİN E HİGHDAN LOW A GEÇTİ
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1,0); //DATALARI ALMASI İÇİN TEKRAR 0 YAPILDI

        SysCtlDelay(50000);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4| GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, (d & 0x0f) << 4 ); // LOW NİBBLE OKUNDU
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 1); // VERİ YAZDIRMAK İÇİN RS 1 YAPILDI
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, GPIO_PIN_1); // DATALARI DATA PİNLERİNE GÖNDERMESİ İÇİN E HİGHDAN LOW A GEÇTİ
        SysCtlDelay(10);
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0); //DATALARI ALMASI İÇİN TEKRAR 0 YAPILDI

        SysCtlDelay(50000);

}
// Gönderilen karakterin adresini tutar
void KARAKTER_ISLE( char* s){
	  LCD_VERI_ISLE(*s);
}
// Gönderilen Stringin adresini tutar ve adresteki karakteri yazdırır taa ki null olana kadar
void STRING_ISLE(char* s){
	while(*s)
		LCD_VERI_ISLE(*s++);


}
























