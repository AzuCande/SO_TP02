#ifndef TESTS_H
#define TESTS_H

#include <test_util.h>
#include <memorylib.h>
#include <stdio.h>

void test_mm();
void test_processes();
void test_prio();
void test_sync(int argc, char *argv[]);
void test_no_sync(int argc, char *argv[]);


#endif