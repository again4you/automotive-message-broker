#include <stdio.h>

#include <libdbus-client.h>

int main()
{
	GVariant *ret;

    printf("test\n");
	ret = get_object_list();

	g_variant_unref(ret);
    return 0;
}
