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
 * Zone information in Vehicle
 */
enum Zone {
	None = 0,
	Front = 1,
	Middle = 1 << 1,
	Right = 1 << 2,
	Left = 1 << 3,
	Rear = 1 << 4,
	Center = 1 << 5,
	LeftSide = 1 << 6,
	RightSide = 1 << 7,
	FrontSide = 1 << 8,
	BackSide = 1 << 9
};
typedef int ZoneType;

/**
 * Property status changed callback function. This function is invocked
 * when the properties of registered object are changed.
 *
 * @param[in] registered object name
 * @param[in] changed status
 *
 * @see amb_unregister_property_changed_handler(), amb_unregister_property_changed_handler()
 */
typedef void (*AMB_PROPERTY_CHANGED_CALLBACK)(const gchar *objname, GVariant *data);

/**
 * primitive APIs
 */

/**
 * Get all properties from a specific object name
 *
 * @param[out] GList pointer that contains GVariant data
 * @param[in] object name
 * @return 0 on success, negative errno value on error
 *
 * @see amb_release_property_all()
 */
int amb_get_property_all(GList **proplist, const char *obj_name);

/**
 * Get all properties from a specific object name & zone number
 *
 * @param[out] GVariant pointer that contains all properties
 * @param[in] object name
 * @param[in] zone number
 * @return 0 on success, negative errno value on error
 *
 * @see amb_release_property_all_with_zone()
 */
int amb_get_property_all_with_zone(GVariant **proplist, const char *obj_name, ZoneType zone);

/**
 * Set a property value for a specific object name, property name & zone number
 *
 * @param[in] object name to be set
 * @param[in] property name to be set
 * @param[in] zone number to be set
 * @param[in] value to be set
 * @return 0 on success, negative errno value on error
 */
int amb_set_property(const char *obj_name, const char *prop_name, ZoneType zone, GVariant *value);

/**
 * Release allocated memory space from amb_get_property_all() function
 *
 * @param[in] GList pointer from amb_get_property_all() function
 *
 * @see amb_get_property_all()
 */
void amb_release_property_all(GList *proplist);

/**
 * Release allocated memory space from amb_get_property_all_with_zone()
 *
 * @param[in] GVariant pointer from amb_get_property_all_with_zone()
 */
void amb_release_property_all_with_zone(GVariant *proplist);

/**
 * Register property changed handler.
 *
 * @param[in] Object name to be monitored
 * @param[in] zone number to be set
 * @param[in] Callback function when the properties of monitored object
 * are changed.
 * @return 0 on success, negative errno value on error
 *
 * @see amb_unregister_property_changed_handler(), AMB_PROPERTY_CHANGED_CALLBACK */
int amb_register_property_changed_handler(gchar *objname,
					ZoneType zone,
					AMB_PROPERTY_CHANGED_CALLBACK callback);

/**
 * Unregister property changed handler.
 *
 * @param[in] Object name to be monitored
 * @param[in] zone number to be set
 * @return 0 on success, negative errno value on error
 *
 * @see amb_register_property_changed_handler(), AMB_PROPERTY_CHANGED_CALLBACK
 */
int amb_unregister_property_changed_handler(gchar *objname, ZoneType zone);

/**
 * higher APIs
 */

/**
 * Get all object names that supported by Vehicle
 *
 * @param[out] GList pointer that contains supported object
 * @return 0 on success, negative errno value on error
 *
 * @see amb_release_object_list()
 */
int amb_get_object_list(GList **objlist);

/**
 * Release allocated memory space from amb_get_object_list() function
 *
 * @param[in] GVariant pointer from amb_get_object_list() function
 *
 * @see amb_get_object_list()
 */
void amb_release_object_list(GList *objlist);

#ifdef __cplusplus
}
#endif
#endif /* __LIBAMB_CLIENT_H__
 */
