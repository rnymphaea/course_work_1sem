#include <stdio.h>
#include <locale.h>

#include "headers/structs.h"
#include "headers/io_functions.h"
#include "headers/edit_text.h"


int main()
{
    setlocale(LC_ALL, "");
    printStartInfoCw();
    int numCase = getNumCase();
    controlInput(numCase);
    return 0;
}


