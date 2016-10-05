/*
 * Automotive Message Broker Client Library
 *
 * Copyright (C) 2016 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License)
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

#include "libamb-common.h"
#include "libamb-util.h"

EXPORT guchar get_guchar(GVariant *value)
{
	guchar ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "y", &ret);

	g_variant_unref(tmp);
	return ret;
}

EXPORT guint16 get_guint16(GVariant *value)
{
	guint16 ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "q", &ret);

	g_variant_unref(tmp);
	return ret;
}

EXPORT gint32 get_gint32(GVariant *value)
{
	gint32 ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "i", &ret);

	g_variant_unref(tmp);
	return ret;
}

EXPORT guint32 get_guint32(GVariant *value)
{
	guint32 ret;
	GVariant *tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "u", &ret);

	g_variant_unref(tmp);
	return ret;
}

EXPORT gdouble get_gdouble(GVariant * value)
{
	gdouble ret;
	GVariant * tmp;

	g_variant_get(value, "v", &tmp);
	g_variant_get(tmp, "d", &ret);

	g_variant_unref(tmp);
	return ret;
}

EXPORT void amb_free_result(void *retdata)
{
	g_free(retdata);
}
