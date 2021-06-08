

#include "XPLMDisplay.h"
#include "XPLMProcessing.h"
#include "XPLMDataAccess.h"
#include "XPLMPlugin.h"
#include "XPLMUtilities.h"

#include "SerialPort.hpp"

#include <string>
#include <sstream>
#include <iomanip>


#ifndef XPLM300
	#error This is made to be compiled against the XPLM300 SDK
#endif

SerialPort* arduino;

XPLMDataRef phi_ind_elec_pilot_deg;
XPLMDataRef the_ind_elec_pilot_deg;
XPLMDataRef altitude_ft_pilot;
XPLMDataRef airspeed_kts_pilot;
XPLMDataRef heading_deg_pilot;
XPLMDataRef vertical_fpm_pilot;
XPLMDataRef barometer_setting;
XPLMDataRef slip_deg;

float temp;
float temp2;
float ind_roll;
float ind_pitch;
float ind_altitude;
float ind_airspeed;
float ind_heading;
float ind_vs;
float ind_slip;
float current_barometer_setting;

char input_buf[255];

float flight_loop_callback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
float altitude_loop_callback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon);
void write_float(char ident, float num);
void write_int(char ident, int num);


PLUGIN_API int XPluginStart(
							char *		outName,
							char *		outSig,
							char *		outDesc)
{
	strcpy(outName, "ArduinoEFIS");
	strcpy(outSig, "bobbythierry.efis");
	strcpy(outDesc, "This plugin sends data via serial to arduino that is controlling an efis");

	arduino = new SerialPort("\\\\.\\COM3");


	phi_ind_elec_pilot_deg = XPLMFindDataRef("sim/cockpit/gyros/phi_ind_elec_pilot_deg");
	the_ind_elec_pilot_deg = XPLMFindDataRef("sim/cockpit/gyros/the_ind_elec_pilot_deg");
	vertical_fpm_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/vvi_fpm_pilot");
	heading_deg_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/heading_AHARS_deg_mag_pilot");
	altitude_ft_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/altitude_ft_pilot");
	airspeed_kts_pilot = XPLMFindDataRef("sim/cockpit2/gauges/indicators/calibrated_airspeed_kts_pilot");
	slip_deg = XPLMFindDataRef("sim/cockpit2/gauges/indicators/slip_deg");
	barometer_setting = XPLMFindDataRef("sim/cockpit/misc/barometer_setting");

	XPLMRegisterFlightLoopCallback(flight_loop_callback, -1.0, NULL);
	XPLMRegisterFlightLoopCallback(altitude_loop_callback, -1.0, NULL);

	return 1;
}

PLUGIN_API void	XPluginStop(void)
{
	XPLMUnregisterFlightLoopCallback(flight_loop_callback, NULL);
	arduino->closeSerial();
}

PLUGIN_API void XPluginDisable(void) { }
PLUGIN_API int  XPluginEnable(void)  { return 1; }
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void * inParam) { }

float altitude_loop_callback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon) {
	temp2 = XPLMGetDataf(altitude_ft_pilot);

	if (std::abs(ind_altitude - temp2) > 10) {
		ind_altitude = temp2;

		write_int('c', ind_altitude);
	}

	temp2 = XPLMGetDataf(airspeed_kts_pilot);

	if (std::abs(ind_airspeed - temp2) > 1) {
		ind_airspeed = temp2;

		write_int('d', (int)ind_airspeed);
	}

	temp2 = XPLMGetDataf(heading_deg_pilot);

	if (std::abs(ind_heading - temp2) > 1) {
		ind_heading = temp2;

		write_int('f', (int)ind_heading);
	}

	return 0.2;
}

float flight_loop_callback(float inElapsedSinceLastCall, float inElapsedTimeSinceLastFlightLoop, int inCounter, void* inRefcon) {

	// Read in data from arduino
	if (arduino->readSerialPort(input_buf, 255)) {
		std::string received = input_buf;

		// Increment/Decrement barometer setting depending on recieved data
		if (received[0] == 'z') {
			current_barometer_setting = XPLMGetDataf(barometer_setting);
			XPLMSetDataf(barometer_setting, current_barometer_setting + 0.01);

			write_float('e', current_barometer_setting + 0.01);
		}
		else if (received[0] == 'x') {
			current_barometer_setting = XPLMGetDataf(barometer_setting);
			XPLMSetDataf(barometer_setting, current_barometer_setting - 0.01);
			write_float('e', current_barometer_setting - 0.01);
		}
	};
	
	// Roll
	temp = XPLMGetDataf(phi_ind_elec_pilot_deg);

	// If there is a roll different of more than 0.1 degree
	if (std::abs(ind_roll-temp) > 0.5) {
		// Send roll as radians
		ind_roll = temp;
		
		write_float('a', ind_roll);
	}

	// Pitch
	temp = XPLMGetDataf(the_ind_elec_pilot_deg);

	if (std::abs(ind_pitch - temp) > 0.5) {
		ind_pitch = temp;

		write_float('b', ind_pitch);
	}

	// Vertical Speed
	temp = XPLMGetDataf(vertical_fpm_pilot);

	if (std::abs(ind_vs - temp) > 10) {
		ind_vs = temp;

		write_int('g', ind_vs);
	}

	// Slip degrees
	temp = XPLMGetDataf(slip_deg);

	if (std::abs(ind_slip - temp) > 0.1) {
		ind_slip = temp;

		write_float('h', ind_slip);
	}

	return -5;
	
}

void write_float(char ident, float num) {
	std::stringstream stream;
	stream << std::fixed << std::setprecision(2) << (num);

	std::string buffer = ident + stream.str() + '\n';
	arduino->writeSerialPort(buffer.c_str(), buffer.size());
}

void write_int(char ident, int num) {
	std::string buffer = ident + std::to_string(num) + '\n';
	arduino->writeSerialPort(buffer.c_str(), buffer.size());
}

