/*
    Send.h
    Author: Seb Madgwick
*/

#ifndef Send_h
#define Send_h

//------------------------------------------------------------------------------
// Class declaration

class Send {
    public:
        static void flexSensorData();
        static void sensorData();
        static void quaternionData(const float qArray[]);
        static void miscData(const char* const charArray);
        static void buttonState(const byte buttonNr);
    private:
        static void IntValToChars(char* const charArray, int* const index, int i);
        static char calcChecksum(const char* const packet, const int packetLength);
};

#endif

//------------------------------------------------------------------------------
// End of file
