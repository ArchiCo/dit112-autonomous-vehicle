#ifndef AutonomousCarSystem_h
#define AutonomousCarSystem_h
#include <Smartcar.h>

class Sensors
{
  public:
    void begin(Car*);
    void update();
    void updateSensors();

    int getSpeed();

    unsigned int  getDistanceFront();
    unsigned int  getDistanceFrontSide();
    unsigned int  getDistanceMiddleSide();
    unsigned int  getDistanceRearCorner();
    unsigned int  getDistanceRear();

    unsigned int  getAngularDisplacement();
    unsigned int  getUnsyncAngularDisplacement();
    unsigned long getOdometerLeftDistance();
    unsigned long getOdometerRightDistance();
    unsigned long getOdometerAvgDistance();

    void enableSensors();
    void disableSensors();
    bool isEnabled();

    void sirenOn();
    void sirenOff();
    void enableMonitor();
    void disableMonitor();
    void resetMonitor();
    bool isMonitoring();
    bool isSectorViable();
    bool isClearSector();
    void monitor();
    void debug();

    void updateRearCorner();
    bool passingObstacle();

  private:
    Car *car;
    SR04 sonicFront, sonicFrontSide, sonicRear;
    GP2Y0A21 irMiddleSide, irRearCorner;
    Odometer  odometerLeft  = Odometer(188),
              odometerRight = Odometer(188);
    Gyroscope gyro          = Gyroscope(17);

    bool sensorsEnabled = true;
    unsigned int   distanceFront,
                   distanceRear,
                   distanceRearCorner,
                   distanceMiddleSide,
                   distanceFrontSide,
                   gyroAngle;

    unsigned long  distanceL,
                   distanceR,
                   updateTimer,
                   sectorStart,
                   sectorEnd;

    const char     _TRIGGER_PIN_FRONT      = 45,
                   _TRIGGER_PIN_FRONT_SIDE = 44,
                   _TRIGGER_PIN_REAR       =  5,
                   _ECHO_PIN_FRONT         = 43,
                   _ECHO_PIN_FRONT_SIDE    = 42,
                   _ECHO_PIN_REAR          =  4,
                   _IR_PIN_MIDDLE_SIDE     = A15,
                   _IR_PIN_REAR_CORNER     = A14,
                   _ODOMETER_PIN_L         =  2,
                   _ODOMETER_PIN_R         =  3,
                   _SIREN_PIN              = A0,
                   _CAMERA_SERVO_PIN       =  6;

    bool monitoring = false;
};


class Driver
{
  public:
    void begin(Car*, Sensors*);
    void update();
    void stop();

    void enableAutonomy();
    void disableAutonomy();
    bool isAutonomous();

    void enableAlignment();
    void disableAlignment();
    bool isAligning();

    void setSpeed(int);
    void setAngle(int);
    int  getSpeed();
    int  getAngle();

    void drive(int, int);
    void steer(int);
    void drive(int);
    void halt();

    void driveForward();
    void driveBackward();
    void driveLeft();
    void driveRight();
    void driveForwardLeft();
    void driveBackwardLeft();
    void driveForwardRight();
    void driveBackwardRight();
    void driveCorrectLeft();
    void driveCorrectRight();

    void enableTrackingCourse();
    void disableTrackingCourse();
    bool isTrackingCourse();
    void trackCourse();
    void setCourse(int);

    void enableDriftCorrection();
    void disableDriftCorrection();
    bool isDriftCorrecting();
    void driftCorrection(int, int);

  private:
    Car *car;
    Sensors *sensors;

    int speedValue  = 0,
        manualSpeedValue = speedValue,
        courseValue = 0,
        angleValue  = 0;

    bool autonomous      = false,
         correctingDrift = false,
         trackingCourse  = false,
         aligning        = false;

    void steer();
    void drive();
    void boost();
    void align();
};


class Parking
{
  public:
    void begin(Driver*, Sensors*);
    void initiate();
    void stop();
    void monitor();
    int  getShortestDisplacement();
    void reverseParking();
    bool isParking();

  private:
    Driver *driver;
    Sensors *sensors;

    enum parkingState {
      _IDLING,
      _STARTING,
      _ALIGNMENT,
      _SEARCHING,
      _ENTERING,
      _BACKING,
      _POSITIONING,
      _MEASURING,
      _ALIGNING,
      _STOPPING,
      _WAITING
    };

    enum {
      _MOVE_SPEED = 30,
      _TURN_SPEED = 35,
      _TURN_ANGLE = 90
    };

    char parkingState = _IDLING;

    unsigned int lastDirection;
    unsigned long timer;
    char targetState;

    bool isViable(int);
    unsigned int parkingDirection,
                 course;
    bool isPositioned();
    unsigned int  getNewDisplacement(int, int);
    void parallelParking();
    bool withinRange(int, int, int);

    int getTarget(int, int);
    void changeState(char);
    void start();
    void idle();
    void search();
    void enter();
    void back();
    void position();
    void measure();
    void align();
    void wait(char, unsigned long);
    void wait();

};

class RemoteControl
{
  public:
    void begin(Driver*, Parking*, Sensors*);
    void listen();
    void listenJoystick();
  private:
    Driver  *driver;
    Parking *parking;
    Sensors *sensors;
    char input;
    int joystickInput;
    enum {
      _FORWARD      = 'F',
      _BACK         = 'B',
      _LEFT         = 'L',
      _RIGHT        = 'R',
      _STOP         = 'S',
      _F_LEFT       = 'G',
      _F_RIGHT      = 'I',
      _B_LEFT       = 'H',
      _B_RIGHT      = 'J',
      _SPEED_0      = '0',
      _SPEED_10     = '1',
      _SPEED_20     = '2',
      _SPEED_30     = '3',
      _SPEED_40     = '4',
      _SPEED_50     = '5',
      _SPEED_60     = '6',
      _SPEED_70     = '7',
      _SPEED_80     = '8',
      _SPEED_90     = '9',
      _SPEED_100    = 'q',
      _AUX_1_ON     = 'W',
      _AUX_1_OFF    = 'w',
      _AUX_2_ON     = 'U',
      _AUX_2_OFF    = 'u',
      _AUX_3_ON     = 'V',
      _AUX_3_OFF    = 'v',
      _AUX_4_ON     = 'X',
      _AUX_4_OFF    = 'x',
      _CAMERA_LEFT  = 'l',
      _CAMERA_RIGHT = 'r',

      // Joystick
      _LEFT_X_NEGATIVE  = 110,
      _LEFT_X_NEUTRAL   = 111,
      _LEFT_X_POSITIVE  = 112,
      _LEFT_Y_NEGATIVE  = 120,
      _LEFT_Y_NEUTRAL   = 121,
      _LEFT_Y_POSITIVE  = 122,

      _RIGHT_X_NEGATIVE = 130,
      _RIGHT_X_NEUTRAL  = 131,
      _RIGHT_X_POSITIVE = 132,

      _J_STOP           = 101,
      _J_SIREN_ON       = 103,
      _J_SIREN_OFF      = 104,
      _J_PARKING_ON     = 105,
      _J_PARKING_OFF    = 106,

      _J_FORWARD        = 141,
      _J_BACK           = 142,
      _J_LEFT           = 143,
      _J_RIGHT          = 144,

      // Signs
      _S_BLUE_TRIANGLE  = 191,
      _S_GREEN_SQUARE   = 192,
      _S_RED_RECTANGLE  = 193,
      _S_STOP_SIGN      = 194

      //


    };

    enum {
      _STANDARD,
      _STICK_LEFT_X,
      _STICK_LEFT_Y,
      _STICK_RIGHT_X,
      _STICK_RIGHT_Y
    };

    char controlState = _STANDARD;
    int  pollJoystick();
    void standardScheme(char);
    void manualControl(char);
    void joystickScheme(int);
};

#endif
