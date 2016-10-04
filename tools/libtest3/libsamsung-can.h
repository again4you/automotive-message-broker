
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

struct VehicleOdometer_t{
	gint32	Zone;
	gint32	ValueSequence;
	gdouble	Time;

	guint32	Value;
};

int get_VehicleOdometer_with_zone(struct VehicleOdometer_t **vo, int zone);
void free_VehicleOdometer(struct VehicleOdometer_t *retdata);

void free_result(void *retdata);

int get_GearboxPositionDisplay_with_zone(struct GearboxPositionDisplay_t **vo, int zone);
int get_VehicleSpeed_with_zone(struct VehicleSpeed_t **vo, int zone);
