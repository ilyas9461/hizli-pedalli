#include "dfPlayer.h"

//Bu haliyle SD kart ana klasörüne mp3 yada vaw dosya atýlmasý
// fark etmiyor her ikisinide çalýþtýrýyor.. Ýsimler ayný olmayacak...


void sendCommand(mbyte Command, mbyte Param1, mbyte Param2) {
    
  //  disable_interrupts(GLOBAL);
// Calculate the checksum
  mbyte commandBuffer[10];
  unsigned int16 checkSum = -(versionByte + dataLength + Command + infoReq + Param1 + Param2);
  mbyte low_cSum=make8(checkSum,0);
  mbyte high_cSum=make8(checkSum,1);
  // Construct the command line
  //mbyte commandBuffer[10] ={startByte, versionByte, dataLength, Command, infoReq, Param1, Param2,high_cSum,low_cSum, endByte};
  
  commandBuffer[0]=startByte;
  commandBuffer[1]=versionByte;
  commandBuffer[2]=dataLength;
  commandBuffer[3]=Command;
  commandBuffer[4]=infoReq;
  commandBuffer[5]=Param1;
  commandBuffer[6]=Param2;
  commandBuffer[7]=high_cSum;
  commandBuffer[8]=low_cSum;
  commandBuffer[9]=endByte;
//
  for (int cnt = 0; cnt < 10; cnt++) {
    //mp3.write(commandBuffer[cnt]);
      //putc(commandBuffer[cnt]);
      fputc(commandBuffer[cnt],dfPlayerSerial);
  }

  // Delay needed between successive commands
  delay_ms(30);
 // enable_interrupts(GLOBAL);
  
}

// Set volume (otherwise full blast!) command code 0x06 followed by high byte / low byte
// sendCommand(0x06, 0, 30);
void mp3_set_volume (char vol){
    sendCommand(0x06, 0, vol);
}
// Equaliser setting
//  sendCommand(0x07, 0, 5);
void mp3_set_equalizer(char eq){
    sendCommand(0x07, 0, eq);
}
// Specify track to play (0 = first track)
//  sendCommand(0x03, 0, 0);
void mp3_play_physical(unsigned int16 fileNum){
  sendCommand(0x03, make8(fileNum,1), make8(fileNum,0));  
  //sendCommand(0x03, 0, 1);
}
//
void mp3_single_loop (unsigned int16 fileNum) {
	sendCommand(0x08, make8(fileNum,1), make8(fileNum,0));
}  
void mp3_enableLoopAll(){
    sendCommand(0x11, 0x00,0x01 );        
}
void mp3_disbleLoopAll(){
    sendCommand(0x11, 0x00,0x00 );        
}
// Play
  //sendCommand(0x0D, 0, 0);
void mp3_play (){
   sendCommand(0x0D, 0, 0); 
}

void mp3_play (unsigned int16 fileNum) 
{
	//mp3_send_cmd (0x12, num);
    sendCommand(0x12, make8(fileNum,1), make8(fileNum,0));  
}
void mp3_stop(){
   sendCommand(0x16, 0, 0);     
}
void mp3_reset(){
    sendCommand(0x0C, 0, 0); 
}

void mp3_get_error () {
    sendCommand(0x40, 0, 0); 
    
}
void mp3_getData(*dizi){        //kilitlendi .....
    char i=0;
    
    while(i<10){
        if(kbhit(dfPlayerSerial)){
            dizi[i]=fgetc(dfPlayerSerial);
            i++;
        }  
    }
}

void mp3_DataSeriGonder(*dizi,char len){ 
   int k=0;  
   fprintf(usbSerial,"\r\n");
   for( k=0;k<len;k++){
           fprintf(usbSerial,"%x ",dizi[k]);    
   } 
   fprintf(usbSerial,"\r\n");
}
void mp3_baslangic(unsigned int ses){
    
    delay_ms(500); /// Mp3  player donaným kurulumndan belli bir süre sonra çalýþtýrmalý....
    mp3_set_equalizer(5);
    delay_ms(100);
    //mp3_set_volume(15);//48 e kadar olabiliyor denendi...
    //dfPlayer_ses= map(read_adc(),0,1023,0,dfPlayerMaxSes);
    mp3_stop();
    mp3_set_volume (ses);       //20
    delay_ms(100);
    mp3_enableLoopAll();
   // mp3_single_loop(18);
   // mp3_play(18);
   // delay_ms(500);
    
}