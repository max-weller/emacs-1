
#include <config.h>

#include "lisp.h"

#include "v8_helper.h"
#include "v8_adapter.h"


DEFUN ("test-javascript", Ftest_javascript, Stest_javascript, 1, 1, "p",
       doc: /* Test function for v8 integration */)
  (Lisp_Object n)
{


    return make_number(42);
}
DEFUN ("test-javascript2", Ftest_javascript2, Stest_javascript2, 1, 1, "p",
       doc: /* Test function for v8 integration */)
    (Lisp_Object n)
{
    init_v8();

    return Qnil;
}


void syms_of_v8() {
    defsubr(&Stest_javascript);
    defsubr(&Stest_javascript2);
    syms_of_v8_b();
}

