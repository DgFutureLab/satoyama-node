void sleepMCU();
void sleep_radio();
void wakeup_radio();

// void doStuff();
void rtcInterrupt();

void read_sensors();

void read_temperature();
void read_humidity();
void read_sonar_distance();

void init_sdcard();
void writeData(unsigned char *buffer);