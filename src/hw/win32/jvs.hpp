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

/*
    Notes:
        Whilst GetCommPorts exist, to ensure compatibility it is not used.
*/

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <windows.h>
#include <cstdio>
#include <thread>
#include <future>
#include <iostream>
#include <atomic>
#include "common/devhandler.hpp"
#include "common/util/log.hpp"

/*
    Todo:
    [ ] - Library is skeleton
    [ ] - Create virtual inputs from each JVS IO
*/

enum JVSTask {
    JVS_TASK_NONE,
    JVS_TASK_AUTODETECT,
    JVS_TASK_INIT,
    JVS_TASK_RUN
};

enum JVSStatus {
    JVS_NOT_READY,
    JVS_READY,
    JVS_IO_ERROR,
    JVS_NO_IO,
    JVS_ERROR
};

constexpr int JVS_CFG_USE_NONE (1 << 0);
constexpr int JVS_CFG_USE_CTS (1 << 1);
constexpr int JVS_CFG_USE_RTS (1 << 2);
constexpr int JVS_CFG_USE_DCR (1 << 3);
constexpr int JVS_CFG_USE_DTR (1 << 4);
constexpr int JVS_CFG_USE_RI (1 << 4);

struct JVSHostPort {
    HANDLE comPort = nullptr;
    DCB* comPortSettings = nullptr;
    JVSStatus comStatus = JVS_NOT_READY;
    uint16_t jvsSenseMode = JVS_CFG_USE_NONE;
};

class JVSHandler : public DeviceHandler
{
private:
    JVSHostPort _hostPort;
    int ioCount = 0;
    //JVSTask _currTask = JVS_TASK_NONE;

    //std::thread *_workerThread = nullptr;

public:
    JVSHandler(const char* port = nullptr);
    int init();
    int reload() { return 0; }
    int isReady(){ return 0; }
    int update();

    const char *getClassName() { return "JVS"; }
    int getDeviceCount() { return ioCount; }
    
    int getClassType() { return INPUT_CLASS_JOYSTICK; }

    bool getSwitch(uint32_t code) { return false; }
    int getAnalog(uint32_t code) { return 0; }
    int getRelative(uint32_t code) { return 0; }
};

//REGISTER_DEVICE_HANDLER(JVSHandler)