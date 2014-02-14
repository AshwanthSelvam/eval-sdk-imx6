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


#include "pipelines.h"

pipelines::pipelines()
{

    // Init variables
    preview_1080p_video="/opt/RR-Demo/media/bbb_1080p.mp4";
    preview_720p_video="/opt/RR-Demo/media/Kungfu.mp4";
    preview_480p_video="/opt/RR-Demo/media/bbb_780p.mp4";

    // Snapshot Location
    SnapshotLocation = "/opt/RR-Demo/media/snapshot.jpeg";

    // Streaming Caps
    CameraCaps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, sprop-parameter-sets=(string)\"Z0JAHqaAoD2QAA\\=\\=\\,aM4wpIAA\", payload=(int)96, ssrc=(uint)2031969529, clock-base=(uint)3875899417, seqnum-base=(uint)45401";
    VideoTestCaps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, sprop-parameter-sets=(string)\"Z0JAFKaBQfkA\\,aM4wpIAA\", payload=(int)96, ssrc=(uint)2288404012, clock-base=(uint)2852572406, seqnum-base=(uint)42161";
    FileCaps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)MP4V-ES, profile-level-id=(string)1, config=(string)000001b001000001b58913000001000000012000c48d8800c53c04871463000001b24c61766335312e34342e30, payload=(int)96, ssrc=(uint)281575967, clock-base=(uint)3498156131, seqnum-base=(uint)20571";
    StreamClientCaps = "application/x-rtp,media=(string)video,clock-rate=(int)90000,encoding-name=(string)H264,sprop-parameter-sets=(string)\"Z0LAHpp0AoAi/PCAAAADAIAAABhHixdQ\\,aM4LyA\\=\\=\",payload=(int)96,ssrc=3107335084,clock-base=3676209233,seqnum-base=50612";

}
