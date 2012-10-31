#include "MyStrategy.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

using namespace model;
using namespace std;

const double MIN_ANGLE = M_PI / 6.0; // угол в 30 градусов

void MyStrategy::Move(Tank self, World world, model::Move& move) {
    vector<Tank> all_tanks = world.tanks();                     // получим список всех танков
    double min_angle_to_enemy = 1E20;
    size_t selected_tank = all_tanks.size();
    for(size_t i = 0; i < all_tanks.size(); ++i) {              // перебираем танк из списка
        Tank tank = all_tanks[i];
        if (!tank.teammate()) {                                 // в свои танки стрелять не будем :) 
            double angle_to_enemy = fabs(self.GetTurretAngleTo(tank)); // найдем модуль угла от башни до танка
            if (angle_to_enemy < min_angle_to_enemy) {          // выберем минимум
                min_angle_to_enemy = angle_to_enemy;
                selected_tank = i;
            }
        }
    }

    if (selected_tank != all_tanks.size()) {
        double angle_to_enemy = self.GetTurretAngleTo(all_tanks[selected_tank]); // найдем угол от башни до танка

        if (angle_to_enemy > MIN_ANGLE) {         // если угол сильно положительный
            move.set_turret_turn(1.0);              
        } else if (angle_to_enemy < -MIN_ANGLE) {  // если угол сильно отрицательный
            move.set_turret_turn(-1.0);
        } else {
            move.set_fire_type(PREMIUM_PREFERRED); // если угол невелик, то выстрелим
        }
    }
	
	vector<Bonus> all_bonuses = world.bonuses();             // получим список всех бонусов
    double min_dist_to_bonus = 1E20;
    size_t selected_bonus = all_bonuses.size();
    for(size_t i = 0; i < all_bonuses.size(); ++i) {         // перебираем бонус из списка
       Bonus bonus = all_bonuses[i];
       double dist_to_bonus = self.GetDistanceTo(bonus);    // найдем расстояние до бонуса
       if (dist_to_bonus < min_dist_to_bonus) {             // найдем ближайший
         min_dist_to_bonus = dist_to_bonus;
         selected_bonus = i;
       }
    }

    if (selected_bonus != all_bonuses.size()) {
       double angle_to_bonus = self.GetAngleTo(all_bonuses[selected_bonus]); // найдем угол до бонуса

       if (angle_to_bonus > MIN_ANGLE) {         // если угол сильно положительный,
         move.set_left_track_power(0.75);      // то будем разворачиваться,
         move.set_right_track_power(-1.0);        // поставив противоположные силы гусеницам.
       } else if (angle_to_bonus < -MIN_ANGLE) {  // если угол сильно отрицательный,
         move.set_left_track_power(-1.0);         // будем разворачиваться
         move.set_right_track_power(0.75);     // в противоположную сторону.
       } else {
         move.set_left_track_power(1.0);         // если угол не больше 30 градусов
         move.set_right_track_power(1.0);        // поедем максимально быстро вперед 
       }
    }
}

TankType MyStrategy::SelectTank(int tank_index, int team_size) {
    return MEDIUM;
}