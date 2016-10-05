
enum GearboxPositionDisplay{
	PARKING = (1<<3) | (1<<1), // 10, 0x0A
	DRIVE,			   // 11, 0x0B
	NEUTRAL,
	REVERSE,
};

struct GearboxPositionDisplay_t{
	gint32	Zone;
	gint32	ValueSequence;
	gdouble	Time;

	guchar Value;
};

struct VehicleSpeed_t{
	gint32	Zone;
	gint32	ValueSequence;
	gdouble	Time;

	guint16 Value;
};

#if 1
struct VehicleOdometer_t{
	gint32	Zone;
	gint32	ValueSequence;
	gdouble	Time;

	guint32	Value;
};
#endif

gdouble get_gdouble2(GVariant * value);                                           

#if 1
#define CAN_OBJECT(obj_name, value_type, alias_name) \
	struct obj_name ## Type { \
		gint32	Zone; \
		gint32	ValueSequence; \
		gdouble Time; \
		value_type Value; \
	}; \
	int get_ ## obj_name ## _with_zone(struct obj_name ## Type **retobj, int zone) \
	{ \
		GVariant *variant; \
		GVariantIter *iter; \
		gchar *key; \
		GVariant *value; \
		int ret; \
		struct obj_name ## Type *retdata = NULL; \
		\
		ret = amb_get_property_all_with_zone(&variant, #obj_name, zone); \
		if (ret < 0) { \
			return ret; \
		} \
		\
		retdata = g_malloc0(sizeof(*retdata)); \
		if (!retdata) { \
			return -1; \
		} \
		\
		g_variant_get(variant, "(a{sv})", &iter); \
		while(g_variant_iter_loop(iter, "{sv}", &key, &value)) { \
			if (!g_strcmp0(key, "Zone")) { \
				retdata->Zone = get_gint32(value); \
			} else if (!g_strcmp0(key, "ValueSequence")) { \
				retdata->ValueSequence = get_gint32(value); \
			} else if (!g_strcmp0(key, "Time")) { \
				retdata->Time = get_gdouble2(value); \
			} else if (!g_strcmp0(key, "Value")) { \
				retdata->Value = get_guint32(value); \
			} \
		} \
		\
		g_variant_iter_free(iter); \
		amb_release_property_all_with_zone(variant); \
		*retobj = retdata; \
		return 0; \
	}
#endif


#if 0
int get_VehicleOdometer_with_zone(struct VehicleOdometer_t **vo, int zone);
void free_VehicleOdometer(struct VehicleOdometer_t *retdata);
#endif

void free_result(void *retdata);

int get_GearboxPositionDisplay_with_zone(struct GearboxPositionDisplay_t **vo, int zone);
int get_VehicleSpeed_with_zone(struct VehicleSpeed_t **vo, int zone);
