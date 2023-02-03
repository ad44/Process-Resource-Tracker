#include <Windows.h>
#include<string>
#include <shobjidl.h> 
#include<iostream>
#include "Misc.cpp"
enum {
        dark_blue = 1,
        green = 2,
        red = 4,
        yellow = 6,
        white = 7,
        blue = 9
    };

class console_design
{
    private:
        HANDLE console_color;
        void gotoxy(int x, int y);
        void hidecursor();
        void horz_draw_line(int x_start, int x_end, int y, char c);
        void vert_draw_line(int x, int y_start, int y_end, char c);
        void collide_border(int x, int y);
        void init_color();
        void change_color(int col);
    
    
    public:
        console_design();
        std::string open_file();
        void init_labels(std::string s, const int& cores, const double& total);
        void draw_all_border();
        void print(const int x, const int y, std::string s, int col = white);
        void update_display(int secs, double cpu_sum, const double& cpu_peak, double mem_sum, const double& mem_peak, double mem_perc_sum,
            const double& mem_perc_peak, const double& p_cpu, const double& p_mem, const double& p_mem_perc);
        void file_open_error();
        void process_closed_msg();
        ~console_design();

};
console_design::console_design()
{
    init_color();
    hidecursor();
}
void console_design::gotoxy(int x, int y)
{
    COORD c;
    c.X = x + 14;
    c.Y = y + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void console_design::hidecursor()
{
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(console_color, &info);
}

void console_design::init_color()
{
    console_color = GetStdHandle(
        STD_OUTPUT_HANDLE);
}

std::string console_design::open_file()
{
    std::string f_name;
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
        COINIT_DISABLE_OLE1DDE);

    if (SUCCEEDED(hr))
    {
        IFileOpenDialog* pFileOpen;

        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

        if (SUCCEEDED(hr))
        {
            hr = pFileOpen->Show(NULL);

            if (SUCCEEDED(hr))
            {
                IShellItem* pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr))
                {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    if (SUCCEEDED(hr))
                    {
                        for (int i = 0; i < wcslen(pszFilePath); i++)
                        {
                            f_name.push_back(pszFilePath[i]);
                            if(pszFilePath[i] =='\\')f_name.push_back(pszFilePath[i]);
                        }

                        if(misc::is_exe(f_name))
                        MessageBoxW(NULL, pszFilePath, L"Process executable selected", MB_OK);
                        else MessageBoxW(NULL, pszFilePath, L"Invalid File Extension", MB_DEFAULT_DESKTOP_ONLY);
                       
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileOpen->Release();
        }
        CoUninitialize();
    }
    return f_name;
}

void console_design::horz_draw_line(int x_start,int x_end,int y,char c)
{
    for (int i = x_start + 1; i < x_end; i++)
    {
        gotoxy(i, y);
        std::cout << c;
    }
}

void console_design::vert_draw_line( int x,int y_start, int y_end, char c)
{
    for (int i = y_start + 1; i < y_end; i++)
    {
        gotoxy(x, i);
        std::cout << c;
    }
}

void console_design::collide_border(int x,int y)
{
    gotoxy(x, y);
    std::cout << "+";
}
void console_design::draw_all_border ()
{
    int x_start = 0, x_end = 90;
    int y_start = 0, y_end = 24;
    
    int mid = (x_start + x_end) / 2 - 1;

    horz_draw_line(x_start, x_end, y_start, '-');
    horz_draw_line(x_start, x_end, y_end, '-');
    vert_draw_line(x_start, y_start, y_end, '|');
    vert_draw_line(x_end, y_start, y_end, '|');


    horz_draw_line(x_start,x_end,y_start+4,'=');
    horz_draw_line(x_start,x_end,y_start+8,'=');
    horz_draw_line(x_start,x_end,y_start+12,'-');
    horz_draw_line(x_start,x_end,y_start+20,'-');
    
    vert_draw_line(mid, y_start, y_start + 4, '|');
    vert_draw_line(mid, y_start + 4, y_start + 8, '|');
    vert_draw_line(mid-1, y_start + 12, y_start + 20, '|');
    vert_draw_line(mid+1, y_start + 12, y_start + 20, '|');
    
    collide_border(mid, y_start+4);
    collide_border(mid, y_start + 8);
    collide_border(mid-1, y_start + 12);
    collide_border(mid+1, y_start + 12);
    collide_border(mid-1,y_start + 20);
    collide_border(mid+1, y_start +20);
    

    collide_border(x_start, y_start);
    collide_border(x_end, y_start);
    collide_border(x_end, y_end);
    collide_border(x_start, y_end);
}


void console_design::change_color(int col)
{
    SetConsoleTextAttribute(console_color, col);
}

void console_design::print(const int x, const int y, std::string s,int col)
{
    change_color(col);
    gotoxy(x,y);
    std::cout << s;
}
void console_design::init_labels(std::string s,const int& cores,const double& total)
{
    if (s.size() > 58)
    {
        s = s.substr(s.size() - 58, s.size());
        s[0] = s[1] = s[2] = '.';
    }

    std::string temp = "Process Name : " ;
    temp.append(s);
    std::string t;
    if (s.size() < 52)
    {
        for (int i = 0; i < (26- s.size() / 2); i++) t += ' ';
    }
    t.append(temp);
    print(22, 2, "CPU");
    print(65, 2, "MEMORY");
    print(10, 6, "System Logical Processors : ");
    print(38, 6, std::to_string(cores), blue);
    print(50, 6, "System Memory Available : ");
    print(76, 6, std::to_string(total).substr(0, 5) + " MB", blue);
    print( 10, 10, t,yellow);
    
    print(6, 14, "Average CPU Used (%) : ");
    print(9, 16, "Peak CPU Used (%) : ");
    print(6, 18, "Current CPU Used (%) : ");

    print(48, 14, "Average Mem Used (MB/%) : ");
    print(51, 16, "Peak Mem Used (MB/%) : ");
    print(48, 18, "Current Mem Used (MB/%) : ");

    print(29, 22, "Total Time Elapsed : ");
}
void console_design::update_display(int secs,double cpu_sum,const double& cpu_peak,double mem_sum, const double& mem_peak,double mem_perc_sum,
    const double& mem_perc_peak, const double& p_cpu, const double& p_mem, const double& p_mem_perc)
{
    int n = secs;
    int min = secs / 60;
    int hrs = min / 60;
    min = min % 60;
    secs = secs % 60;
    
    //deciding color
    int c1, c2, c3, c4, c5, c6;
    c1 = c2 = c3 = c4 = c5 = c6 = green;
    
    if (cpu_sum / n > 30)c1 = red;
    else if (cpu_sum / n > 10)c1 = yellow;

    if (cpu_peak> 30)c2 = red;
    else if (cpu_peak> 10)c2 = yellow;

    if (p_cpu > 30)c3 = red;
    else if (p_cpu > 10)c3 = yellow;

    if (mem_perc_sum / n > 30)c4 = red;
    else if (mem_perc_sum / n > 10)c4 = yellow;

    if (mem_perc_peak > 30)c5 = red;
    else if (mem_perc_peak > 10)c5 = yellow;

    if (p_mem_perc > 30)c6 = red;
    else if (p_mem_perc > 10)c6 = yellow;

    print(50, 22, std::to_string(hrs) + " H "+ std::to_string(min) +" M " + std::to_string(secs) + " S  ");
    
    print(29, 14, std::to_string(cpu_sum / n).substr(0, 5) + " ",c1);
    print(29, 16, std::to_string(cpu_peak).substr(0, 5) + " ",c2);
    print(29, 18, std::to_string(p_cpu).substr(0, 5) + " ",c3);

    print(74, 14, std::to_string(mem_sum/n).substr(0,6) + '/' + std::to_string(mem_perc_sum/n).substr(0,6) + "  ",c4);
    print(74, 16, std::to_string(mem_peak).substr(0,6) + '/' + std::to_string(mem_perc_peak).substr(0,6) + "  ",c5);
    print(74, 18, std::to_string(p_mem).substr(0,6) + '/' + std::to_string(p_mem_perc).substr(0,6) + "  ",c6);
}

void console_design::file_open_error()
{
    print(30, 5, "Process Creation Failed", red);
    print(31, 7, "Press any key to exit",blue);
}

void console_design::process_closed_msg()
{
    print(35, 11, "PROCESS NOT RUNNING", red);
    print(35, 26, "PRESS ENTER TO EXIT");
}

console_design::~console_design()
{
    CloseHandle(console_color);
}