#include <glib.h>


/* primitive APIs */
GList *get_property_all(const char *obj_name);
GVariant *get_property_all_with_zone(const char *obj_name, int zone);

/* higher APIs */
GList *get_object_list();
