/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Huda
 *
 * Created on September 12, 2020, 10:54 AM
 */

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
////linux
#include <fcntl.h>
#include <unistd.h>
////linux
////win
//#include <windows.h>

/*
 * 
 */
int main(int argc, char** argv) 
{
    ////linux
    static std::string fileName = "/tmp/input.txt";
    ////win
    //static std::string fileName = "d:\\input.txt";
    
    ////linux only
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_whence = SEEK_SET;
    lock.l_len = 0;
    lock.l_pid = getpid();
    ////linux only
    
    ////linux only
    while (1)
    {
        FILE *fileL = fopen(fileName.data(), "a");
        if (fileL == NULL)
        {
            std::cout << "file gagal dibuka" << std::endl;
            exit(1);
        }
        int fd = fileno(fileL);
        int res = fcntl(fd, F_SETLK, &lock);
        if (res != 0)
        {
            std::cout << "gagal lock " << std::endl; //<< res << std::endl;
        }
        else
        {
            std::cout << "berhasil lock" << std::endl;
            std::srand(std::time(nullptr));
            int num = std::rand();
            fprintf(fileL, "%i\n", num);
            int res = fcntl(fd, F_UNLCK, &lock);
        }
        fclose(fileL);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    ////linux only
    
    ////win
    /*
    while (1)
    {
        HANDLE fh = CreateFile(fileName.data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
        if (fh == INVALID_HANDLE_VALUE)
        {
            DWORD res = GetLastError();
            std::cout << "fail " << res << std::endl;
            exit(1);
        }
        std::srand(std::time(nullptr));
        int num = std::rand();
        std::string tmp = std::to_string(num);
        tmp.append("\n");
        DWORD written = 0;
        auto res = WriteFile(fh, tmp.data(), tmp.length(), &written, NULL);
        CloseHandle(fh);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
     */
    
    return 0;
}

