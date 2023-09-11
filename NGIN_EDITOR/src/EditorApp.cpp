// EditorApp.cpp

#include <NGIN.h>
#include <NGIN/Meta/Reflection/Registry.hpp>

class EditorApp : public NGIN::App
{
public:
	EditorApp() {}
	~EditorApp() {}
	void Init() override
	{
		while (true)
		{
		}
	}
};

int main(int argc, char *argv[])
{
	int success = NGIN::Init<EditorApp>(argc, argv);

	return 0;
}