/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Huda
 *
 * Created on September 12, 2020, 11:48 AM
 */

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
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
    
    std::vector<int> buffDat;
    char buffLine[30];
    
    ////linux only
    while (1)
    {
        auto iteB = buffDat.begin();
        std::cout << "start iterate" << std::endl;
        while (!buffDat.empty())
        {
            if ((*iteB) % 9 == 0)
            {
                std::cout << *iteB << std::endl;
            }
            iteB = buffDat.erase(iteB);
        }
        
        std::ifstream data("/tmp/inputData.txt");
        while (!data.eof())
        {
            data.getline(buffLine, 30);
            std::string line = std::string(buffLine);
            std::cout << "get str " << line << std::endl;
            if (line.length() < 1)
            {
                break;
            }
            std::string  tmp = line.substr(0, line.length() - 1);
            int num = std::stoi(tmp);
            std::cout << "get " << num << std::endl;
            buffDat.push_back(num);
        }
        data.close();
        
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
            system("rm -f /tmp/inputData.txt");
            system("cp /tmp/input.txt /tmp/inputData.txt");
            ftruncate(fd, 0);
            int res = fcntl(fd, F_UNLCK, &lock);
        }
        fclose(fileL);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    ////linux only
    
    /*
    while (1)
    {
        auto iteB = buffDat.begin();
        std::cout << "start iterate" << std::endl;
        while (!buffDat.empty())
        {
            if ((*iteB) % 9 == 0)
            {
                std::cout << *iteB << std::endl;
            }
            iteB = buffDat.erase(iteB);
        }
        
        std::ifstream data("d:\\inputData.txt");
        while (!data.eof())
        {
            data.getline(buffLine, 30);
            std::string line = std::string(buffLine);
            std::cout << "get str " << line << std::endl;
            if (line.length() < 1)
            {
                break;
            }
            std::string  tmp = line.substr(0, line.length() - 1);
            int num = std::stoi(tmp);
            std::cout << "get " << num << std::endl;
            buffDat.push_back(num);
        }
        data.close();
        
        HANDLE fh = CreateFile(fileName.data(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
        if (fh == INVALID_HANDLE_VALUE)
        {
            DWORD res = GetLastError();
            std::cout << "fail " << res << std::endl;
            exit(1);
        }
        auto res = DeleteFile("d:\\inputData.txt");
        CopyFile(fileName.data(), "d:\\inputData.txt", TRUE);
        CloseHandle(fh);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
     */
    
    return 0;
}

