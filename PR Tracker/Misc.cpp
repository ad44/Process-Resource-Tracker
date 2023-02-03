#include<string>

class misc
{
public:
	static bool is_exe(std::string& s)
	{
		return s.substr(s.size() - 4, s.size()) == ".exe";
	}

};

