#pragma once

#define LOG_DEBUG(fmt, ...) printf("\n\nfunc(%s),file:%s:%d\nDEBUG >> "##fmt, __FUNCTION__ ,  __FILE__ , __LINE__ , __VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf("\n\nfunc(%s),file:%s:%d\nERROR >> "##fmt, __FUNCTION__ ,  __FILE__ , __LINE__ , __VA_ARGS__)

#define Min(a, b) ((a) < (b) ? (a) : (b))
#define Max(a, b) ((a) > (b) ? (a) : (b))

#define SvrPort 7788

#define GameID 0
#define ResVer 0
