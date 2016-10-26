#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libamb-client.h>
#include <libsamsung-can.h>

void test_CheckSeatHeaterL()
{
	struct CheckSeatHeaterLType *p;
	int ret = amb_get_CheckSeatHeaterL_with_zone(&p, 0);
	if (ret != 0) {
		fprintf(stderr, "Fail to %s\n", __func__);
		return ;
	}
	fprintf(stderr, "Zone: %d\n", p->Zone);
	fprintf(stderr, "Value: %d\n", p->Value);
	fprintf(stderr, "ValueSequence: %d\n", p->ValueSequence);
	fprintf(stderr, "Time: %f\n", p->Time);

	amb_free_result(p);
	return ;
}

static void test_set_CheckSeatHeaterL(int value)
{
	int rc;

	rc = amb_set_CheckSeatHeaterL_with_zone(value, 0);
	if (rc < 0) {
		fprintf(stderr, "Fail to amb_set_CheckSeatHeaterL_with_zone(): %d\n", rc);
		return ;
	}
}

static void test_set_VehicleSpeed(guint16 value)
{
	int rc;

	rc = amb_set_VehicleSpeed_with_zone(value, 0);
	if (rc < 0) {
		fprintf(stderr, "Fail to amb_set_CheckSeatHeaterL_with_zone(): %d\n", rc);
		return ;
	}
}
int main()
{
	test_set_CheckSeatHeaterL(0);
	test_CheckSeatHeaterL();

	test_set_CheckSeatHeaterL(3);
	test_CheckSeatHeaterL();

	return 0;
}
