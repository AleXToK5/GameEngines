#include "Window.h"
#include "ConfigReader.h"

int main()
{
    setlocale(LC_ALL, "");

    ConfigReader config("config.json");
    Window window(config);
    window.Run();

    return 0;
}