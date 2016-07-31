/*
 * log.h
 *
 * Created: 31/07/2016 19:29:00
 *  Author: Xevel
 */ 
#ifndef LOG_H_
#define LOG_H_
// Code sample  (needs to be put in every file that uses debug calls):
//---------------------------------------------------------------------------//
//
// #include "log.h"
// #define LOG_TAG        "Test LOG"
// #define LOG_LEVEL     LOG_LEVEL_INFO
//
// int main(int argc, char **argv)
// {
//       LOG_DEBUG("hello debug");
//       LOG_INFO ("hello info");
//        return 0;
// }
//
//---------------------------------------------------------------------------//

#include "asf.h"
#include "term.h"
#include <string.h>

//---------------------------------------------------------------------------//
/* USERS parameters */

#define LOG_ENABLE // comment this line to completely disable log output

#define FILENAME_SHORT // use short file name without complete path

/* dont change anything bellow this line */
//---------------------------------------------------------------------------//

#ifdef FILENAME_SHORT
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
#define __FILENAME__  __FILE__
#endif // FILENAME_SHORT


#define LOG_LEVEL_USER      (1<<2)
#define LOG_LEVEL_DEBUG     (1<<4)
#define LOG_LEVEL_INFO      (1<<3)
#define LOG_LEVEL_WARNING   (1<<2)
#define LOG_LEVEL_ERROR     (1<<1)
#define LOG_LEVEL_FATAL     (1<<0)
#define LOG_LEVEL_NONE      (0<<0)


#define LOG_STRING_USER     "[USER]\t"
#define LOG_STRING_DEBUG    "[" COLOR_BLUE "DEBUG" COLOR_NC "]\t"
#define LOG_STRING_INFO     "[INFO]\t"
#define LOG_STRING_WARNING  "[WARNING]\t"
#define LOG_STRING_ERROR    "[" COLOR_RED "ERROR" COLOR_NC "]\t"
#define LOG_STRING_FATAL    "[" COLOR_RED "FATAL" COLOR_NC "]\t"
#define LOG_STRING_SYSTEM   "[" COLOR_GREEN "SYS" COLOR_NC "]\t"

#define TEST_LOG_USER()    if( LOG_LEVEL_USER      <=  (LOG_LEVEL ))
#define TEST_LOG_DEBUG()   if( LOG_LEVEL_DEBUG     <=  (LOG_LEVEL ))
#define TEST_LOG_INFO()    if( LOG_LEVEL_INFO      <=  (LOG_LEVEL ))
#define TEST_LOG_WARNING() if( LOG_LEVEL_WARNING   <=  (LOG_LEVEL ))
#define TEST_LOG_ERROR()   if( LOG_LEVEL_ERROR     <=  (LOG_LEVEL ))
#define TEST_LOG_FATAL()   if( LOG_LEVEL_FATAL     <=  (LOG_LEVEL ))

#define LOG_PRINT_USER( ... )            \
    printf( __VA_ARGS__ );\
    printf( "\n\r");
#define LOG_PRINT_DEBUG( ... )            \
    printf("[%s]\t %s %s:%d:\t", LOG_TAG, LOG_STRING_DEBUG, __FILENAME__ , __LINE__); \
    printf( __VA_ARGS__ );\
    printf( "\n\r");
#define LOG_PRINT_DEBUG_HEX( data, len )            \
    printf("[%s]\t %s %s:%d:\t", LOG_TAG, LOG_STRING_DEBUG, __FILENAME__ , __LINE__); \
    uint32_t debug_variable_i = len;            \
    do {        \
        printf("%02x ", ((uint8_t*)data)[len-debug_variable_i--]);\
    } while (debug_variable_i);\
    printf( "\n\r");
#define LOG_PRINT_INFO( ... )            \
    printf("[%s]\t %s %s:%d:\t", LOG_TAG, LOG_STRING_INFO, __FILENAME__, __LINE__); \
    printf( __VA_ARGS__ );\
    printf( "\n\r");
#define LOG_PRINT_WARNING( ... )        \
    printf("[%s]\t %s %s:%d:\t", LOG_TAG, LOG_STRING_WARNING, __FILENAME__, __LINE__); \
    printf( __VA_ARGS__ );\
    printf( "\n\r");
#define LOG_PRINT_ERROR( ... )            \
    printf("[%s]\t %s %s:%d:\t", LOG_TAG, LOG_STRING_ERROR, __FILENAME__, __LINE__); \
    printf( __VA_ARGS__ );\
    printf( "\n\r");
#define LOG_PRINT_PERROR( ... )            \
    {int err = errno;\
        printf("[%s]\t %s %s:%d:\t", LOG_TAG, LOG_STRING_ERROR, __FILENAME__, __LINE__); \
        errno = err;\
        perror( __VA_ARGS__ );\
    }
#define LOG_PRINT_FATAL( ... )            \
    printf("[%s]\t %s %s:%d:\t", LOG_TAG, LOG_STRING_FATAL, __FILENAME__, __LINE__); \
    printf( __VA_ARGS__ ); \
    printf( "\n\r");


#ifdef LOG_ENABLE
#define LOG_USER(...)       TEST_LOG_USER()         {LOG_PRINT_USER     (__VA_ARGS__)}
#define LOG_DEBUG(...)      TEST_LOG_DEBUG()        {LOG_PRINT_DEBUG     (__VA_ARGS__)}
#define LOG_INFO(...)       TEST_LOG_INFO()         {LOG_PRINT_INFO      (__VA_ARGS__)}
#define LOG_WARNING(...)    TEST_LOG_WARNING()      {LOG_PRINT_WARNING    (__VA_ARGS__)}
#define LOG_ERROR(...)      TEST_LOG_ERROR()        {LOG_PRINT_ERROR     (__VA_ARGS__)}
#define LOG_PERROR(...)     TEST_LOG_ERROR()        {LOG_PRINT_PERROR     (__VA_ARGS__)}
#define LOG_FATAL(...)      TEST_LOG_FATAL()        {LOG_PRINT_FATAL    (__VA_ARGS__)}
#define LOG_ENTER()         LOG_DEBUG("function %s called", __FUNCTION__)
#define LOG_EXIT()          LOG_DEBUG("function %s exit", __FUNCTION__)
#else
#define LOG_USER(...)
#define LOG_DEBUG(...)
#define LOG_INFO(...)
#define LOG_WARNING(...)
#define LOG_ERROR(...)
#define LOG_PERROR(...)
#define LOG_FATAL(...)
#define LOG_ENTER()
#define LOG_EXIT()
#endif




#endif /* LOG_H_ */