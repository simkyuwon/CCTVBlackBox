#include "pch.h"
#include "setting.h"
#include "imageProcessing.h"

CSetting::CSetting() {
	sprintf_s(mainPath, "C:\\Program Files\\CCTVBlackBox");
	if (!dirExists(mainPath)) {
		_mkdir(mainPath);
	}
	sprintf_s(imagePath, "%s\\image", mainPath);
	if (!dirExists(imagePath)) {
		_mkdir(imagePath);
	}
	sprintf_s(settingPath, "%s\\setting.ini", mainPath);

	readSetting("timerInterval", &timerInterval, DEFAULT_TIMER_INTERVAL);
	readSetting("alarmInterval", &alarmInterval, DEFAULT_ALARM_INTERVAL);
	readSetting("deleteInterval", &deleteInterval, DEFAULT_DELETE_INTERVAL);
	readSetting("saveInterval", &saveInterval, DEFAULT_SAVE_INTERVAL);
	readSetting("threshold", &threshold, DEFAULT_THRESHOLD);
	readSetting("matching", &matching, DEFAULT_MATCHING);

	saveClock = alarmClock = clock();
}

void CSetting::setSaveInterval(pair<int, int> saveinterval) {
	saveInterval = saveinterval;
	writeSetting("saveInterval", saveInterval);
}

void CSetting::setTimerInterval(int timerinterval) {
	timerInterval = timerinterval;
	writeSetting("timerInterval", timerInterval);
	KillTimer(NULL, getTimerId());
	setTimerId(SetTimer(NULL, 0, getTimerInterval(), (TIMERPROC)&screenCapture));
}

void CSetting::setAlarmInterval(int alarminterval) {
	alarmInterval = alarminterval;
	writeSetting("alarmInterval", alarmInterval);
}

void CSetting::setThreshold(float th) {
	threshold = th;
	writeSetting("threshold", threshold);
}

void CSetting::setMatching(float rate) {
	matching = rate;
	writeSetting("matching", matching);
}

void CSetting::setTimerId(UINT_PTR id) {
	timerId = id;
}

void CSetting::setAlarmClock(clock_t clk) {
	alarmClock = clk;
}

void CSetting::setsSaveClock(clock_t clk) {
	saveClock = clk;
}

void CSetting::setDebug(bool chk) {
	debug = chk;
}

pair<int, int> CSetting::getSaveInterval() {
	return saveInterval;
}

int CSetting::getDeleteInterval() {
	return deleteInterval;
}
int CSetting::getTimerInterval() {
	return timerInterval;
}

int CSetting::getAlarmInterval() {
	return alarmInterval;
}

float CSetting::getThreshold() {
	return threshold;
}

float CSetting::getMatching() {
	return matching;
}

UINT_PTR CSetting::getTimerId() {
	return timerId;
}

clock_t CSetting::getAlarmClock() {
	return alarmClock;
}

clock_t CSetting::getSaveClock() {
	return saveClock;
}

char* CSetting::getMainPath() {
	return mainPath;
}

char* CSetting::getImagePath() {
	return imagePath;
}

bool CSetting::getDebug() {
	return debug;
}

void CSetting::readSetting(const char *var, int *ret, int default) {
	char buf[32];
	if (!GetPrivateProfileStringA("variable", var, NULL, buf, sizeof(buf), settingPath)) {
		writeSetting(var, default);
		GetPrivateProfileStringA("variable", var, NULL, buf, sizeof(buf), settingPath);
	}
	*ret = atoi(buf);
}

void CSetting::readSetting(const char* var, float* ret, float default) {
	char buf[32];
	if (!GetPrivateProfileStringA("variable", var, NULL, buf, sizeof(buf), settingPath)) {
		writeSetting(var, default);
		GetPrivateProfileStringA("variable", var, NULL, buf, sizeof(buf), settingPath);
	}
	*ret = (float)atof(buf);
}

void CSetting::readSetting(const char *var, pair<int, int> *ret, pair<int, int> default) {
	char buf[32];
	if (!GetPrivateProfileStringA("variable", var, NULL, buf, sizeof(buf), settingPath)) {
		writeSetting(var, default);
		GetPrivateProfileStringA("variable", var, NULL, buf, sizeof(buf), settingPath);
	}
	sscanf_s(buf, "{%d, %d}", &(ret->first), &(ret->second));
}

void CSetting::writeSetting(const char *var, int data) {
	char buf[32];
	sprintf_s(buf, "%d", data);
	WritePrivateProfileStringA("variable", var, buf, settingPath);
}

void CSetting::writeSetting(const char *var, float data) {
	char buf[32];
	sprintf_s(buf, "%f", data);
	WritePrivateProfileStringA("variable", var, buf, settingPath);

}

void CSetting::writeSetting(const char *var, pair<int, int> data) {
	char buf[32];
	sprintf_s(buf, "{%d, %d}", data.first, data.second);
	WritePrivateProfileStringA("variable", var, buf, settingPath);
}

bool dirExists(const string& dirName) {
	DWORD ftyp = GetFileAttributesA(dirName.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES)
		return false;	//something is wrong with your path!

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;	 // this is a directory!

	return false;		// this is not a directory!
}

bool deleteDirectory(LPCTSTR RootDir) {
	if (RootDir == NULL)
		return FALSE;

	bool ret = FALSE;

	CString szNextDirPath = _T("");
	CString szRoot = _T("");

	szRoot.Format(_T("%s\\*.*"), RootDir);

	CFileFind find;

	ret = find.FindFile(szRoot);

	if (!ret)
		return ret;

	while (ret) {
		ret = find.FindNextFile();

		if (!find.IsDots())
		{
			if (find.IsDirectory())
				deleteDirectory(find.GetFilePath());
			else
				ret = DeleteFile(find.GetFilePath());
		}
	}
	find.Close();
	ret = RemoveDirectory(RootDir);
	return ret;
}