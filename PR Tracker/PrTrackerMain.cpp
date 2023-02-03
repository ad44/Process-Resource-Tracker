#include <windows.h>
#include <string>
#include "process_main.cpp"
#include "gui_main.cpp"

int main()
{
    process_handler* PrcessHandle = new process_handler();
    console_design*  ConsoleDesignHandle = new console_design();

    std::string process_name = ConsoleDesignHandle->open_file();

    if (!PrcessHandle->open(process_name))
    {
        ConsoleDesignHandle->file_open_error();
        getchar();
        return -1;
    }
    
    ConsoleDesignHandle->draw_all_border();
    ConsoleDesignHandle->init_labels(process_name,PrcessHandle->cores,PrcessHandle->total);

    int tim = 0;
    double cpu_sum = 0, cpu_peak = 0, mem_sum = 0, mem_peak = 0, mem_perc_sum = 0, mem_perc_peak = 0;
    PrcessHandle->init_cpu();
    while(PrcessHandle->is_running())
    {
        PrcessHandle->GetMemoryInfo();
        if (PrcessHandle->p_cpu < 100)
        {
            cpu_sum += PrcessHandle->p_cpu;
            cpu_peak = max(cpu_peak, PrcessHandle->p_cpu);
        }
        if(PrcessHandle->p_mem_perc < 100)
        {
            mem_sum += PrcessHandle->p_mem;
            mem_peak = max(mem_peak, PrcessHandle->p_mem);

            mem_perc_sum += PrcessHandle->p_mem_perc;
            mem_perc_peak = max(mem_perc_peak, PrcessHandle->p_mem_perc);
        }

        ConsoleDesignHandle->update_display(tim++, cpu_sum, cpu_peak, mem_sum, mem_peak, mem_perc_sum, mem_perc_peak,PrcessHandle->p_cpu,PrcessHandle->p_mem,PrcessHandle->p_mem_perc);
        Sleep(1000);
    }
    ConsoleDesignHandle->process_closed_msg();
    getchar();
    return 0;
}

