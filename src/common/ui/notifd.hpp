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

#pragma once

#include <vector>

namespace notif
{
    typedef enum {
        None,
        Information,
        Warning,
        Error,
        Question,
        Controller,
        Joystick,
        Keyboard,
        Mouse,
        Optical,
        Harddrive,
        MemoryCard,
        File,
        Folder
    } PushIcons;

    typedef enum {
        New,
        Opening,
        Opened,
        Closing
    } MessageState;

    struct PushMessage {
        const char *name;
        const char *message;
        PushIcons icon;
        uint32_t startTime = 0;
        MessageState state = MessageState::New;
    };
    
    class Context
    {
    private:
        std::vector<PushMessage> _messageList = {};

    public:
        void init(){}
        void update(){}

        inline void postMessage(const char *msg, const char *service = nullptr){
            postMessage(PushIcons::Information, msg, service);
        }

        void postMessage(PushIcons icon, const char *msg, const char *service = nullptr);

        void getMessage(int idx, PushMessage &msg) {
            if(idx < (int)_messageList.size()){
                msg = _messageList.at(idx);
            }
        }

        void deleteMessage(int idx){
            if(idx < (int)_messageList.size()){
                _messageList.erase(_messageList.begin() + idx);
            }
        }

        int getLength(){
            return _messageList.size();
        }
    };
}