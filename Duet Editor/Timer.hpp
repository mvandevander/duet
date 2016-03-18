#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

class Timer
{
    private:
    int startTicks, pausedTicks;
    bool paused, started;
    public:
    Timer();
    void Start();
    void Stop();
    void Pause();
    void UnPause();
    int GetTicks();
    bool IsStarted();
    bool IsPaused();
};

#endif // TIMER_HPP_INCLUDED
