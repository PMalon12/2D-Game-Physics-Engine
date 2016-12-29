#pragma once
#include <cstdint>

class Time
{
public:
	Time() { setSeconds(0); };
	~Time() {};

	Time& setMicroSeconds(int64_t ms) { microSeconds = ms; milliSeconds = double(ms) * 0.001; seconds = double(ms) * 0.000001; return *this; }
	Time& setMilliSeconds(double ms) { milliSeconds = ms; microSeconds = ms * 1000; seconds = ms * 0.001; return *this; }
	Time& setSeconds(double s) { seconds = s; microSeconds = s * 1000000; milliSeconds = s * 1000; return *this; }

	int64_t getMicroSeconds() { return microSeconds; }
	double getMilliSeconds() { return milliSeconds; }
	double getSeconds() { return seconds; }

	int64_t removeMicroSeconds(int64_t ms) { setMicroSeconds(microSeconds - ms); return microSeconds; }
	double removeMilliSeconds(double ms) { setMilliSeconds(milliSeconds - ms); return milliSeconds; }
	double removeSeconds(double s) { setSeconds(seconds - s); return seconds; }

	int64_t addMicroSeconds(int64_t ms) { setMicroSeconds(microSeconds + ms); return microSeconds; }
	double addMilliSeconds(double ms) { setMilliSeconds(milliSeconds + ms); return milliSeconds; }
	double addSeconds(double s) { setSeconds(seconds + s); return seconds; }

	void operator-(Time& rhs)
	{
		microSeconds -= rhs.getMicroSeconds();
		milliSeconds -= rhs.getMilliSeconds();
		seconds -= rhs.getSeconds();
	}

	void operator+(Time& rhs)
	{
		microSeconds += rhs.getMicroSeconds();
		milliSeconds += rhs.getMilliSeconds();
		seconds += rhs.getSeconds();
	}

private:

	int64_t microSeconds;
	double milliSeconds;
	double seconds;
};