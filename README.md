# Process-Resource-Tracker

Process resource tracker is a simple light weight console application that can be used to monitor memory and cpu usage of a selected process .  It can be effectively used to test any process over certain period of time by obtaining average, maximum and realtime cpu/memory usage consumed by the process. It's very similar to task manager's resource monitor.

## **Platform Supported** 
* Windows (32 and 64 bit)

## **Test**  
* Tested with Windows 10.
* Expected to run on all Windows OS newer to Windows 7.

## **Technologies**
* C++ 14
* Microsoft Win API

## **IDE** 
* Visual Studio 2022.

## **Installation** 
* Just run the Solution file with Visual Studio 2022 (with C++ development options). It already comes up with all required Win API header files.

## **Requirement** 
* To run the Executable without IDE, you would need  - Visual C++ Redistributable for Visual Studio 2015 which can be downloaded here - https://www.microsoft.com/en-in/download/details.aspx?id=48145.

## **Usage** 
* Just select the process executable you want to monitor. If the process starts running successfully, the console will display the necessery data.
![Untitled](https://user-images.githubusercontent.com/47539347/216705555-032e4689-2fa3-4878-8fba-9230820039a4.png)
* To run any process in admin mode, start the program as admin.

## **Features** 
It will display the following details about the opened process:-
* Current,average, and peak CPU usage (percent).
* Current,average, and peak Memory usage (percent and megabytes).

## **Limitations**
* It is not supposed to work with process that creates several child process. It might run in this case but won't provide accurate data.

### *Note* 
*The data obtained with this application might differ slightly from the one obtained through task manager. It's still very relevant / reliable and sometimes collected more efficient.*

