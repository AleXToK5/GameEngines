#include "Window.h"
#include "ConfigReader.h"

int main()
{
    setlocale(LC_ALL, "");


    ConfigReader config("config.json");
    Window window(config.GetWindowWidth(), config.GetWindowHeight());
    window.Run();

    return 0;
}