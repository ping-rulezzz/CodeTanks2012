#include "MyStrategy.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

using namespace model;
using namespace std;

const double MIN_ANGLE = M_PI / 6.0; // ���� � 30 ��������

void MyStrategy::Move(Tank self, World world, model::Move& move) {
    vector<Tank> all_tanks = world.tanks();                     // ������� ������ ���� ������
    double min_angle_to_enemy = 1E20;
    size_t selected_tank = all_tanks.size();
    for(size_t i = 0; i < all_tanks.size(); ++i) {              // ���������� ���� �� ������
        Tank tank = all_tanks[i];
        if (!tank.teammate()) {                                 // � ���� ����� �������� �� ����� :) 
            double angle_to_enemy = fabs(self.GetTurretAngleTo(tank)); // ������ ������ ���� �� ����� �� �����
            if (angle_to_enemy < min_angle_to_enemy) {          // ������� �������
                min_angle_to_enemy = angle_to_enemy;
                selected_tank = i;
            }
        }
    }

    if (selected_tank != all_tanks.size()) {
        double angle_to_enemy = self.GetTurretAngleTo(all_tanks[selected_tank]); // ������ ���� �� ����� �� �����

        if (angle_to_enemy > MIN_ANGLE) {         // ���� ���� ������ �������������
            move.set_turret_turn(1.0);              
        } else if (angle_to_enemy < -MIN_ANGLE) {  // ���� ���� ������ �������������
            move.set_turret_turn(-1.0);
        } else {
            move.set_fire_type(PREMIUM_PREFERRED); // ���� ���� �������, �� ���������
        }
    }
	
	vector<Bonus> all_bonuses = world.bonuses();             // ������� ������ ���� �������
    double min_dist_to_bonus = 1E20;
    size_t selected_bonus = all_bonuses.size();
    for(size_t i = 0; i < all_bonuses.size(); ++i) {         // ���������� ����� �� ������
       Bonus bonus = all_bonuses[i];
       double dist_to_bonus = self.GetDistanceTo(bonus);    // ������ ���������� �� ������
       if (dist_to_bonus < min_dist_to_bonus) {             // ������ ���������
         min_dist_to_bonus = dist_to_bonus;
         selected_bonus = i;
       }
    }

    if (selected_bonus != all_bonuses.size()) {
       double angle_to_bonus = self.GetAngleTo(all_bonuses[selected_bonus]); // ������ ���� �� ������

       if (angle_to_bonus > MIN_ANGLE) {         // ���� ���� ������ �������������,
         move.set_left_track_power(0.75);      // �� ����� ���������������,
         move.set_right_track_power(-1.0);        // �������� ��������������� ���� ���������.
       } else if (angle_to_bonus < -MIN_ANGLE) {  // ���� ���� ������ �������������,
         move.set_left_track_power(-1.0);         // ����� ���������������
         move.set_right_track_power(0.75);     // � ��������������� �������.
       } else {
         move.set_left_track_power(1.0);         // ���� ���� �� ������ 30 ��������
         move.set_right_track_power(1.0);        // ������ ����������� ������ ������ 
       }
    }
}

TankType MyStrategy::SelectTank(int tank_index, int team_size) {
    return MEDIUM;
}