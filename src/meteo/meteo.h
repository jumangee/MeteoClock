/*
* MeteClock Firmware
*/


///////////////////////////////////////////////////
// PROCESSES NAMES
#define PRC_MAIN "PRC_MAIN"
#define PRC_SENSORS "PRC_SENSORS"
//#define PRC_I2CSCANNER "PRC_I2CSCANNER"
// END: NAMES
//////////////////////////////////////////////////

#ifndef _METEOCLOCK_FFPRO_H
#define _METEOCLOCK_FFPRO_H

#include "processy_cfg.h"
#include "processy.h"

#include "meteo_process.h"
#include "meteo_main.h"
//#include "meteo_sensors.h"
//#include "meteo_i2c_scanner.h"

#include "stuff.h"

#define FACTORY_DEFAULT(name, className) this->registerFactory(name, [](String id, IProcessMessage* msg){return new className(id, msg);}, true);
#define FACTORY(name, className) this->registerFactory(name, [](String id, IProcessMessage* msg){return new className(id, msg);});

class MeteoClockFirmware: public IFirmware {

	//@implement
	//@include <Arduino.h>
	MeteoClockFirmware(): IFirmware() {
		#ifdef DEBUG_SERIAL
		Serial.println("START");
		#endif
	};

	public:

		//@implement
		//@include "processy_cfg.h"
		//!@include "meteo_main.h"
		void init() {
			FACTORY_DEFAULT(PRC_MAIN, MainProcess)
			//FACTORY(PRC_SENSORS, EnvironmentSensorsProcess)
			//FACTORY_DEFAULT(PRC_I2CSCANNER, i2cPortScannerProcess)
			//this->registerFactory(PRC_MAIN, [](String id, IProcessMessage* msg){return new MainProcess(id, msg);}, true);
			
			//this->addProcess(PRC_MAIN);
			//this->addProcess(PRC_SENSORS);
		}

		//@implement
		//@include <Arduino.h>
		void log(String msg) {
			#ifdef DEBUG_SERIAL
			Serial.println(msg);
			#endif
		}

		//@implement
		static IFirmware* get() {
			if (IFirmware::instance == NULL) {
        		Serial.println("new MeteoClockFirmware");
				IFirmware::instance = new MeteoClockFirmware();
				((MeteoClockFirmware*)IFirmware::instance)->init();
			}
			return IFirmware::instance;
		}

};

#endif
