#include "iso8601.h"
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

struct Iso8601Test
{
	char *inputstr;
	double posixTs;
};

void Iso8601TestFactory(const char *inputstr, double posixTs, struct Iso8601Test **tests, int *cursor, int maxtests)
{
	struct Iso8601Test *out = (struct Iso8601Test *)malloc(sizeof(struct Iso8601Test));
	int sl = strlen(inputstr);
	out->inputstr = (char *)malloc(sl+1);
	strncpy(out->inputstr, inputstr, sl);
	out->posixTs = posixTs;
	if(*cursor < maxtests)
	{
		tests[*cursor] = out;
		(*cursor) ++;
	}
}

struct Iso8601Test **Iso8601TestCases()
{
	int numTests = 100;
	struct Iso8601Test **tests = (struct Iso8601Test **)malloc(sizeof(struct Iso8601Test *)*numTests);
	for(unsigned int i=0; i<sizeof(tests); i++)
		tests[i] = NULL;
	int cursor = 0;

	//POSIX time generated by:
	//https://coderstoolbox.net/unixtimestamp/
	//or python: calendar.timegm(datetime.parser.parse("2010-02-18T16:23.33").utctimetuple())
	//ISO weeks: https://calendarhome.com/calculate/convert-a-date/

	Iso8601TestFactory("2017-09-11", 1505088000, tests, &cursor, numTests-1);
	
	Iso8601TestFactory("2017-09-11T21:52:13+00:00", 1505166733, tests, &cursor, numTests-1);

	Iso8601TestFactory("2017-09-11T21:52:13Z", 1505166733, tests, &cursor, numTests-1);

	Iso8601TestFactory("20170911T215213Z", 1505166733, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-W05-3", 1265155200, tests, &cursor, numTests-1);

	Iso8601TestFactory("2011-W25-6", 1308960000, tests, &cursor, numTests-1);

	//Subset of test cases from https://www.myintervals.com/blog/2009/05/20/iso-8601-date-validation-that-doesnt-suck/
	Iso8601TestFactory("2009-12T12:34", 1259670840, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009", 1230768000, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19", 1242691200, tests, &cursor, numTests-1);

	Iso8601TestFactory("20090519", 1242691200, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009123", 1241308800, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05", 1241136000, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-123", 1241308800, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-222", 1249862400, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-001", 1230768000, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-W01-1", 1230508800, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-W51-1", 1260748800, tests, &cursor, numTests-1);

	//I don't think this is valid ISO 8601?
	//Iso8601TestFactory("2009-W511");
	//testTimestamps.push_back(1260748800);

	Iso8601TestFactory("2009-W33", 1249862400, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009W511", 1260748800, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19T14:39Z", 1242743940, tests, &cursor, numTests-1);

	Iso8601TestFactory("20090621T0545Z", 1245563100, tests, &cursor, numTests-1);

	Iso8601TestFactory("2007-04-06T00:00", 1175817600, tests, &cursor, numTests-1);

	Iso8601TestFactory("2007-04-05T24:00", 1175817600, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16:23:48.5", 1266510228.5, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16:23.4", 1266510204, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16:23+0600", 1266488580, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16:23:35-0530", 1266530015, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16.23334444", 1266509640, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16.2233344445555", 1266509604.0040004, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16.22333444455555666666", 1266509604.0040004, tests, &cursor, numTests-1);

	//Bad ISO8601 strings
	Iso8601TestFactory("200905", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009367", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2007-04-05T24:50", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-000", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-M511", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009M511", -1, tests, &cursor, numTests-1);

 	Iso8601TestFactory("2009-05-19T14a39r", -1, tests, &cursor, numTests-1);
 
	Iso8601TestFactory("2009-05-19T14:3924", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-0519", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-1914:39", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19 14:", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19r14:39", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19 14a39a22", -1, tests, &cursor, numTests-1);
 
	Iso8601TestFactory("200912-01", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19 14a39a22", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19 14:39:22+06a00", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19 146922.500", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16.5:23.35:48", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16:23.35:48", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16:23.35:48.45", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2009-05-19 14.5.44", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16:23.33.600", -1, tests, &cursor, numTests-1);

	Iso8601TestFactory("2010-02-18T16,25:23:48,444", -1, tests, &cursor, numTests-1);

	return tests;
}

int main()
{
	struct Iso8601Test **tests = Iso8601TestCases();
	
	int errors = 0;
	int cursor = 0;
	while(tests[cursor] != NULL)
	{
		printf("Test case %d: %s\n", cursor, tests[cursor]->inputstr);
		struct tm dt;
		int timezoneMin = -1;
		bool ok = ParseIso8601Datetime(tests[cursor]->inputstr, &dt, &timezoneMin);
		if(!ok)
		{
			if(tests[cursor]->posixTs < 0.0)
				printf("Parse fail as expected\n");
			else
			{
				printf("Parse FAIL as NOT EXPECTED\n");
				errors++;
			}
			cursor ++;
			continue;
		}
		else
		{
			if(tests[cursor]->posixTs < 0.0)
			{
				printf("Parse should have FAILED\n");
				errors ++;
				cursor ++;
				continue;
			}
		}

		//Get timestamp and timezone info separately
		time_t ts = mktime (&dt);
		printf("%d,%d,%d,%d,%d,%d,%d\n", 1900+dt.tm_year, 1+dt.tm_mon, dt.tm_mday, \
			dt.tm_hour, dt.tm_min, dt.tm_sec, timezoneMin);

		//Get UTC time
		TmToUtc(&dt, timezoneMin);
		ts = mktime (&dt);

		printf("UTC %s", ctime(&ts));
		printf("result %ld = correct %ld", (int64_t)ts, (int64_t)round(tests[cursor]->posixTs)); //UTC unix time-stamp
		if(fabs((int64_t)ts - round(tests[cursor]->posixTs))>1e-6)
		{
			printf(" FAIL");
			errors ++;
		}
		printf("\n");
		cursor ++;
	}
	printf("Errors %d\n", errors);
}

