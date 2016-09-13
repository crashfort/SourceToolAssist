#if defined TIME_INCLUDED
#endinput
#endif
#define TIME_INCLUDED

public void FormatTimeSpan(char[] buffer, int buffersize, float timeinseconds)
{
	int mstime = RoundFloat(timeinseconds * 1000.0);

	int minutes = (mstime / 1000) / 60;
	int seconds = (mstime / 1000) % 60;
	int milli = mstime % 1000;
	
	FormatEx(buffer, buffersize, "%02d:%02d.%03d", minutes, seconds, milli);
}
