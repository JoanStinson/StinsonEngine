#ifndef __MSTIMER_H__
#define __MSTIMER_H__

class MSTimer {
public:
	MSTimer();

	void Start();
	void Read();
	int Stop();

private:
	int currentTime = 0;
	bool runTimer = false;
};

#endif // __TIMER_H__
