/**
 * MQ sensors parent process
 * for Processy Firmware Framework
 * MQ sensors parent process
 * 
 * to use, learn MQ sensors article:
 * https://circuitdigest.com/microcontroller-projects/arduino-mq137-ammonia-sensor
 * 
 * m-value:
 * [log(ppm1 / "RsRo1") - log(ppm2 / "RsRo2")], 
 * 
 * b-value:
 * [log("RsRo3") - m * log(ppm3)]
 * 
 * 
 * js:
 * var lg = Math.log; var m = (lg(0.41) - lg(0.19))/(lg(4) - lg(60)); console.log( "m=" + m ); console.log( "b=" + ( (lg(0.17) - m * lg(17)) ) )
 * 
 * 
 * curve:
 * [m-value, b-value]
 */

#ifndef _MQ_SENSOR_PROCESS_H
#define _MQ_SENSOR_PROCESS_H

#include "processy.h"
#include "processy_process.h"

#define PREHEAT_TIME 45000

class MQSensorProcess: public IFirmwareProcess {
	private:
		byte readingsCount;
        uint16_t value;
		uint32_t startTime;
		bool preHeated;

	public:
		//@implement
		//@include "meteo_cfg.h"
		MQSensorProcess(int pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
			readingsCount = 0;
            value = 0;
			preHeated = false;
			startTime = millis();

			this->pause(PREHEAT_TIME);

			TRACELNF("MQSensorProcess::init");
		}

        uint16_t instantValue(byte pin) {
            return analogRead(pin);
        }

        uint16_t getValue() {
            return value;
        }

        /**
         * Calc V from analog value
         */
        float getVoltage(float Vfull = 5) {
            return (value + .5) * (5.0 / 1023.0);
        }

		/*float getRatio(float airR0, float Rl, float V = 5) {
			float Vrl = this->getVoltage(V);

			float Rs0 = ((V/Vrl)-1) * Rl;

			float rs = ((V * Rl) / Vrl) - Rl;

			float actualR0 = Rs0 / airR0;

			//TRACEF("actRo = ");
			//TRACELN(actualR0);

			return rs / actualR0;
		}*/

		bool readingsDone(byte pin, byte countPerResult) {
			if (!preHeated) {
				if (PREHEAT_TIME > (millis() - startTime)) {
					return false;
				}
				preHeated = true;
			}

			if (readingsCount >= countPerResult) {
				readingsCount = 0;
				value = 0;
			}
	
			this->value += this->instantValue(pin);
			readingsCount++;

			if (readingsCount >= countPerResult) {
				value = round(value / float(readingsCount));
				return true;
			}
			return false;
		}

		//@implement
		//@include <Arduino.h>
		~MQSensorProcess() {
			// stop process
			TRACELNF("MQSensorProcess::stop");
		}

		/*float cosh(float x) {
			return ( expf(x) + expf(-x) ) / 2;
		}*/

		byte getQuality(float k = .6) {
			return exp((getVoltage()-5)*k)*127;
		}

		/*int MQGetConcentration(float airR0, float Rl, const float *pcurve, float V = 5) {
            return pow(10, ( double(log10( this->getRatio(airR0, Rl, V) ) - pcurve[1]) / pcurve[0]));
			//return (double)(pcurve[0] * pow(((double)this->getRatio(airR0, Rl, V)), pcurve[1]));
		}*/
};

#endif
