#include <Windows.h>
#include<tchar.h>
#include<string>
#include <Psapi.h>

class process_handler
{
	private:
		HANDLE hProcess_main;
		DWORD processID_main;
		ULARGE_INTEGER previous_sys_time, previous_process_kernel_time, previous_process_user_time;
        PROCESS_INFORMATION pi;
        PROCESS_MEMORY_COUNTERS pmc;
        
        void init_sys();
        double getCurrentCpuValue();
        void close_handles();
        

	public:
		double p_cpu, p_mem, p_mem_perc;
		double total;
		int cores;
		
        process_handler();
        void init_cpu();
        bool open(std::string arg);
        void GetMemoryInfo();
        bool is_running();
        ~process_handler();
};

process_handler::process_handler()
{
    init_sys();
}

void process_handler::init_sys()
{
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);

    GlobalMemoryStatusEx(&statex);
    total = statex.ullTotalPhys / 1024 / 1024;

    SYSTEM_INFO sys_info;
    GetSystemInfo(&sys_info);
    cores = sys_info.dwNumberOfProcessors;
    p_cpu = p_mem = p_mem_perc = 0;
}

bool process_handler::open(std::string arg)
{
    STARTUPINFOA si;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    LPSTR s = const_cast<char*>(arg.c_str());

    if (!CreateProcessA(NULL, s,NULL,NULL, FALSE,CREATE_NEW_CONSOLE,NULL,NULL, &si, &pi))
        return false;
    
    processID_main = pi.dwProcessId;
    hProcess_main = pi.hProcess;
    return true;
}

void process_handler::init_cpu() {
    FILETIME current_system_time, current_process_kernel_time, current_process_user_time;

    GetSystemTimeAsFileTime(&current_system_time);
    memcpy(&previous_sys_time, &current_system_time, sizeof(FILETIME));

    GetProcessTimes(hProcess_main, &current_system_time, &current_system_time, &current_process_kernel_time
        , &current_process_user_time);
    memcpy(&previous_process_kernel_time, &current_process_kernel_time, sizeof(FILETIME));
    memcpy(&previous_process_user_time, &current_process_user_time, sizeof(FILETIME));
}

double process_handler::getCurrentCpuValue() {

    FILETIME current_system_time, current_process_kernel_time, current_process_user_time;
    ULARGE_INTEGER current, kernel, user;
    double percent;

    GetSystemTimeAsFileTime(&current_system_time);
    memcpy(&current, &current_system_time, sizeof(FILETIME));

    GetProcessTimes(hProcess_main, &current_system_time, &current_system_time, 
        &current_process_kernel_time, &current_process_user_time);

    memcpy(&kernel, &current_process_kernel_time, sizeof(FILETIME));
    memcpy(&user, &current_process_user_time, sizeof(FILETIME));

    percent = (kernel.QuadPart - previous_process_kernel_time.QuadPart) +
        (user.QuadPart - previous_process_user_time.QuadPart);

    percent /= (current.QuadPart - previous_sys_time.QuadPart);

    percent /= cores;
    previous_sys_time = current;
    previous_process_user_time = user;
    previous_process_kernel_time = kernel;

    return percent * 100;
}

void process_handler::GetMemoryInfo()
{

    if (GetProcessMemoryInfo(hProcess_main, &pmc, sizeof(pmc)))
    {
        double phy_memory_used = pmc.WorkingSetSize / 1024 / 1024;

        p_cpu = getCurrentCpuValue();
        p_mem = phy_memory_used;
        p_mem_perc = phy_memory_used / total * 100;
    }
}

void process_handler::close_handles()
{
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hProcess_main);
}

bool process_handler::is_running()
{
    DWORD ex = STILL_ACTIVE;
    GetExitCodeProcess(hProcess_main, &ex);
    return (ex == STILL_ACTIVE);
}
process_handler::~process_handler()
{
    close_handles();
}