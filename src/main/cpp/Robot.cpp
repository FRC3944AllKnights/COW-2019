/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/
#include <thread> 

#include <cameraserver/CameraServer.h>
#include <opencv2/core/core.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <wpi/raw_ostream.h>
#include <frc/TimedRobot.h>
#include <frc/Joystick.h>

#include "Drive.h"
#include "NotServo.h"
#include "Pusher.h"
#include "Lifter.h"

/** This is a test
 * This is a demo program showing how to use Mecanum control with the
 * MecanumDrive class.
 */
class Robot : public frc::TimedRobot {
 public:
  void RobotInit() override {
    // Invert the left side motors. You may need to change or remove this to
    // match your robot.
    //frc::CameraServer::GetInstance() -> StartAutomaticCapture("0", 0);
    std::thread visionThread(VisionThread);
    visionThread.detach();
    COWDrive.init();
    Servo.originPos();
  }

  void TeleopPeriodic() override {
    /* Use the joystick X axis for lateral movement, Y axis for forward
     * movement, and Z axis for rotation.
     */
    COWDrive.mechanum(m_stick.GetX(), m_stick.GetY(), m_stick.GetZ(), m_stick.GetThrottle());
    Servo.servoRotate(m_stick.GetTrigger());
    if (m_stick.GetRawButton(6)){
      lifter.manualUp(m_stick.GetRawButton(6));
    }
    else{
      lifter.manualDown(m_stick.GetRawButton(4));
    }
//lifter manual control
    if (m_stick.GetRawButton(5)){
      PushMotor.manualPush(m_stick.GetRawButton(5));
    }
    else{
      PushMotor.manualRetract(m_stick.GetRawButton(3));
    }

  }

 private:
  static void VisionThread() {
    // Get the USB camera from CameraServer
    cs::UsbCamera camera =
        frc::CameraServer::GetInstance()->StartAutomaticCapture("0", 0);
    // Set the resolution
    camera.SetResolution(240, 144);
    // Get a CvSink. This will capture Mats from the Camera
    cs::CvSink cvSink = frc::CameraServer::GetInstance()->GetVideo();
    cs::CvSource outputStream =
        frc::CameraServer::GetInstance()->PutVideo("gray", 240, 144);
    // Mats are very memory expensive. Lets reuse this Mat.
    cv::Mat mat;

    while(true) {
            cvSink.GrabFrame(mat);
            cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
            outputStream.PutFrame(mat);
        }
  }

 //this is where to change variables
  static constexpr int kJoystickChannel = 0;
  Drive COWDrive;
  Lifter lifter;
  NotServo Servo; //names the class notServo - servo, also creates an instance of servo
  Pusher PushMotor;
  frc::Joystick m_stick{kJoystickChannel};
};

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif