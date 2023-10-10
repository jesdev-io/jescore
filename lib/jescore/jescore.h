#ifndef _JESCORE_H_
#define _JESCORE_H_

#include "core.h"
#include "err.h"

err_t jes_init();
err_t register_job(const char* name,
                   uint32_t mem_size,
                   uint8_t priority,
                   void (*function)(void* p));

#ifdef JES_DISABLE_CLI
err_t launch_job(const char* name);
#endif

#endif