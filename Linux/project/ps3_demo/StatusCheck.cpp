/*
 * StatusCheck.cpp
 *
 *  Created on: 2011. 1. 21.
 *      Author: zerom, Farrell Robotics
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "StatusCheck.h"
#include "Head.h"
#include "Action.h"
#include "Walking.h"
#include "MotionStatus.h"
#include "MotionManager.h"
#include "LinuxActionScript.h"
#include "PS3Controller.h"
#include "PS3BallFollower.h"
#include "LineFollower.h"
#include "RobotFollower.h"
#include "LinuxDARwIn.h"

using namespace Robot;

int StatusCheck::m_cur_mode     = READY;
int StatusCheck::m_old_btn      = 0;
int StatusCheck::m_is_started   = 0;
int StatusCheck::m_current_walk_speed = FAST_WALK;

bool bLJState = false, bRJState = false;
minIni* StatusCheck::m_ini;
minIni* StatusCheck::m_ini1;

//#define Southpaw

void StatusCheck::Check(ArbotixPro &arbotixpro)
{
	int value = 0;

//////////////////////////////////////////////////////////////////////////////////////
// System Standby Toggle
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.PS != 0)
		{
			if (Walking::GetInstance()->IsRunning() == true)
				{
					Walking::GetInstance()->Stop();
					while (Walking::GetInstance()->IsRunning() == 1) usleep(8000);
				}
			if (ToggleRobotStandby() == 1)
				// wait for key release
				while (PS3.key.PS != 0) usleep(8000);
		}
	if (robotInStandby == 1) return;



//////////////////////////////////////////////////////////////////////////////////////
// IMU AUTO GETUP ROUTINE
//////////////////////////////////////////////////////////////////////////////////////
	/*
		 if(MotionStatus::FALLEN != STANDUP && (m_cur_mode == SOCCER) && m_is_started == 1)
	   	{
	     Walking::GetInstance()->Stop();

		 	while(Walking::GetInstance()->IsRunning() == 1) usleep(8000);

	     Action::GetInstance()->m_Joint.SetEnableBody(true, true);

	    if(MotionStatus::FALLEN == FORWARD)
	        Action::GetInstance()->Start(1);   // FORWARD GETUP 10
	    else if(MotionStatus::FALLEN == BACKWARD)
	        Action::GetInstance()->Start(1);   // BACKWARD GETUP 11
	     while(Action::GetInstance()->IsRunning() == 1) usleep(8000);

	    Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
	    Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);
			}
	*/


//////////////////////////////////////////////////////////////////////////////////////
// Shut it down, sit down.
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.Cross != 0)
		{
			Walking::GetInstance()->Stop();
			while (Walking::GetInstance()->IsRunning() == 1) usleep(8000);
			m_is_started    = 0;
			m_cur_mode      = SITTING;
			LinuxActionScript::m_stop = 1;


			Walking::GetInstance()->m_Joint.SetEnableBody(false);
			Action::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true);
			while (Action::GetInstance()->Start(16) == false) usleep(8000);
			while (Action::GetInstance()->IsRunning() == true) usleep(8000);
			Head::GetInstance()->m_Joint.SetEnableHeadOnly(true);
			while (PS3.key.Cross != 0) usleep(8000);
		}


//////////////////////////////////////////////////////////////////////////////////////
// Start Walk Ready
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.Triangle != 0)
		{
			if (m_is_started == 0)
				{
					arbotixpro.DXLPowerOn(true);
				}

			Walking::GetInstance()->Stop();
			while (Walking::GetInstance()->IsRunning() == 1) usleep(8000);
			int lastMode = m_cur_mode;
			m_cur_mode = SOCCER;
			MotionManager::GetInstance()->Reinitialize();
			MotionManager::GetInstance()->SetEnable(true);
			m_is_started = 1;
			bLJState = bRJState = false;
			Head::GetInstance()->m_Joint.SetEnableBody(false);
			Walking::GetInstance()->m_Joint.SetEnableBody(false);
			Action::GetInstance()->m_Joint.SetEnableBody(true);

			if (lastMode == SITTING)
				Action::GetInstance()->Start(8); //50
			else
				Action::GetInstance()->Start(9); //9
			while (Action::GetInstance()->IsRunning() == true) usleep(8000);

			Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true);
			Action::GetInstance()->m_Joint.SetEnableBody(false);
			usleep(500);
			Head::GetInstance()->m_Joint.SetEnableHeadOnly(true);
			while (PS3.key.Triangle != 0) usleep(8000);
		}


//////////////////////////////////////////////////////////////////////////////////////
// Action Script Button Assignment
//////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////
// Select
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.Select != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_SELECT);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.Select != 0) usleep(8000);
		}




//////////////////////////////////////////////////////////////////////////////////////
// Square
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.Square != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_SQUARE);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.Square != 0) usleep(8000);
		}


//////////////////////////////////////////////////////////////////////////////////////
// Circle
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.Circle != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_CIRCLE);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.Circle != 0) usleep(8000);
		}


//////////////////////////////////////////////////////////////////////////////////////
// R1
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.R1 != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_R1);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.R1 != 0) usleep(8000);
		}


//////////////////////////////////////////////////////////////////////////////////////
// R2
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.R2 != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_R2);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.R2 != 0) usleep(8000);
		}



//////////////////////////////////////////////////////////////////////////////////////
// L1
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.L1 != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_L1);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.L1 != 0) usleep(8000);
		}


//////////////////////////////////////////////////////////////////////////////////////
// L2
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.L2 != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_L2);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.L2 != 0) usleep(8000);
		}


//////////////////////////////////////////////////////////////////////////////////////
// Start
//////////////////////////////////////////////////////////////////////////////////////
	if (PS3.key.Start != 0)
		{
			if (LinuxActionScript::m_is_running == 0)
				{
					m_cur_mode = SOCCER;
					LinuxActionScript::m_stop = 0;
					Head::GetInstance()->m_Joint.SetEnableBody(false);
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					LinuxActionScript::ScriptStart(SCRIPT_FILE_PATH_START);
					while (Action::GetInstance()->IsRunning() == true) usleep(8000);
				}
			while (PS3.key.Select != 0) usleep(8000);
		}




//////////////////////////////////////////////////////////////////////////////////////
// PS3 R/C Control code
//////////////////////////////////////////////////////////////////////////////////////


	if (Walking::GetInstance()->IsRunning() == true)
		{
			int rx = 128, ry = 128;
			int dead_band = 5;
			double FBStep = 0, RLTurn = 0, RLStep = 0, xd, yd;
			static double speedAdjSum = 0;

#ifdef Southpaw
			rx = -(PS3.key.RJoyX - 128);
			ry = -(PS3.key.RJoyY - 128);
#else
			rx = -(PS3.key.LJoyX - 128);
			ry = -(PS3.key.LJoyY - 128);
#endif

//			fprintf(stderr, " (X:%d Y:%d)\n", rx, ry);

			if (abs(rx) > dead_band || abs(ry) > dead_band)
				{
					xd = (double)(rx - dead_band) / 256;
					yd = (double)(ry - dead_band) / 256;
					RLTurn = 60 * xd;
					FBStep = 20 * yd;
//For config_FAST.ini
//				RLTurn = 50*xd;
//				FBStep = 45*yd;

//				fprintf(stderr, " (yd:%.1f)\n", yd);
//				Walking::GetInstance()->HIP_PITCH_OFFSET =
					Walking::GetInstance()->HIP_PITCH_OFFSET_START + yd / 2;
					if (FBStep < 0)
						{
							FBStep = 20 * yd;
						}
					speedAdjSum += yd;

					if (speedAdjSum > Walking::GetInstance()->UPPER_VELADJ_LIMIT)
						{
							speedAdjSum = Walking::GetInstance()->UPPER_VELADJ_LIMIT;
						}

					else if (speedAdjSum < Walking::GetInstance()->LOWER_VELADJ_LIMIT)
						{
							speedAdjSum = Walking::GetInstance()->LOWER_VELADJ_LIMIT;
						}
					else
						{
							speedAdjSum = 0;
						}
				}
			Walking::GetInstance()->speedAdj = speedAdjSum;
//			Walking::GetInstance()->X_OFFSET = Walking::GetInstance()->X_OFFSET_START -
			speedAdjSum;

//			double hip_offset = Walking::GetInstance()->HIP_PITCH_OFFSET;
//			fprintf(stderr, " (hip offset:%.1f)\n", hip_offset);
			Walking::GetInstance()->X_MOVE_AMPLITUDE = FBStep;
			Walking::GetInstance()->Y_MOVE_AMPLITUDE = RLStep;
			Walking::GetInstance()->A_MOVE_AMPLITUDE = RLTurn;
//			fprintf(stderr, " (FB:%.1f RL:%.1f)\n", FBStep, RLTurn);
		}
	else //things only done in auto mode
		{
		}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// STOP WALKING GAIT ENGINE
//////////////////////////////////////////////////////////////////////////////////////

	if (Walking::GetInstance()->IsRunning() == true && PS3.key.Down != 0)
		{
			fprintf(stderr, "STOPPING WALKING GAIT\n");

			Walking::GetInstance()->Stop();
			while (Walking::GetInstance()->IsRunning() == 1) usleep(8000);
		}
//////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////
// START WALKING GAIT ENGINE
//////////////////////////////////////////////////////////////////////////////////////
	if (Walking::GetInstance()->IsRunning() == false && PS3.key.Up != 0)
		{
			if (m_cur_mode == SOCCER)
				{
					fprintf(stderr, "STARTING WALKING GAIT\n");
					Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
					Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0;
					Walking::GetInstance()->Start();
				}
		}
//////////////////////////////////////////////////////////////////////////////////////



	/*
	//////////////////////////////////////////////////////////////////////////////////////
		// toggle head from auto to not

		if(PS3.key.LeftHat != 0)
			{
			if(bLJState == true)
				{
				bLJState = false;
				if(m_cur_mode == SOCCER)
					PS3BallFollower::GetInstance()->bHeadAuto = false;
				else if(m_cur_mode == SITTING)
					LineFollower::GetInstance()->bHeadAuto = false;
				else if(m_cur_mode == LINE_FOLLOWING)
					LineFollower::GetInstance()->bHeadAuto = false;
				else if(m_cur_mode == ROBOT_FOLLOWING)
					RobotFollower::GetInstance()->bHeadAuto = false;

				//double pan,tilt;
				//pan = MotionStatus::m_CurrentJoints.GetAngle(JointData::ID_HEAD_PAN);
				//tilt = MotionStatus::m_CurrentJoints.GetAngle(JointData::ID_HEAD_TILT);
				//Head::GetInstance()->MoveByAngle(pan,tilt);
				}
			else
				{
				bLJState = true;
				if(m_cur_mode == SOCCER)
					{
					if(PS3BallFollower::GetInstance()->bFullAuto == true)
						PS3BallFollower::GetInstance()->bHeadAuto = true;
					else
						PS3BallFollower::GetInstance()->bHeadAuto = false;
					}
				else if(m_cur_mode == SITTING)
					{
					LineFollower::GetInstance()->bHeadAuto = true;
					}
				else if(m_cur_mode == LINE_FOLLOWING)
					{
					if(LineFollower::GetInstance()->bFullAuto == true)
						LineFollower::GetInstance()->bHeadAuto = true;
					else
						LineFollower::GetInstance()->bHeadAuto = false;
					}
				else if(m_cur_mode == ROBOT_FOLLOWING)
					{
					if(RobotFollower::GetInstance()->bFullAuto == true)
						RobotFollower::GetInstance()->bHeadAuto = true;
					else
						RobotFollower::GetInstance()->bHeadAuto = false;
					}
				}
			PS3Vibrate();
			// wait for key release
			while(PS3.key.LeftHat != 0) usleep(8000);
			}

	*/

//////////////////////////////////////////////////////////////////////////////////////
// PS3 Head Control
//////////////////////////////////////////////////////////////////////////////////////

	if ((PS3BallFollower::GetInstance()->bHeadAuto == false && (m_cur_mode == SOCCER || m_cur_mode == SITTING)) )
		{
			int lx = 128, ly = 128;
			int dead_band = 5;
			double pan, tilt;
			pan = MotionStatus::m_CurrentJoints.GetAngle(JointData::ID_HEAD_PAN);
			tilt = MotionStatus::m_CurrentJoints.GetAngle(JointData::ID_HEAD_TILT);
			Point2D pos = Point2D(pan, tilt);

#ifdef Southpaw
			lx = -(PS3.key.LJoyX - 128);
			ly = -(PS3.key.LJoyY - 128);
#else
			lx = -(PS3.key.RJoyX - 128);
			ly = -(PS3.key.RJoyY - 128);
#endif

			if (abs(lx) > dead_band || abs(ly) > dead_band)
				{
					pos.X = pan + 0.2 * Camera::VIEW_V_ANGLE * (lx - dead_band) / 256;
					pos.Y = tilt + 0.2 * Camera::VIEW_H_ANGLE * (ly - dead_band) / 256;
				}
			Head::GetInstance()->MoveByAngle(pos.X, pos.Y);
			//Head::GetInstance()->MoveTracking(pos);
		}



//////////////////////////////////////////////////////////////////////////////////////


}



void StatusCheck::mPlay(int motion_page, int mode, int wait)
{
	Walking::GetInstance()->Stop();
	while (Walking::GetInstance()->IsRunning() == 1) usleep(8000);
	m_cur_mode = mode;
	MotionManager::GetInstance()->Reinitialize();
	MotionManager::GetInstance()->SetEnable(true);
	m_is_started = 1;

	Action::GetInstance()->m_Joint.SetEnableBody(true, true);

	Action::GetInstance()->Start(motion_page);
	if (wait == WAIT)
		{
			while (Action::GetInstance()->IsRunning() == true) usleep(8000);
			if (mode != SITTING && mode != STAIRS)
				{
					Head::GetInstance()->m_Joint.SetEnableHeadOnly(true, true);
					Walking::GetInstance()->m_Joint.SetEnableBodyWithoutHead(true, true);
				}
		}
	return;
}
