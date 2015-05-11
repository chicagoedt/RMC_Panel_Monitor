#ifndef INPUTMONITOR_H
#define INPUTMONITOR_H

#include <QThread>
#include <SDL2/SDL.h>

class InputUpdate
{
        friend class InputMonitor;

    public:
        struct Axis
        {
            friend class InputMonitor;

            public:
                Axis(void) : _x(0.0f), _y(0.0f) {}

                inline int X(void) const { return _x; }
                inline int Y(void) const { return _y; }

            private:
                int _x;
                int _y;
        };

        InputUpdate(void) : _btnState(0) {}

        enum eState
        {
           eOFF,
           eON
        };

        inline const Axis&   AxisLeft(void)  const { return _axisLeft; }
        inline const Axis&   AxisRight(void) const { return _axisRight; }
                     eState  BtnState(unsigned char buttonID) const;

    private:
        Axis          _axisLeft;
        Axis          _axisRight;
        unsigned char _btnState;
};

class InputMonitor : public QThread
{
    Q_OBJECT

        const int DEAD_ZONE;

    public:
            explicit InputMonitor(QObject* parent = 0L);
            virtual ~InputMonitor();

        enum eStatus
        {
            eOK,
            eERROR
        };

     signals:
        void    DeviceConnected(const QString& label);
        void    DeviceDisconnected(void);
        void    DeviceStatusUpdate(const InputMonitor::eStatus& status, const QString& message);
        void    DeviceUpdate(const InputUpdate& state);

     public slots:
        void    UpdateRateChanged(unsigned int ms) ;

     protected:
        void    run(void) Q_DECL_OVERRIDE;

     private:
        void            Initialize(void);
        SDL_Joystick*   SelectController(void);
        void            HandleController(void);
        void            OnControllerButtonEvent( const SDL_ControllerButtonEvent& event );
        void            OnControllerAxisEvent( const SDL_ControllerAxisEvent& event );
        void            OnJoystickAxisEvent( const SDL_JoyAxisEvent& event);
        void            OnJoystickButtonEvent( const SDL_JoyButtonEvent& event);
        void            AddControllerEvent( const SDL_ControllerDeviceEvent& event );
        void            RemoveControllerEvent( const SDL_ControllerDeviceEvent& event );
        void            AddJoystickEvent( const SDL_JoyDeviceEvent& event);
        void            RemoveJoystickEvent( const SDL_JoyDeviceEvent& event);

     private:
        int         _sleepRate;
        InputUpdate _currentState;
};

#endif // INPUTMONITOR_H
