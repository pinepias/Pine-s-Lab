#include <stdio.h>
#include "engine.h"

int main(int argc, char *args[])
{
    Window GameWindow;
    Engine_Init("The most fucking engine of humanity!1!!", 1024, 576, &GameWindow);

    while (GameWindow.running)
    {
        Engine_Events(&GameWindow);
        Engine_Update(&GameWindow);
        Engine_Render(&GameWindow);
    }

    Engine_CleanUp(&GameWindow);

    return 0;
}