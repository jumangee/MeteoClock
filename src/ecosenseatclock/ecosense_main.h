/**
 * Display process
 * for Processy Firmware Framework
 */

#ifndef _ECOSENSEATCLOCK_MAIN_H
#define _ECOSENSEATCLOCK_MAIN_H

#include "processy.h"
#include "processy_process.h"

#include "ecosenseatclock.h"
#include "ecosense_messages.h"

#include <Arduino.h>

#define INCLUDE_SCROLLING 0

#include <SSD1306Ascii.h>

#undef OPTIMIZE_I2C
#define OPTIMIZE_I2C 0
#undef INCLUDE_SCROLLING
#define INCLUDE_SCROLLING 0

#include <SSD1306AsciiWire.h>

#include <math.h>
#include "ecosenseicons.h"
#include "stuff.h"

#define OLED_ADDR   0x3C

//#define MAIN_FONT System5x7
//#define MAIN_FONT Stang5x7
#define MAIN_FONT fixednums7x15
#define ICONS_FONT ecosenseicons

#define SPRITE_GAS_CH4	0x2A
#define SPRITE_GAS_H2S	0x24
#define SPRITE_GAS_CO2	0x2B
#define SPRITE_OK		0x2C
#define SPRITE_WARNING	0x21
#define SPRITE_DANGER	0x2E
#define SPRITE_SPC		0x20
#define SPRITE_WIFI		0x22
#define SPRITE_ENV_TEMP	0x28
#define SPRITE_ENV_C	0x29
#define SPRITE_ENV_HUM	0x23
#define SPRITE_ENV_PRES	0x2D

#define SCREENROW_GAS_H2S 2
#define SCREENROW_GAS_CH4 4
#define SCREENROW_GAS_CO2 6

#define USE_WARNING_LIGHT 1
#if USE_WARNING_LIGHT == 1
	#define WARNLED_R_PIN A2
	#define WARNLED_G_PIN A1
	#define WARNLED_B_PIN 17

	const byte warnLightGradient[11] PROGMEM = {0, 89, 124, 149, 170, 188, 203, 218, 231, 243, 255};
#endif

//#define PHOTORESISTOR_PIN A3

class MainProcess: public IFirmwareProcess {
	private:
		SSD1306AsciiWire oled;
		bool	updateScreen;

		int		temp;
		byte	humidity;
		int		pressure;
		byte	gasH2S;
		byte	gasCH4;

		bool	clocktick;

	public:
		//@implement
		//@include <SSD1306AsciiWire.h>
		MainProcess(uint16_t pId, IProcessMessage* msg): IFirmwareProcess(pId, msg) {
			TRACELNF("MainProcess::start");

			#if USE_WARNING_LIGHT == 1
			pinMode(WARNLED_R_PIN, OUTPUT);
			pinMode(WARNLED_G_PIN, OUTPUT);
			pinMode(WARNLED_B_PIN, OUTPUT);
			#endif
			
			Wire.setClock(400000L);
			oled.begin(&Adafruit128x64, OLED_ADDR);
			oled.clear();
			oled.setFont(MAIN_FONT);
			//oled.print(F("CTAPT..."));	// no chars in font ((

			temp = 0;

			clocktick = true;
			gasH2S = 0;
			gasCH4 = 0;
			temp = 0;
			humidity = 0;
			pressure = 0;

			this->updateScreen = false;
		}

		//@implement
		static IFirmwareProcess* factory(uint16_t pId, IProcessMessage* msg) {
			TRACELNF("MainProcess::factory");
			return new MainProcess(pId, msg);
		}

		//@implement
		~MainProcess() {
			// stop process
			TRACELNF("MainProcess::stop");
		}

		//@implement
		void update(unsigned long ms) {
			// oled contrast auto adjustment
			#ifdef PHOTORESISTOR_PIN
				oled.setContrast(100);
			#endif

			if (this->updateScreen) {
				this->render();
				this->updateScreen = false;
			}

			/*if (this->gasH2S > 0) {
				printGasInfo(SPRITE_GAS_H2S, SCREENROW_GAS_H2S, this->gasH2S);
			}
			if (this->gasCH4 > 0) {
				printGasInfo(SPRITE_GAS_CH4, SCREENROW_GAS_CH4, this->gasCH4);
			}*/

			this->pause(10);
		}

		void prn2X(String s) {
			oled.set2X();
			oled.print(s);
			oled.set1X();
		}

		bool prnEnvData(char spr, byte row, byte value) {
			if (value > 0) {
				oled.setFont(ICONS_FONT);
				oled.setCursor(0, row);
				prn(spr);
				prn(SPRITE_SPC);
				oled.setFont(MAIN_FONT);
				oled.print(value);
				///prn2X(S(value));
				//oled.set2X();
				//oled.print(value);
				/*} else {
					oled.print(F("-   "));
				}*/
				//oled.set1X();
				return true;
			}
			return false;
		}

		//@implement
		//@include <SSD1306AsciiWire.h>
		void render() {
			if (prnEnvData(SPRITE_ENV_TEMP, 2, this->temp)) {
				oled.setFont(ICONS_FONT);
				prn(SPRITE_ENV_C);
				prn(SPRITE_SPC);
			}

			if (prnEnvData(SPRITE_ENV_HUM, 4, this->humidity)) {
				oled.setFont(ICONS_FONT);
				prn(SPRITE_ENV_C);prn(SPRITE_ENV_C);
				oled.print(0x25);	//%
				prn(SPRITE_SPC);
			}

			if (prnEnvData(SPRITE_ENV_PRES, 6, this->pressure)) {
				prn(SPRITE_SPC);
			}
		}

		//@implement
		bool handleMessage(IProcessMessage* msg) {
			switch (msg->getType())
			{
				case ENVDATA_MESSAGE: {
					this->handleEnvDataMsg((EnvDataMessage*)msg);
					break;
				}
				case AIRQUALITY_MESSAGE: {
					this->handleAirQualityMsg((AirQualityMsg*)msg);
					break;
				}
				case CURTIME_MESSAGE: {
					this->handleTimeMsg((CurrentTimeMsg*)msg);
					return true; // dispose
				}
				case WIFISTATE_MESSAGE: {
					this->handleWifiMsg((WiFiStateMsg*)msg);
					return true; // dispose
				}
			}
			return false;
		}

		//@implement
		void handleEnvDataMsg(EnvDataMessage* msg) {
			if (msg->isActive()) {
				/*String s = SF("BME data: T=");
				s += msg->getTemp();
				s += F("C, Hum=");
				s += msg->getHumidity();
				s += F("%, Pres=");
				s += msg->getPressure();
				TRACELN(s);*/
				if (this->temp != msg->getTemp()) {
					this->temp = msg->getTemp();
					this->updateScreen = true;
				}
				if (this->humidity != msg->getHumidity()) {
					this->humidity = msg->getHumidity();
					this->updateScreen = true;
				}
				if (this->pressure != msg->getPressure()) {
					this->pressure = msg->getPressure();
					this->updateScreen = true;
				}
			}
		}

		#if USE_WARNING_LIGHT == 1
		//@include "stuff.h"
		void updateWarningLight() {
			byte warningLevel = 0;
			warningLevel += this->gasH2S;
			warningLevel += this->gasCH4;
			//warningLevel += this->gasCO2;
			warningLevel += (this->temp > 18 && this->temp < 35) ? 0 : ((this->temp > 40 || this->temp < 12) ? 2 : 1);
			if (humidity > 0)
				warningLevel += (this->humidity > 19 && this->humidity < 50) ? 0 : 1;
			
			/*for (warningLevel = 0; warningLevel < 11; warningLevel++) {
			rgbColor warnLed = warningLevel > 0 ? (rgbColor::ColorMix(rgbColor(0, 255, 0), rgbColor(255, 0, 0),  warningLevel / 10 )) : rgbColor(0, 255, 0);
			TRACEF("ColorMix result: r=")
			TRACE(warnLed.r)
			TRACEF(", g=")
			TRACE(warnLed.g)
			TRACEF(", b=")
			TRACELN(warnLed.b)
			TRACELNF("===")
			}*/

			analogWrite(WARNLED_R_PIN, warnLightGradient[warningLevel]);
			analogWrite(WARNLED_G_PIN, warnLightGradient[11 - warningLevel]);
			analogWrite(WARNLED_B_PIN, 0);
		}
		#endif

		//@implement
		void handleTimeMsg(CurrentTimeMsg* msg) {
			clocktick = !clocktick;

			if (!msg->getHrs() && !msg->getMins()) {
				return;
			}
			oled.setFont(MAIN_FONT);
			oled.setCursor(0, 0);
			//prn2X(msg->getTime());
			oled.print(msg->getTime());
			if (!msg->getDots()) {
				oled.clearField(15, 0, 1);
			}
			//oled.set2X();
			//oled.clearField(0, 0, 5);
			//oled.print(msg->getTime());
			//oled.set1X();

			#if USE_WARNING_LIGHT == 1
				this->updateWarningLight();
			#endif
		}

		void prn(char c) {
			oled.print(c);
		}

		//@implement
		void printGasInfo(char g, byte row, byte quality) {
			if (quality > 0) {
				oled.setFont(MAIN_FONT);
				if (quality > 1) {
					oled.setInvertMode(clocktick);
				} else {
					oled.setInvertMode(false);
				}
				oled.setCursor(90, row);
				oled.setFont(ICONS_FONT);
				prn(g);
				prn(SPRITE_SPC);
			
				oled.setCol(108);
				if (quality == 1) {
					prn(SPRITE_OK);
				} else if (quality == 2) {
					prn(SPRITE_WARNING);
				} else {
					prn(SPRITE_DANGER);
				}
				prn(SPRITE_SPC);
				oled.setInvertMode(false);
			}
		}

		//@implement
		void handleAirQualityMsg(AirQualityMsg* msg) {
			switch (msg->gasType())
			{
				case H2S: {
					if (this->gasH2S != msg->getQuality()) {
						this->gasH2S = msg->getQuality();
						printGasInfo(SPRITE_GAS_H2S, SCREENROW_GAS_H2S, this->gasH2S);
					}
					return;
				}
				case CH4: {
					if (this->gasCH4 != msg->getQuality()) {
						this->gasCH4 = msg->getQuality();
						printGasInfo(SPRITE_GAS_CH4, SCREENROW_GAS_CH4, this->gasCH4);
					}
					return;
				}
				/*case CO2: {
					if (this->gasCO2 != msg->getQuality()) {
						this->gasCO2 = msg->getQuality();
						printGasInfo(SPRITE_GAS_CO2, SCREENROW_GAS_CO2, this->gasCO2);
					}
					return;
				}*/
			}
		}

		//@implement
		void handleWifiMsg(WiFiStateMsg* msg) {
			oled.setFont(ICONS_FONT);
			oled.setCursor(100, 0);
			oled.print(F("   "));
			oled.setCursor(108, 0);
			if (msg->isActive()) {
				prn(SPRITE_WIFI);
				prn(SPRITE_SPC);
			}
		}
};

#endif
