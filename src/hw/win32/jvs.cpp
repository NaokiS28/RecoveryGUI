/*
 * BemaniUX - Copyright (C) 2022-2024 spicyjpeg, NaokiS
 *
 * BemaniUX is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * BemaniUX is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * BemaniUX. If not, see <https://www.gnu.org/licenses/>.
 */

#include "jvs.hpp"

int findComPorts(char *foundComList, uint8_t &foundComSize, uint8_t maxSize = 50)
{
    // Find available com ports
    char *portName = nullptr;
    char *fileName = new char[16];
    if (fileName == nullptr)
    {
        LOG_APP("Failed to create file string");
        return -1;
    }

    for (int i = 1; i < maxSize; i++)
    {
        if (((portName = new char[16]) == nullptr) || (fileName = new char(16)) == nullptr)
        {
            LOG_APP("Failed to create port string");
            return -1;
        }

        strncpy(fileName, "\\\\.\\", 16);
        snprintf(portName, 6, "COM%d", i);
        strncat(fileName, portName, 16);

        HANDLE hComPort = CreateFile(
            fileName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        uint32_t error = GetLastError();

        if (error == ERROR_SUCCESS)
        {
            char *newComList = new char[6 * (foundComSize + 1)];
            for (uint8_t c = 0; c < foundComSize; c++)
            {
                newComList[c] = foundComList[c];
            }
            newComList[foundComSize] = *portName;
            delete[] foundComList;
            foundComList = newComList;
            foundComSize++;
            CloseHandle(hComPort);
        }
        delete[] portName;
    }
    delete[] fileName;
    return foundComSize;
}

JVSHandler::JVSHandler(const char *port)
{
    if (port == nullptr)
    {
        LOG_APP("Warning: No COM port given");
    }
}

int JVSHandler::init()
{
    if (_hostPort.comPort == nullptr)
    {
        /*
        _currTask = JVS_TASK_AUTODETECT;

        // Create a promise to hold the result of the port detection
        std::promise<char *> portListPromise;
        std::promise<uint8_t> portListSizePromise;
        std::future<char *> portFuture = portListPromise.get_future();
        std::future<uint8_t> portFutureSize = portListSizePromise.get_future();

        _workerThread = new std::thread([&portListPromise, &portListSizePromise]()
        {
            try {
            char *portList = nullptr;
            uint8_t portListSize = 0;
            findComPorts(portList, portListSize);
            } catch (...){
                portListPromise.set_exception(std::current_exception());
                portListSizePromise.set_exception(std::current_exception());
            }
        });
        */
    }
    return 0;
}

int JVSHandler::update()
{
    return 0;
}
