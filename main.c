#include <stdio.h>
#include <wctype.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

#include "structs.h"
#include "io_functions.h"
#include "edit_text.h"


int main()
{
    setlocale(LC_ALL, "");
    printStartInfoCw();
    int numCase = getNumCase();
    controlInput(numCase);
    return 0;
}


