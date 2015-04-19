#include <hidapi.h>
#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <cstring>

#include <signal.h>

#define VID 0x16c0
#define PID 0x0486

typedef struct recvDescriptor {
	float dist0;
	float dist1;
	unsigned char in0;
	unsigned char in1;
	char padding[48];
} recvDescriptor;

typedef union recvData {
	recvDescriptor values;
	unsigned char bytes[sizeof(recvDescriptor)];
} recvData;

typedef struct sendDescriptor {
	char reportID;
	char padding[64];
} sendDescriptor;

typedef union sendData {
	sendDescriptor values;
	unsigned char bytes[sizeof(sendDescriptor)];
} sendData;

bool halted = false;

static void sig_handler(int signo) {
	halted = true;
}

int main(int argc, char ** argv)
{
	int res;
	signal(SIGINT, sig_handler);
	sendData sData;
	recvData rData;
	memset(sData.bytes, 0, sizeof(sData));
	memset(rData.bytes, 0, sizeof(rData));
	
	hid_device* handle = hid_open(VID, PID, NULL);
	if (!handle)
	{
		throw std::runtime_error("Error: Could not find Teensyduino!");
	}
	std::cout << "Found Teensyduino!" << std::endl;
	
	//hid_set_nonblocking(handle, 1); // Makes hid_read non-blocking

	res = hid_write(handle, sData.bytes, sizeof(sendData));
	if (res < sizeof(sendData))
	{
		throw std::runtime_error("Error: Could not write to Teensyduino!");
	}
	std::cout << "Successfully wrote to Teensyduino!" << std::endl;
	
	while (!halted)
	{
		res = hid_write(handle, sData.bytes, sizeof(sendData));
		if (res < sizeof(sendData))
		{
			throw std::runtime_error("Error: Could not write to Teensyduino!");
		}
		res = hid_read(handle, rData.bytes, sizeof(recvData));
		if (res < sizeof(recvData))
		{
			throw std::runtime_error("Error: Could not read from Teensyduino!");
		}	
		//std::cout << "Successfully read from Teensyduino!" << std::endl;
	
		//for (unsigned int i = 0; i < sizeof(recvData); i++)
		//{
		//	std::cout << (unsigned int)rData.bytes[i] << " ";
		//}	
		//std::cout << std::endl;
	
		
		printf("\r%08x : %5.2f:%1d, %5.2f:%1d",*(int *) &rData.values.dist0, rData.values.dist0, rData.values.in0, rData.values.dist1, rData.values.in1); 
	}
	
	std::cout << std::endl << "Closing USB connection" << std::endl;
	hid_close(handle);
}