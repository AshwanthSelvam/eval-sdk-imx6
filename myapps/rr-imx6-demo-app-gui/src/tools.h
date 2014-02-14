/*
 * Ridgerun
 * 	2013 efernandez edison.fernandez@ridgerun.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#ifndef TOOLS_H
#define TOOLS_H

#include <fstream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/mxcfb.h>
#include <cstdlib>
#include <cstring>
#include <QWidget>
#include <QFile>
#include <QTextStream>

#include <string.h> /* for strncpy */
#include <iostream>
#include <string>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

/**
 * @file tools.h
 * @brief Colection of functions to be used for the iMX6 Demo to perform common tasks
 * @author Edison Fernández <edison.fernandez@ridgerun.com>
 */

/**
  * @mainpage RidgeRun iMX6 Demo
 *  @details
 *  @author    Edison Fernández <edison.fernandez@ridgerun.com>
 *  @version   2.0
 *  @date      December 2013
 *
 *
 * \section contents Contents
 * - \ref introsec
 *
 * \section introsec Introduction
 *
 * Library for the managment of common tasks for the iMX6 Demo
 *
 */

/**
 *  @brief
 *  Tools
 * */
class Tools
{
public:
    Tools();

    virtual ~Tools();

    // Stores CPU usage
    float  cpu_usage[4];

    /**
      * Set the global alpha value for the selected frame buffer
      *
      * @param alpha Alpha value to be used
      * @param fb Frame buffer to be modified
      *
      * @return True if succeded
      **/
    bool set_alpha(int alpha, int fb);

    /**
      * Wake the selected frame buffer
      *
      * @param Frame buffer number
      **/
    bool wake_fb(int fb);

    /**
      * Get the resolution for the selected display
      *
      * @param fb Frame buffer number
      * @param x Retorned x resolution
      * @param y Retorned y resolution
      **/
    bool get_display_resolution(int fb, int &x, int &y);

    /**
      * Set the resolution for the selected display
      *
      * @param fb Frame buffer number
      * @param x X resolution
      * @param y Y resolution
      **/
    bool set_display_resolution(int fb, int x, int y);

    /**
      * Looks into filepath for key and returns into value the characters between {}.\n
      * sintax: key={value}\n
      *
      *@param filepath complete file path
      *@param key string to be found
      *@param value returned text (if found)
      *@return true if success
      */
    bool get_key(const char* filepath, std::string key, QString &value);

    /**
      * Convert an int value to a std::string
      *
      *@param value int value
      *@return std_string
      */
    std::string to_string(int value);

    /**
      * Print a string to std output
      *
      * @param message Message to be printed
      *
      **/
    void PrintToStdOUT(QString message);

    /**
      * Get Board IP Address
      **/
    QString GetIpAddress();

    /**
      * Updates the CPU Usage
      * The value is stored in the public cpu_usage array
      **/
    void UPDATE_CPU_USEGE();

private:
    QString res_spec;

    // For CPU usage
    float total_cpu_last_time[4];
    float work_cpu_last_time[4];
};

#endif // TOOLS_H
