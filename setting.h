#pragma once
#include <fstream>
#include <utility>
#include <string>
#include <fileapi.h>
#include <stdio.h>
#include <direct.h>

#define DEFAULT_TIMER_INTERVAL 500
#define DEFAULT_ALARM_INTERVAL 60
#define DEFAULT_DELETE_INTERVAL 3
#define DEFAULT_SAVE_INTERVAL {10, 180}
#define DEFAULT_THRESHOLD 0.004F
#define DEFAULT_MATCHING 0.8F

using namespace std;

class CSetting {
private:
	pair<int, int> saveInterval;//minimum, maximum
	int deleteInterval;//day
	int timerInterval;//ms
	int alarmInterval;//sec
	float threshold;//corner threshold
	float matching;//matching rate
	char mainPath[256];
	char imagePath[256];
	char settingPath[256];
	UINT_PTR timerId;
	clock_t alarmClock, saveClock;
public:
	CSetting();
	void setSaveInterval(pair<int, int> savedelay);
	void setTimerInterval(int timerinterval);
	void setAlarmInterval(int alarminterval);
	void setTimerId(UINT_PTR id);
	void setThreshold(float th);
	void setMatching(float rate);
	void setAlarmClock(clock_t clk);
	void setsSaveClock(clock_t clk);
	pair<int, int>getSaveInterval();
	int getDeleteInterval();
	int getTimerInterval();
	int getAlarmInterval();
	float getThreshold();
	float getMatching();
	UINT_PTR getTimerId();
	clock_t getAlarmClock();
	clock_t getSaveClock();
	char* getMainPath();
	char* getImagePath();

	void readSetting(const char *var, int *ret, int default);
	void readSetting(const char *var, float *ret, float default);
	void readSetting(const char *var, pair<int, int> *ret, pair<int, int> default);
	void writeSetting(const char *var, int data);
	void writeSetting(const char *var, float data);
	void writeSetting(const char *var, pair<int, int> data);
};

bool dirExists(const string& dirName);
bool deleteDirectory(LPCTSTR RootDir);