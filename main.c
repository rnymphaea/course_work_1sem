#include <stdio.h>
#include <wctype.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

#include "structs.h"
#include "io_functions.h"


int main()
{
    setlocale(LC_ALL, "");
    print_start_info_cw();
    int numCase = getNumCase();
    controlInput(numCase);
    return 0;
}


