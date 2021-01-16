/**
 * MQ-4 process
 * for Processy Firmware Framework
 */
 
#ifndef _MQ4_SENSOR_PROCESS_H
#define _MQ4_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#include "meteo.h"
#include "mqsensor_process.h"

//#include <Arduino.h>

#define MQ4_ANALOG_PIN A2
//15
#define MQ4_DIGITAL_PIN 8

#define READINGS_PER_RESULT 50

#define MQ136_RL 2.3
#define MQ136_FRESH_RO 1

// curve params calculated using article: https://circuitdigest.com/microcontroller-projects/arduino-mq137-ammonia-sensor
//const float CH4_CURVE[2] PROGMEM = {-0.2840, -0.9672};
//const float H2_CURVE[2] PROGMEM = {-1.24, 0.7562};

class MQ4SensorProcess: public MQSensorProcess {
	public:
		//@implement
		//@include "meteo_cfg.h"
		MQ4SensorProcess(int pId, IProcessMessage* msg): MQSensorProcess(pId, msg) {
			pinMode(MQ4_ANALOG_PIN, INPUT);
			pinMode(MQ4_DIGITAL_PIN, INPUT) ;

			TRACELNF("MQ4SensorProcess::init");
		}

		//@implement
		static IFirmwareProcess* factory(int pId, IProcessMessage* msg) {
			TRACELNF("MQ4SensorProcess::factory");
			return new MQ4SensorProcess(pId, msg);
		}

		//@implement
		//@include "processy_cfg.h"
		//@include "meteo_cfg.h"
		//@include "mqsensor_process.h"
		void update(unsigned long ms) {
			if (!readingsDone(MQ4_ANALOG_PIN, READINGS_PER_RESULT)) {
				return;
			}

			{
				TRACEF("[ MQ-4 ] analog=");
				TRACE(this->getValue());
				//TRACEF(", dig=");
				//TRACE(this->mq136dig);
				TRACEF(", V=");
				TRACE( this->getVoltage() );
				TRACEF(", instant=");
				TRACE( this->instantValue(MQ4_ANALOG_PIN) );
				TRACEF(", Dig=");
				TRACELN( digitalRead(MQ4_DIGITAL_PIN) );
			}

			this->getHost()->sendMessage(new AirQualityMsg(CH4, this->getQuality(.4), this->getVoltage()));

			this->pause(ENVSENSORS_TIMEOUT);
		}

};

#endif
