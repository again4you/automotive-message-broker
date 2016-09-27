/*
 * Automotive Message Broker Client Library
 *
 * Copyright (C) 2016 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LIBAMB_CLIENT_H__
#define __LIBAMB_CLIENT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <glib.h>

/**
 * primitive APIs 
 */


/**
 * Get all properties from a specific object name
 *
 * @param[in] object name 
 * @return GList pointer that contains GVariant data 
 */
int amb_get_property_all(GList **proplist, const char *obj_name);

/**
 * Get all properties from a specific object name & zone number
 *
 * @param[in] object name
 * @param[in] zone number
 */
// GVariant *get_property_all_with_zone(const char *obj_name, int zone);
int amb_get_property_all_with_zone(GVariant **proplist, const char *obj_name, int zone);


/**
 * Set a property value for a specific object name, property name & zone number
 *
 * @param[in] object name to be set
 * @param[in] property name to be set
 * @param[in] zone number to be set
 * @param[in] value to be set
 */
int amb_set_property(const char *obj_name, const char *prop_name, int zone, GVariant *value);

/**
 * Release allocated memory space from get_property_all() function
 *
 * @param[in] GList pointer from get_property_all() function
 * @return
 */
void amb_release_property_all(GList *proplist);

/**
 * Release allocated memory space from release_property_all_with_zone()
 *
 *
 *
 */
void amb_release_property_all_with_zone(GVariant *proplist);

/**
 * Release allocated memory space from get_object_list() function
 *
 *
 *
 *
 */
void amb_release_object_list(GList *objlist);

/**
 * higher APIs
 */

/**
 * Get all object names that supported by Vehicle 
 * 
 */
int amb_get_object_list(GList **objlist);

typedef void (*callback)(gchar *objname, GVariant *data);

int amb_register_signal_handler(const gchar *objname, callback c);
int amb_unregister_signal_handler();



#ifdef __cplusplus
}
#endif
#endif /* __LIBAMB_CLIENT_H__
 */
