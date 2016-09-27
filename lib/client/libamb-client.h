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
 * @param[out] GList pointer that contains GVariant data
 * @param[in] object name
 * @return 0 on success, -1 on error
 */
int amb_get_property_all(GList **proplist, const char *obj_name);

/**
 * Get all properties from a specific object name & zone number
 *
 * @param[out] GVariant pointer that contains all properties
 * @param[in] object name
 * @param[in] zone number
 * @return 0 on success, -1 on error
 */
int amb_get_property_all_with_zone(GVariant **proplist, const char *obj_name, int zone);


/**
 * Set a property value for a specific object name, property name & zone number
 *
 * @param[in] object name to be set
 * @param[in] property name to be set
 * @param[in] zone number to be set
 * @param[in] value to be set
 * @return 0 on success, -1 on error
 */
int amb_set_property(const char *obj_name, const char *prop_name, int zone, GVariant *value);

/**
 * Release allocated memory space from amb_get_property_all() function
 *
 * @param[in] GList pointer from amb_get_property_all() function
 */
void amb_release_property_all(GList *proplist);

/**
 * Release allocated memory space from amb_get_property_all_with_zone()
 *
 * @param[in] GVariant pointer from amb_get_property_all_with_zone()
 */
void amb_release_property_all_with_zone(GVariant *proplist);

/**
 * higher APIs
 */

/**
 * Get all object names that supported by Vehicle
 *
 * @param[out] GList pointer that contains supported object
 * @return 0 on success, -1 on error
 */
int amb_get_object_list(GList **objlist);

/**
 * Release allocated memory space from amb_get_object_list() function
 *
 * @param[in] GVariant pointer from amb_get_object_list() function
 */
void amb_release_object_list(GList *objlist);

#ifdef __cplusplus
}
#endif
#endif /* __LIBAMB_CLIENT_H__
 */
