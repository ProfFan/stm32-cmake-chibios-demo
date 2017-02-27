//
// Created by Fan Jiang on 2017/2/27.
//

#include <ch.h>

extern "C" int __aeabi_atexit(void *obj, void (*dtr)(void *), void *dso_h) {
  return 0;
}

/* Enable if your stub don't provide dso handle symbol */
#if 0
void *__dso_handle = 0;
#endif

extern "C" void __cxa_pure_virtual() {
  while (1)
    ;
}

namespace __gnu_cxx {
  void __verbose_terminate_handler() { while(1) ; }
}

extern "C" int _getpid(void) {
  return 1;
}

extern "C" void _kill(int pid) { while(1) ; }

extern "C" int _exit(int status){
  (void) status;
  chSysHalt("_exit called!");

  while(1){}
}

extern "C" int _end;

static char *end_of_data = (char *) &_end;

/*
 * WARNING: No stack/heap colition check.
 * For check colition and stack growing overflow use SP register value
 * or RAM end limit depeding of processor architecture
 */
extern "C" char *_sbrk (int delta) {
  char *ptr = end_of_data;
  end_of_data += delta;
  return ptr;
}