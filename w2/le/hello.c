#include <stdio.h>
#include <cs50.h> // asks compiler to trust that there will be this file
                    // need to actually link in the code
                    // clang -o hello hello.c -lcs50
                        // l links in the zeros and ones needed
                        // -o means output a file named hello based on hello.c
                    // we're gonna not do this and use the make thing to abstract away
                        // just an easier less under the hood version

int main(void)
{
    string name = get_string("What's your name? ");
    printf("hello, %s\n",name);
}

// "Compiling" code:
    // 1. preprocessing
        // '#include' statements, and prototypes of functions
    // 2. compiling
        // make it into assembly language
            // that's the stuff that gets directly fed into the cpu
    // 3. assembling
        // assembly language turned into 0s and 1s
    // 4. linking
        // link hello.c, cs50.c, stdio.c into one single file