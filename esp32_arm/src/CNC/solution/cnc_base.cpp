// #include "robot_hardware_base.h"
#include "cnc_base.h"
#include "MyLibs/MyFunctions.hpp"
#include "HardwareSerial.h"
#include "MyLibs/message_queue.h"
// void CncBase::FeedMessage(char* message, int length){
// 	Gcode gcode = Gcode(message);
// 	this->RunGcode(&gcode);
// }
void CncBase::SayHello(){
	Serial.println("[Debug] CncBase::SayHello()");
}

void CncBase::SpinOnce(){
	// Serial.print("[Debug] CncBase::SpinOnce() is entering.  Current state= ");
	// Serial.println(this->State);
	switch (this->State){
	case CncState::IDLE:
		break;
	case CncState::RUNNING_G4:
		this->__running_G4();
		break;
	case CncState::RUNNING_G1:
		this->_running_G1();
		break;
	case CncState::RUNNING_G28:
		this->_running_G28();
		break;
	default:
		Serial.print("[Warning] CncBase::SpinOnce() Unknown current state: ");
		// Serial.print(this->State);
		break;
	}

	// Serial.println("[Debug]( CncBase::SpinOnce() is finished.)");
	this->SpinOnce_BaseExit();
}

// Check gcode queue, if there is gcode to be run.
void CncBase::SpinOnce_BaseExit(){
	if (this->State != CncState::IDLE)
		return;
	if (this->_gcode_queue->BufferIsEmpty())
		return;

	MessageQueue::SingleMessage* message = this->_gcode_queue->FetchTailMessage();
	if (message == NULL){
		Serial.println("\n\n\n [Error] CncBase::SpinOnce_BaseExit() tail_message is null. \n\n ");
		return;
	}

	// this->_mq->SayHello("CncBase::SpinOnce_BaseExit()");
	if (true){
		Serial.print("\n[Info] CncBase::SpinOnce_BaseExit()  Going to run next gcode   ===> ");
		Serial.print(message->payload);
		Serial.println(" ");
	}
	
	// type convert   from char* to std::string
	char* p = &message->payload[0];
	std::string str = std::string(p);
	// feed std::string to Gcode constructor.
	Gcode gcode = Gcode(str);
	this->RunGcode(&gcode);
}

void CncBase::RunG4(Gcode* gcode){
	__g4_start_timestamp = micros();
	__g4_time_second = gcode->get_value('S');
}

void CncBase::__running_G4(){
	long delayed = (micros() - __g4_start_timestamp) / 1000 /1000;
	if (delayed >= __g4_time_second ){
		this->State = CncState::IDLE;
		return;
	}
}

void CncBase::RunGcode(Gcode* gcode){
	std::string result;
	// if ((gcode->get_command() == COMMU_OK) || (gcode->get_command() == COMMU_UNKNOWN_COMMAND)){
	//   Serial.print("RunGcode()   OK or Unknown");
	//   return;
	// }
	bool debug = false;

	if(gcode->has_g){
		char home_axis = '+';
		switch (gcode->g){
		case 28:
			// G28: Home
			this->State = CncState::RUNNING_G28;
			if (gcode->has_letter('X')) home_axis='X';
			if (gcode->has_letter('Y')) home_axis='Y';
			if (gcode->has_letter('Z')) home_axis='Z';
			if (gcode->has_letter('A')) home_axis='A';
			if (gcode->has_letter('B')) home_axis='B';
			if (gcode->has_letter('C')) home_axis='C';
			if (gcode->has_letter('W')) home_axis='W';
			this->_home_as_inverse_kinematic = false;
			if (gcode->has_letter('I')) this->_home_as_inverse_kinematic = true;
			if (home_axis == '+'){
				Serial.print("\n\n\n\n[Error] CncBase::RunGcode()  :");
				Serial.print(home_axis);

			}
			//TODO:  convert char to enum
			// this->ConvertToEnum(home_axis);
			this->RunG28(this->ConvertToEnum(home_axis));
			// this->commuDevice->OutputMessage(COMMU_OK);  For calble-bot-corner, it should be 'Unknown Command'
			break;

		case 1:
			// G1 Move
			//TODO:  1. put position to movement queue. called "plan" in smoothieware? 
			//       2. send out OK.
			//       3. Set status to busy.
			//       4. Start Moving.
			this->State = CncState::RUNNING_G1;
			this->RunG1(gcode);
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 4:
			// G4 Dwell, Pause for a period of time.
			this->State = CncState::RUNNING_G4;
			this->RunG4(gcode);
			break;
		case 6:
			this->RunG6(gcode);
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 90:
			// Absolute position
			this->is_absolute_position = true;
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		case 91:
			// Relative position
			this->is_absolute_position = false;
			// this->commuDevice->OutputMessage(COMMU_OK);
			break;
		// case 92:
			// Set Position     G92 X10 E90
			// break;
		default:
			break;
		}
	}else if(gcode->has_m){
		uint8_t p_value = 33;   //TODO: Make sure this is no harmful!
		uint8_t s_value = 0;
		EefAction action;
		switch (gcode->m){
		case 42:
			p_value =  gcode->get_value('P');
			s_value = gcode->get_value('S');
			this->RunM42(p_value, s_value);
		case 84:
			this->RunM84();
		case 114:
			// Get Current Position
			break;

		case 119:
			// Get Endstop Status
			result = GetHomeTrigerStateString();
			// this->commuDevice->OutputMessage(COMMU_OK);
			Serial.print(result.c_str());
			// this->commuDevice->WriteNotification(result);
			break;

		case 123:
			//M123 P=channel_index, S=Set EEF action			
			while (this->State != CncState::IDLE){
				this->SpinOnce();
			}
			p_value =  gcode->get_value('P');
			s_value = gcode->get_value('S');
			debug = true;
			if (debug){
				Serial.print("CncBase::RunGcode() For EEF_ACTION  M123 P= ");
				Serial.print(p_value);
				Serial.print("  S= ");
				Serial.print(s_value);
			}
			action = (EefAction)s_value;
			this->RunM123(p_value, s_value);
			break;

		case 280:
			// Set servo position  
			//	 	Pnnn Servo index
			// 		Snnn Angle or microseconds
			// Wait for all gcode, mcode is finished
			// Serial.println("M280 Started");
			while (this->State != CncState::IDLE){
				this->SpinOnce();
			}
			if (gcode->has_letter('P')) p_value = gcode->get_value('P');
			if (gcode->has_letter('S')) s_value = gcode->get_value('S');
			ledcWrite(p_value, s_value);   // from ledcWrite(ledChannel, dutyCycle);
			// this->commuDevice->OutputMessage(COMMU_OK);
			// this->commuDevice->WriteNotification("IDLE");
			break;
		case 996:
			// Do nothing. this should be the last gcode of a movement in transaction.
			// after MCU reset, This should be the first gcode it received, even the message queue is nothing.
			break;
		default:
			break;
		}
	}else{
		// this->commuDevice->OutputMessage("\n[Warning] CncBase::RunGcode()  Has NO letter 'G' or 'M'. ");
		// this->commuDevice->OutputMessage(gcode->get_command());
		// this->commuDevice->OutputMessage(COMMU_UNKNOWN_COMMAND);
	}
}


void CncBase::RunM42(uint8_t pin_number, uint8_t pin_value){
	digitalWrite(pin_number, pin_value);
}
