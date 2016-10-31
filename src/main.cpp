// MSU-1 Audio Tool
// Copyright (c) 2016 Michael Larouche <michael.larouche@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// SOX includes
#include <sox.h>

// Local includes
#include "Builder.h"
#include "BuildInfo.h"
#include "ConfigFile.h"

struct SoxInitializer
{
    SoxInitializer()
    : _initResult(0)
    {
        _initResult = sox_init();
    }
    ~SoxInitializer()
    {
        sox_quit();
    }

    bool IsValid() const
    {
        return _initResult == SOX_SUCCESS;
    }

private:
    int _initResult;
};

void showpause()
{
    system("pause");
}

int main(int argc, char** argv)
{
    //atexit(showpause);

    SoxInitializer soxInit;
    if (!soxInit.IsValid())
    {
        fprintf(stderr, "SOX initialization failed !\n");
        return 1;
    }

    msu1::ConfigFile batchConfigFile(argv[1]);

    if (batchConfigFile)
    {
        for (auto& track : batchConfigFile.Tracks())
        {
            msu1::BuildInfo trackBuildInfo;
            trackBuildInfo.SetGlobalSettings(batchConfigFile.GlobalSettings());
            trackBuildInfo.SetTrackSettings(track);
            trackBuildInfo.SetKeepWav(false);

            msu1::Builder builder(trackBuildInfo);
            builder.Build();
        }
    }
    else
    {
        return 1;
    }

    return 0;
}
