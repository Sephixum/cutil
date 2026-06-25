#include "base/base_include.c"
#include "base/base_include.h"


int main(void) {
  int ballz = (1 << 3);
  ThreadContext *main_thread = ThreadContextAlloc();
  ThreadContextSelect(main_thread);

  Temp a = ScratchBegin(0, 0);
  {
    String8 name = Str8Lit("Nigger");
    printf("%*s", Str8Varg(name));
  }
  ScratchEnd(a);
}
