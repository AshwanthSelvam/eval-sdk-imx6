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


#include "tools.h"
#include <unistd.h> 

Tools::Tools()
{
}


Tools::~Tools()
{
}


bool Tools::set_alpha(int alpha, int fb){

    struct mxcfb_gbl_alpha gbl_alpha;
    int _fb;
    QString dev = QString("/dev/fb%1").arg(fb); // Frame buffer device

    // Enable end set alpha value
    gbl_alpha.enable = 1;
    gbl_alpha.alpha = alpha;

    _fb = open(qPrintable(dev), O_RDWR, 0);
    if(_fb < 0){
        printf("\n->Could not open %s...\n",qPrintable(dev));
        exit(-1);
    }

    if(ioctl(_fb, MXCFB_SET_GBL_ALPHA, &gbl_alpha) < 0){ // Set alpha to fb
        printf("\nUnable to set alpha blending for %s\n",qPrintable(dev));
    }

    close(_fb);
    return true;
}


/**
  * Wake display
  **/
bool Tools::wake_fb(int fb)
{
    printf("-> Waiking fb%d...\n", fb);
    std::ofstream blank;
    QString _fb = QString("/sys/class/graphics/fb%1/blank").arg(fb);
    blank.open(qPrintable(_fb));
    blank << 1;
    blank.close();

    blank.open(qPrintable(_fb));
    blank << 0;
    blank.close();

    if(blank.fail())
    {
        printf("\nFailed to open fb%d",fb);
        exit(-1);
    }

    return true;
}

/**
  ** Gets display resolution
  */
bool Tools::get_display_resolution(int fb, int &x, int &y){

    struct fb_var_screeninfo screeninfo; // Structure for screen info
    QString dev = QString("/dev/fb%1").arg(fb); // Frame buffer device
    int _fb = open(qPrintable(dev), O_RDONLY); // Try to open devide

    if (! (_fb < 0) ){
        printf("\n-> Getting resolution for %s...\n",qPrintable(dev));

        // Read screen information
        if(ioctl(_fb, FBIOGET_VSCREENINFO, &screeninfo) < 0){
            printf("\n--> Failed to obtain %s info...",qPrintable(dev));
            exit(-1);
        }

        // Get the resolution
        x = screeninfo.xres;
        y = screeninfo.yres;

        printf("--> x = %d...\n",x);
        printf("--> y = %d...\n",y);

        close(_fb);

    }
    else{
        printf("\nFailed to open %s...\n",qPrintable(dev));
        exit(-1);
    }
}

/**
  ** Sets display resolution
  */
bool Tools::set_display_resolution(int fb, int x, int y){

    struct fb_var_screeninfo screeninfo; // Structure for screen info
    QString dev = QString("/dev/fb%1").arg(fb); // Frame buffer device
    int _fb = open(qPrintable(dev), O_RDWR); // Try to open devide

    if (! (_fb < 0) ){
        printf("\n-> Setting resolution for %s...\n",qPrintable(dev));
        printf("--> x = %d...\n",x);
        printf("--> y = %d...\n",y);

        // Read screen information
        if(ioctl(_fb, FBIOGET_VSCREENINFO, &screeninfo) < 0){
            printf("\n--> Failed to obtain %s info...",qPrintable(dev));
            exit(-1);
        }

        // Updating resolution
        screeninfo.xres = x;
        screeninfo.yres = y;

        if(ioctl(_fb, FBIOPUT_VSCREENINFO, &screeninfo) < 0){
            printf("\n--> Failed to obtain %s info...", qPrintable(dev));
            exit(-1);
        }

        close(_fb);

    }
    else{
        printf("\nFailed to open %s...\n",qPrintable(dev));
        exit(-1);
    }
}


bool Tools::get_key(const char* filepath, std::string key, QString& value){

    std::ifstream file;
    std::string read;
    size_t  start_pos;
    size_t  end_pos;

    file.open(filepath);

    if(!file.is_open()) {
        printf("\nCould not open file...");
        return false;
    }

    while(file.good()){

        getline(file,read); // Read Line

        start_pos = read.find(key); // Find Key

        if(start_pos == std::string::npos) continue;    // Key not fount

        start_pos = read.find("{"); // First delimiter
        end_pos = read.find("}"); // Last delimiter

        if(start_pos == std::string::npos || end_pos == std::string::npos){
            printf("\nDelimiters not found...");
            break;
        }

        read = read.substr(start_pos + 1, end_pos - start_pos - 1); // Read value

        value = QString::fromUtf8(read.c_str());
        return true;    // Key found

    }

    return false;   // Key not found

}


void Tools::PrintToStdOUT(QString message){
    QTextStream(stdout) << message << endl;
}


std::string Tools::to_string(int value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}



QString Tools::GetIpAddress(){
    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    return QString::fromUtf8(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    /* display result */
    //printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}


void Tools::UPDATE_CPU_USEGE()
{
        std::ifstream *fp = new std::ifstream("/proc/stat");
        char cpu_id[5]; // CPU to search
        char* line_buffer;
        int i = 0;      // CPU counter
        int times[7];

        // Time variables
        int total_time;
        int work_time;

        std::string line;
          if (fp->is_open())
          {
            while ( std::getline (*fp,line) && i<4 ) // End of file or 4 cpus founded
            {
                sprintf(cpu_id, "cpu%d", i);  // Get CPU ID

                if(line.find(cpu_id) != std::string::npos){

                    // Pass line to line buffer
                    line_buffer = new char[line.size() + 1];
                    strcpy (line_buffer, line.c_str());

                    // Split data and get total and work time
                    strtok(line_buffer, " ");
                    // Get all 7 values
                    times[0] = atoi(strtok(NULL, " "));
                    times[1] = atoi(strtok(NULL, " "));
                    times[2] = atoi(strtok(NULL, " "));
                    times[3] = atoi(strtok(NULL, " "));
                    times[4] = atoi(strtok(NULL, " "));
                    times[5] = atoi(strtok(NULL, " "));
                    times[6] = atoi(strtok(NULL, " "));
                    work_time = times[0] + times[1] + times[2];
                    total_time = work_time + times[3] + times[4] + times[5] + times[6];

                    // Update CPU Usage
                    cpu_usage[i] = ((float)work_time - work_cpu_last_time[i]) / ((float)total_time -total_cpu_last_time[i]) * 100;

                    // Update last values
                    work_cpu_last_time[i] = (float)work_time;
                    total_cpu_last_time[i] = (float)total_time;

                    /*std::cout << line << std::endl;
                    std::cout << cpu_usage[i] << std::endl;
                    std::cout << work_time << std::endl;
                    std::cout << total_time << std::endl;
                    */

                    i++; // Next CPU
                }

            }
            fp->close();
          }
}
