//
// Created by cpasjuste on 18/04/23.
//

#ifndef U_GAMEPAD_TIME_H
#define U_GAMEPAD_TIME_H

namespace uGamePad {
    class Time {
    public:
        Time();

        [[nodiscard]] float asSeconds() const;

        [[nodiscard]] int asMilliseconds() const;

        [[nodiscard]] long asMicroseconds() const;

        static const Time Zero_;

    private:

        friend Time seconds(float);

        friend Time milliseconds(int);

        friend Time microseconds(long);

        explicit Time(long microseconds);

        long m_microseconds;
    };

    Time seconds(float amount);

    Time milliseconds(int amount);

    Time microseconds(long amount);

    bool operator==(Time left, Time right);

    bool operator!=(Time left, Time right);

    bool operator<(Time left, Time right);

    bool operator>(Time left, Time right);

    bool operator<=(Time left, Time right);

    bool operator>=(Time left, Time right);

    Time operator-(Time right);

    Time operator+(Time left, Time right);

    Time &operator+=(Time &left, Time right);

    Time operator-(Time left, Time right);

    Time &operator-=(Time &left, Time right);

    Time operator*(Time left, float right);

    Time operator*(Time left, long right);

    Time operator*(float left, Time right);

    Time operator*(long left, Time right);

    Time &operator*=(Time &left, float right);

    Time &operator*=(Time &left, long right);

    Time operator/(Time left, float right);

    Time operator/(Time left, long right);

    Time &operator/=(Time &left, float right);

    Time &operator/=(Time &left, long right);

    float operator/(Time left, Time right);

    Time operator%(Time left, Time right);

    Time &operator%=(Time &left, Time right);
}

#endif // U_GAMEPAD_TIME_H
