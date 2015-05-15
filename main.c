//
//  main.c
//  DaylightSavingTime
//
//  Created by Daniel on 3/19/14.
//  Copyright (c) 2014 Daniel. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct date_time {
    int year, month, day, hour;
};

struct dst_info {
    int year_b, month_b, day_b, hour_b; /* Begin */
    int year_e, month_e, day_e, hour_e; /* End */
};

static void dst_calculate(struct dst_info *info)
{
    time_t tt;
    struct tm t_tt, dst_tt;

    dst_tt.tm_mday = 1;
    dst_tt.tm_mon = 3 - 1;
    dst_tt.tm_year = info->year_b-1900;
    dst_tt.tm_hour = 0;
    dst_tt.tm_min = 0;
    dst_tt.tm_sec = 0;

    tt = mktime(&dst_tt);
    localtime_r(&tt, &t_tt);

    info->month_b = 3;
    info->day_b = (1+(7-(t_tt.tm_wday==0?7:t_tt.tm_wday))+7);
    info->hour_b = 2;

    dst_tt.tm_mday = 1;
    dst_tt.tm_mon = 11 - 1;
    dst_tt.tm_year = info->year_e-1900;
    dst_tt.tm_hour = 0;
    dst_tt.tm_min = 0;
    dst_tt.tm_sec = 0;

    tt = mktime(&dst_tt);
    localtime_r(&tt, &t_tt);

    info->month_e = 11;
    info->day_e= (1+(7-(t_tt.tm_wday==0?7:t_tt.tm_wday)));
    info->hour_e= 2;
}

static int dst_check(struct dst_info *dst_date, struct date_time *now_date)
{
    time_t tt_now, tt_dst_begin, tt_dst_end;
    struct tm t_now, t_dst_begin, t_dst_end;
    
    t_dst_begin.tm_mday = dst_date->day_b;
    t_dst_begin.tm_mon = dst_date->month_b-1;
    t_dst_begin.tm_year = dst_date->year_b-1900;
    t_dst_begin.tm_hour = dst_date->hour_b;
    //printf("%04d/%02d/%02d %02d\n", t_dst_begin.tm_year, t_dst_begin.tm_mon, t_dst_begin.tm_mday, t_dst_begin.tm_hour);
    t_dst_begin.tm_min = 0;
    t_dst_begin.tm_sec = 0;
    
    tt_dst_begin = mktime(&t_dst_begin);
    
    t_dst_end.tm_mday = dst_date->day_e;
    t_dst_end.tm_mon = dst_date->month_e-1;
    t_dst_end.tm_year = dst_date->year_e-1900;
    t_dst_end.tm_hour = dst_date->hour_e;
    //printf("%04d/%02d/%02d %02d\n", t_dst_end.tm_year, t_dst_end.tm_mon, t_dst_end.tm_mday, t_dst_end.tm_hour);
    t_dst_end.tm_min = 0;
    t_dst_end.tm_sec = 0;
    
    tt_dst_end = mktime(&t_dst_end);
    
    if (tt_dst_begin >= tt_dst_end)
        return 0;
    
    t_now.tm_mday = now_date->day;
    t_now.tm_mon = now_date->month-1;
    t_now.tm_year = now_date->year-1900;
    t_now.tm_hour = now_date->hour;
    //printf("%04d/%02d/%02d %02d\n", t_now.tm_year, t_now.tm_mon, t_now.tm_mday, t_now.tm_hour);
    t_now.tm_min = 0;
    t_now.tm_sec = 0;
    
    tt_now = mktime(&t_now);
    
    //printf("dst_begin=%ld, dst_end=%ld, now=%lu\n", tt_dst_begin, tt_dst_end, tt_now);
    
    if ((tt_now < tt_dst_begin) ||
        (tt_now > tt_dst_end))
        return 0;
    
    return 1;
}

#define MAX_DST_TEST_YEARS  10
int main(int argc, const char * argv[])
{
    int i, ret=0;
    struct date_time tnow;
    struct dst_info dst_info[MAX_DST_TEST_YEARS];
    
    tnow.year=2014;
    tnow.month=3;
    tnow.day=11;
    tnow.hour=10;
    
    for (i=0; i<MAX_DST_TEST_YEARS; i++)
    {
        dst_info[i].year_b = dst_info[i].year_e = 2014+i;
        dst_calculate((struct dst_info *)&dst_info[i]);
        
        tnow.year=2014+i;
        
        printf("dst %04d/%02d/%02d %02d to %04d/%02d/%02d %02d\n",
               dst_info[i].year_b, dst_info[i].month_b, dst_info[i].day_b, dst_info[i].hour_b,
               dst_info[i].year_e, dst_info[i].month_e, dst_info[i].day_e, dst_info[i].hour_e);
        
        printf("now %04d/%02d/%02d %02d\n", tnow.year, tnow.month, tnow.day, tnow.hour);
        
        ret = dst_check(&dst_info[i], &tnow);
        
        printf("dst: %d\n", ret);
    }
    
    return 0;
}

