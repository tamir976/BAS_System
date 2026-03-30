# BAS_System
Biometric Attendance System is solution for taking attendance for events that is necessary for post processing and automation.
This system uses R307 fingerprint module along with Rasperry pico to take fingerprint and deploy to backend service in Raspberry Pi for further analysis. 

This project starts from developing real time driver using FreeRTOS for R307 on Rasberry pico for fast processing with UART communication protocol in Embedded C.

The backend service is then developed in Cpp and python for storing the information and perform ML analysis for future prediction of how likely the person is going to be late or at time or early for the next events. 

This project demonstrates full stack development skill from hardware integration in low level C language development to high level data processing development. 
