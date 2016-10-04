


struct VehicleOdometer_t{
	gint32	Zone;
	gint32	ValueSequence;
	gdouble	Time;

	guint32	Value;
};

int get_VehicleOdometer_with_zone(struct VehicleOdometer_t **vo, int zone);
void free_VehicleOdometer(struct VehicleOdometer_t *retdata);

void free_result(void *retdata);
