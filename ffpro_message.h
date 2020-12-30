/*
* Firmware Process Control Framework (FFPro)
*
*/
 
#ifndef _FIRMWARE_FRAMEWORK_MESSAGE_H
#define _FIRMWARE_FRAMEWORK_MESSAGE_H

class IFirmwareProcess;

#include "ffpro_process.h"

class IProcessMessage {
	public:
		IProcessMessage(IFirmwareProcess* from, String type) {
			this->sender = from;
			this->type = type;
		}
		
		String getSenderId();
		
		bool isSenderId(String compareTo);
		
		IFirmwareProcess* getSender() {
			return sender;
		}
		
		bool isAnonymous() {
			return getSender() == NULL;
		}
		
		String getType() {
			return this->type;
		}

	private:
		IFirmwareProcess* sender;
		String type;
};

#endif