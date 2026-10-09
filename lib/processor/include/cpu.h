#pragma once
#include "processes.h"

extern long long moment;

void cpu(process *p);
long long arrival_generator();
void tick();