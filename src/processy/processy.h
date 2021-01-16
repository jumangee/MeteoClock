/**
 * Processy Firmware Framework Core
 */

#ifndef _FIRMWARE_FRAMEWORK_H
#define _FIRMWARE_FRAMEWORK_H

#include "LinkedList/LinkedList.h"

#include "processy_cfg.h"
#include "processy_process.h"
#include "processy_message.h"
//#include <WString.h>
#include <math.h>

#define FACTORY(name, className) ProcessFactoryReg(name, &className::factory)

typedef IFirmwareProcess* (*ProcessFactory)(int, IProcessMessage*);

class ProcessFactoryReg {
	public:
		int id;
		ProcessFactory factory;

		ProcessFactoryReg(int pId, ProcessFactory f) {
			this->id = pId;
			this->factory = f;
		}
};

class IFirmware {
	protected:
		static IFirmware* instance;

		//@implement
		//@include <Arduino.h>
		IFirmware() {
			this->processList = LinkedList<IFirmwareProcess*>();

			#ifdef DEBUG_PRO_MS
			this->resetMsDebugTimer(millis());
			#endif
		}

	public:
		static IFirmware* get() {
			return IFirmware::instance;
		}

		virtual ProcessFactory getFactory(int pId) = 0;

		//@implement
		IFirmwareProcess* getProcess(int pId) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				return this->processList.get(pos);
			}
			return NULL;
		}

		//@implement
		void stopProcess(int pId) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				this->processList.remove(pos);
			}
		}

		//@implement
		void pauseProcess(int pId, unsigned long pauseTime) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				//IFirmwareProcess *process = ;
				this->processList.get(pos)->pause(pauseTime);
			}
		}

		//@implement
		void unPauseProcess(int pId) {
			int pos = this->findProcess(pId);
			if (pos > -1) {
				IFirmwareProcess *process = this->processList.get(pos);
				process->unPause();
			}
		}

		//@implement
		void stopAll() {
			this->processList.clear();
		}

		//@implement
		void soloProcess(int pId) {
			this->stopAll();
			this->addProcess(pId);
		}

		//@implement
		void sendMessage(IProcessMessage* msg) {
			if (msg == NULL) return;

			for (int i = 0; i < processList.size(); i++) {
				if (this->processList.get(i)->handleMessage(msg) == true) {	// message processing stop
					break;
				}
			}
			delete msg;
		}

		//@implement
		void addProcess(int pId) {
			this->addProcess(pId, NULL);
		}

		//@implement
		//@include "processy_cfg.h"
		void run() {
			if (this->processList.size() == 0) {
				#ifdef DEFAULT_PROCESS
					this->addProcess(F(DEFAULT_PROCESS));
				#else
					TRACELNF("NOTHING TO DO!")
					return;
				#endif
			}
			unsigned long curTime = millis();
			if (this->update(curTime)) {	// true - auto process, false - manual process
				curTime = millis();
				for (int i = 0; i < this->processList.size(); i++) {
					//IFirmwareProcess* process = ;
					curTime = this->processList.get(i)->run(curTime);
				}
			}
			#ifdef DEBUG_PRO_MS
			unsigned long dT = curTime - msDebugTimerStart;
			if (dT >= DEBUG_PRO_PERIOD_MS) {
				// call report
				handlerProcessDebugTimer(dT);
				this->resetMsDebugTimer(millis());
			}
			#else
				handlerProcessDebugTimer(0);
			#endif
		}

		//@implement
		void addProcess(int pId, IProcessMessage* msg) {
			if (this->findProcess(pId) > -1) {
				return;	// only 1 instance of process
			}
			IFirmwareProcess* newProcess = this->createProcess(pId, msg);
			if (msg != NULL) {
				delete msg;
			}
			if (newProcess == NULL) {
        		TRACELNF("IFirmware::addProcess//!newProcess")
				return;
			}

      		TRACELNF("IFirmware::addProcess//newProcess")
			this->processList.add(newProcess);
		}

	protected:
		LinkedList<IFirmwareProcess*> processList;

		#ifdef DEBUG_PRO_MS
		unsigned long msDebugTimerStart;

		void resetMsDebugTimer(unsigned long val) {
			this->msDebugTimerStart = val;
		}
		#endif

		//@implement
		//*** OVERRIDE THIS ***/
		//@include "stuff.h"
		//@include "processy_cfg.h"
		//@include "MemoryFree.h"
		void handlerProcessDebugTimer(unsigned long dT) {
			#ifdef DEBUG_PRO_MS
			{
				String s = F("----- PROC SUMMARY (for ");
				s += dT;
				s += F("ms) -----");
				TRACELN(s);
			}
			for (int i = 0; i < this->processList.size(); i++) {
				IFirmwareProcess* process = processList.get(i);
				{
					String s = String(process->getId());
					s += F(": ");
					s += round((process->getUsedMs() * 100) / dT);
					s += F("% ");
					TRACELN(s + process->getUsedMs());
				}
				process->resetUsedMs();
			}
			#endif
			TRACEF("[!] MEMORY STATUS: ");
			{
				int free = freeMemory();
				this->sendMessage(new MemUsageMessage(free));
				TRACELN(free)
			}
			TRACELNF("--------------------------------------");
		}

		#ifdef DEBUG_PRO_MS
		void resetProcessMsTotal() {
			for (int i = 0; i < this->processList.size(); i++) {
				//IFirmwareProcess* process = ;
				this->processList.get(i)->resetUsedMs();
			}
		}
		#endif

		//@implement
		bool update(unsigned long ms) {
			return true;
		};

		//@implement
		IFirmwareProcess* createProcess(int pId, IProcessMessage* msg) {
			ProcessFactory factory = this->getFactory(pId);
			TRACELNF("IFirmware::createProcess//factory/!")
			if (factory != NULL) {
        		TRACELNF("IFirmware::createProcess//factory")
				IFirmwareProcess* t = factory(pId, msg);
				return t;
			}
			return NULL;
		}

		//@implement
		int findProcess(int pId) {
			for (int i = 0; i < this->processList.size(); i++) {
				//IFirmwareProcess* process = this->processList.get(i);
				if (this->processList.get(i)->isId(pId)) {
					return i;
				}
			}
			return -1;
		}

};

#endif
