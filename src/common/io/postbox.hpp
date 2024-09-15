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

#include <queue>
#include <mutex>

namespace post
{
    struct InputMsgStruct
    {
        uint32_t classCode;
        uint32_t classIdx;
        uint32_t message;
        uint32_t param;
    };

    class PostBox
    {
    private:
        std::queue<InputMsgStruct> _msgList;
        std::mutex _mutex;  // Mutex to protect _msgList

    public:
        // Post a message to the queue
        void postMessage(uint32_t classCode, uint32_t classIdx, uint32_t messageCode, uint32_t param)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _msgList.push(InputMsgStruct{classCode, classIdx, messageCode, param});
        }

        // Get a message from the queue
        bool getMessage(InputMsgStruct *msg)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if (_msgList.empty()) return false;
            *msg = _msgList.front();
            _msgList.pop();
            return true;
        }

        // Check if there are any messages available
        bool available()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return !_msgList.empty();
        }

        // Get the size of the message queue
        size_t getSize()
        {
            std::lock_guard<std::mutex> lock(_mutex);
            return _msgList.size();
        }
    };
}
