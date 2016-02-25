#include "../include/Car.h"

Car::Car(string name) {
	m_name = name;
}

Car::~Car() {}

void Car::adjust_direction() {
	if (m_map.compare_current2next_direction('x') == 0) {
		if (m_map.compare_current2next_direction('y') == -1) {
			switch (m_map.get_current_direction()) {
				case d_east:
					break;
				case d_west:
					m_motor.turn_180();
					break;
				case d_south:
					m_motor.turn_left();
					break;
				case d_north:
					m_motor.turn_right();
					break;
				default:
					break;
			}
			m_map.set_current_direction(d_east);
			printf("Current direction: EAST\n");
		} else {
			switch (m_map.get_current_direction()) {
				case d_east:
					m_motor.turn_180();
					break;
				case d_west:
					break;
				case d_south:
					m_motor.turn_right();
					break;
				case d_north:
					m_motor.turn_left();
					break;
				default:
					break;
			}
			m_map.set_current_direction(d_west);
			printf("Current direction: WEST\n");
		}
	} else if (m_map.compare_current2next_direction('y') == 0) {
		if (m_map.compare_current2next_direction('x') == -1) {
			switch (m_map.get_current_direction()) {
				case d_east:
					m_motor.turn_right();
					break;
				case d_west:
					m_motor.turn_left();
					break;
				case d_south:
					break;
				case d_north:
					m_motor.turn_180();
					break;
				default:
					break;
			}
			m_map.set_current_direction(d_south);
			printf("Current direction: SOUTH\n");
		} else {
			switch (m_map.get_current_direction()) {
				case d_east:
					m_motor.turn_left();
					break;
				case d_west:
					m_motor.turn_right();
					break;
				case d_south:
					m_motor.turn_180();
					break;
				case d_north:
					break;
				default:
					break;
			}
			m_map.set_current_direction(d_north);
			printf("Current direction: NOTRH\n");
		}
	}
}

Map& Car::get_map() {
	return m_map;
}

Motor& Car::get_motor() {
	return m_motor;
}