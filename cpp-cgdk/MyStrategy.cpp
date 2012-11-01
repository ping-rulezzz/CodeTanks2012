#include "MyStrategy.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "iostream"

using namespace model;

const double MIN_ANGLE = M_PI / 6.0; // угол в 30 градусов

void MyStrategy::Move(Tank self, World world, model::Move& move) {
    vector<Tank> allTanks = world.tanks();                     
    double minAngleToEnemy = 1E20;
    int selectedTank = GetNearestTankByAngle(self, allTanks, minAngleToEnemy);

    if (selectedTank != -1) {
        double angleToEnemy = minAngleToEnemy;//self.GetTurretAngleTo(allTanks[selectedTank]); // найдем угол от башни до танка

        if (angleToEnemy > MIN_ANGLE) 
		{         // если угол сильно положительный
            move.set_turret_turn(1.0);              
        } 
		else if (angleToEnemy < -MIN_ANGLE) 
		{  // если угол сильно отрицательный
            move.set_turret_turn(-1.0);
        } 
		else 
		{
            move.set_fire_type(PREMIUM_PREFERRED); // если угол невелик, то выстрелим
        }
    }
	
	vector<Bonus> allBonuses = world.bonuses();             // получим список всех бонусов
    double minDistToBonus = 1E20;
    int selectedBonus = GetNearestBonus(self, allBonuses);
	cout << selectedBonus;

    if (selectedBonus != -1) {
       double angleToBonus = self.GetAngleTo(allBonuses[selectedBonus]); // найдем угол до бонуса

	   if ((angleToBonus < MIN_ANGLE && angleToBonus > 0) ||
		   (angleToBonus > -MIN_ANGLE && angleToBonus < 0))
	   {
		   move.set_left_track_power(1.0);        
           move.set_right_track_power(1.0); 
	   }
	   else if ((angleToBonus > M_PI - MIN_ANGLE && angleToBonus > 0) ||
				(angleToBonus < -(M_PI - MIN_ANGLE)  && angleToBonus < 0))
	   {
			move.set_left_track_power(-1.0);         
			move.set_right_track_power(-1.0); 
	   }
	   else if (angleToBonus > MIN_ANGLE) 
	   {         
			move.set_left_track_power(0.75);      
			move.set_right_track_power(-1.0);       
       } 
	   else if (angleToBonus < -MIN_ANGLE) 
	   {  
			move.set_left_track_power(-1.0);         
			move.set_right_track_power(0.75);    
       } 
	   else 
	   {
			move.set_left_track_power(1.0);         
			move.set_right_track_power(1.0);       
       }
    }
}

//bonuses
int MyStrategy::GetNearestBonus(Tank &self,vector<Bonus> &bonuses)
{
	double minDistToBonus = -1;
	int selectedBonus = -1;

	for(size_t i = 0; i < bonuses.size(); ++i)
	{     
		Bonus bonus = bonuses[i];
		double distToBonus = self.GetDistanceTo(bonus);    
		
		if (distToBonus < minDistToBonus || minDistToBonus < 0) 
		{            
			minDistToBonus = minDistToBonus;
			selectedBonus = i;
		}
	}

	return selectedBonus;
}

int MyStrategy::GetNearestBonusByType(Tank &self, vector<Bonus> &bonuses, BonusType type)
{
	double minDistToBonus = -1;
	int selectedBonus = -1;

	for(size_t i = 0; i < bonuses.size(); ++i)
	{     
		Bonus bonus = bonuses[i];
		double distToBonus = self.GetDistanceTo(bonus);    
		
		if ((distToBonus < minDistToBonus || minDistToBonus < 0) && bonus.type() == type) 
		{            
			minDistToBonus = minDistToBonus;
			selectedBonus = i;
		}
	}

	return selectedBonus;
}

//opponents
int MyStrategy::GetNearestTankByAngle(Tank &self, vector<model::Tank> &tanks, double &angleToTarget)
{
	angleToTarget = 999;
    int selectedTank = -1;

    for(size_t i = 0; i < tanks.size(); ++i) 
	{             
        Tank tank = tanks[i];
        bool isAlive = tank.crew_health() > 0 && tank.hull_durability() > 0;

		if (!tank.teammate() && isAlive) 
		{                                
            double anglToEnemy = self.GetTurretAngleTo(tank); 
            
			if (fabs(anglToEnemy) < fabs(angleToTarget) || angleToTarget == 999) 
			{          
                angleToTarget = anglToEnemy;
                selectedTank = i;
            }
        }
    }

	return selectedTank;
}

int MyStrategy::GetNearestTankByDistance(Tank &self, vector<model::Tank> &tanks, double &distanceToTarget)
{
	distanceToTarget = -1;
    int selectedTank = -1;

    for(size_t i = 0; i < tanks.size(); ++i) 
	{             
        Tank tank = tanks[i];
        bool isAlive = tank.crew_health() > 0 && tank.hull_durability() > 0;
		
		if (!tank.teammate() && isAlive) 
		{                                
            double distToEnemy = self.GetDistanceTo(tank);   

			if (distToEnemy < distanceToTarget || distanceToTarget < 0) 
			{          
                distanceToTarget = distToEnemy;
                selectedTank = i;
            }
        }
    }

	return selectedTank;
}

int MyStrategy::GetTankWithMinHP(vector<model::Tank> &tanks, int &targetHP )
{
	targetHP = -1;
	int selectedTank = -1;
	
    for(size_t i = 0; i < tanks.size(); ++i) 
	{             
        Tank tank = tanks[i];
        bool isAlive = tank.crew_health() > 0 && tank.hull_durability() > 0;
		
		if (!tank.teammate() && isAlive) 
		{                                
            int enemyHP = tank.crew_health();   

			if (enemyHP < targetHP || targetHP < 0) 
			{          
                targetHP = enemyHP;
                selectedTank = i;
            }
        }
    }

	return selectedTank;
}

int MyStrategy::GetTankWithMaxHP(vector<model::Tank> &tanks, int &targetHP )
{

	targetHP = -1;
	int selectedTank = -1;
	
    for(size_t i = 0; i < tanks.size(); ++i) 
	{             
        Tank tank = tanks[i];
        bool isAlive = tank.crew_health() > 0 && tank.hull_durability() > 0;
		
		if (!tank.teammate() && isAlive) 
		{                                
            int enemyHP = tank.crew_health();   

			if (enemyHP > targetHP || targetHP < 0) 
			{          
                targetHP = enemyHP;
                selectedTank = i;
            }
        }
    }

	return selectedTank;
}

TankType MyStrategy::SelectTank(int tank_index, int team_size) {
    return MEDIUM;
}