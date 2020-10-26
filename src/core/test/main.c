#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "client.h"
#include "tagset.h"

void testTagsetCreate()
{
    struct client c;
    struct monitor m;

    tagsetCreate(&c.tagset);
    tagsetCreate(&m.tagset);
    c.tagset.selTags[0] = 7;
    m.tagset.selTags[0] = 8;
    assert_false(visibleon(&c, &m));
    tagsetDestroy(&m.tagset);
    tagsetDestroy(&c.tagset);
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(testTagsetCreate),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
