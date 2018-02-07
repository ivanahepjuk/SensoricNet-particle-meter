187 3 39 66
245 238 58 66
175 62 210 66

pm1: 41.753643
pm25: 46.733356
pm10: 105.122429

3 30 60 66
219 207 83 66
122 70 86 66

pm1: 47.029308
pm25: 52.952984
pm10: 53.568825

https://github.com/ksarkies/ARM-Ports/blob/master/test-libopencm3-stm32f1/iwdg-et-stm32f103.c

MEASURED VALUES:
temp 	[°C]: 		24.31
pm 1 	[ug/m3]: 	20.76
pm 2.5 	[ug/m3]: 	25.85
pm 10 	[ug/m3]: 	29.52

MEASURED VALUES:
temp [°C]: 24.32
pm 1 [ug/m3]: 24.43
pm 2.5 [ug/m3]: 28.95
pm 10 [ug/m3]: 29.05


/*

RN2483 1.0.3 Mar 22 2017 06:00:42
invalid_param
invalid_param
0004A30B00222137
RN2483 1.0.3 Mar 22 2017 06:00:42
radio set pwr
invalid_param
radio set

invalid_param
ge
invalid_param
radio get pwr
1
radio set pwr 14
ok
radio get pwr
14
radio get mod
lora
radio get freq
868100000
radio get sg
invalid_param
radio get sf
sf12
mac set deveui 0004A30B00222137
ok
mac set appeui 70B3D57ED00082D2
ok

invalid_param
mac set appkey D94AC6F27881D3505F3E595B69472898
ok
mac save
ok
mac join otaa
ok
accepted
mac get status
00000401
mac get devaddr
260127E9
mac tx uncnf 1 AABABBB
ok
mac_tx_ok


*/

//double num = 123412341234.123456789; 
		//char output[10];

		//sprintf(output, 10, "%f", 12.2278889);//temp_readout_BME280());



/*
     //686f646e6f74792c626f783d41647273706163682068756d3d
     //hodnoty,box=Adrspach hum=
     //

     char hodnota[20]={0};
     char hexadec[20];
     int x = 0;
     
     //Hum
     float hum = 12.121;//BMEsensor.getHumidity();
     sprintf(hodnota, "%f.2", hum);

     while(hodnota[x] != '\0'){
         charToHex(hodnota[x], hexadec);
         strcat(zprava, hexadec);   
     }
     
     //pricist znaky  ,temp=
     //2c74656d703d
     strcat(zprava, "2c74656d703d");   
     
     //temp
     float temp = 12.121;//BMEsensor.getTemperature();
     sprintf(hodnota, "%f.2", temp);

     while(hodnota[x] != '\0'){
         charToHex(hodnota[x], hexadec);
         strcat(zprava, hexadec);   
     }
     
     //pricist znaky  ,pres=
     //2c707265733d
     strcat(zprava, "2c707265733d");
     
     //press
     float press = 12.121;//BMEsensor.getTemperature();
     sprintf(hodnota, "%f.2", press);

     while(hodnota[x] != '\0'){
         charToHex(hodnota[x], hexadec);
         strcat(zprava, hexadec);   
     }   
     
     //pricist konec  \r\n
     //5c725c6e
     strcat(zprava, "5c725c6e");

////////////////////////////////////////////////////////
*/
