//
// Copyright (c) 2014 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

#ifndef SAMPLE_UTIL_TIMER_H
#define SAMPLE_UTIL_TIMER_H

class Timer
{
public:
    Timer()
        : mPreviousTime(0.0)
    {}
    virtual ~Timer() {}
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual double getElapsedTime() const = 0;

    virtual double getDeltaTime()
    {
        double elapsedTime = getElapsedTime();
        double deltaTime = elapsedTime - mPreviousTime;
        mPreviousTime = elapsedTime;
        return deltaTime;
    }

private:
    double mPreviousTime;
};

Timer *CreateTimer();

#endif // SAMPLE_UTIL_TIMER_H
