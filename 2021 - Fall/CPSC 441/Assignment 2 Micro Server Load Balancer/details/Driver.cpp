#include <stdlib.h> // system()
#include <unistd.h> // fork() sleep()
#include <stdio.h>  // popen()
#include <iostream>
int main(int argc, char const *argv[])
{
    FILE *fp1, *fp2, *fp3, *fp4;
    // compile micro servers
    system("g++ Echo.cpp -o echo");
    system("g++ Translate.cpp -o translate");
    system("g++ Currency.cpp -o currency");
    system("g++ Vote.cpp -o vote");
    

    if (!fork()) // child process
    {
        fp1 = popen("./echo 9999", "r");
    }
    else if (!fork())
    {
        fp2 = popen("./translate 1111", "r");
    }
    else if (!fork())
    {
        fp3 = popen("./currency 2222", "r");
    }
    else if (!fork())
    {
        fp4 = popen("./vote 3333", "r");
    }
    else
    {
        printf("Enter 1 to terminate micro servers: ");
        int n;
        std::cin >> n; 
        // sleep(120);
        pclose(fp1);
        pclose(fp2);
        pclose(fp3);
        pclose(fp4);

    }
    return 0;
}
