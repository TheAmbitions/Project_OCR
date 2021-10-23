#include <stdio.h>
#include "display.h"

int main(int argc, char** argv)
{
        if (argc != 2)
                return 1;
        display(argv[1]);
        return 0;
}
