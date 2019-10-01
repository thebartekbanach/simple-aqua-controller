#define I2C_SCANNER_ENABLED false

#if I2C_SCANNER_ENABLED == true
    #define DIAGNOSTICS_MODE

	#include <Wire.h>
		
		
	void setup()
	{
		Wire.begin();
		
		SerialUSB.begin(9600);
		SerialUSB.println("\nI2C Scanner");
	}
		
		
	void loop()
	{
		byte error, address;
		int nDevices;
		
		SerialUSB.println("Scanning...");
		
		nDevices = 0;
		for (address = 1; address < 127; address++)
		{
			Wire.beginTransmission(address);
			error = Wire.endTransmission();
			
			if (error == 0)
			{
				SerialUSB.print("I2C device found at address 0x");
				if (address<16)
				SerialUSB.print("0");
				SerialUSB.print(address,HEX);
				SerialUSB.println("  !");

				nDevices++;
			}
			
			else if (error==4)
			{
				SerialUSB.print("Unknown error at address 0x");
				if (address<16)
				SerialUSB.print("0");
				SerialUSB.println(address,HEX);
			}    
		}

		if (nDevices == 0)
			SerialUSB.println("No I2C devices found\n");
		else SerialUSB.println("done\n");
		
		delay(2000);
	}

#endif