#pragma once
#ifndef MYMATH_H
#define MYMATH_H
#include <algorithm>
#undef max
#undef min
#include <DirectXMath.h>
namespace Lib
{
    class MyMath
    {
    public:
        static const float PIDIV;
        static const float PIDIV2;
        static const float PIDIV4;
        static const float PI2;

        // 引数value_を引数min_〜引数max_の範囲に制限する
        template <class T>
        static T clamp(const T value_, const T max_, const T min_)
        {
            return std::min(std::max(min_, value_), max_);
        }

        // 引数value_がlimit以上の場合value_を0に、また0以下の場合value_をlimit_に
        template <class T>
        static T rollup(const T value_, const T limit_) {
            if (value_ < 0) {
                return limit_;
            }
            else if (value_ > limit_){
                return 0;
            }
            return value_;
        }
    };

    const float MyMath::PIDIV  = 3.141592741f;
    const float MyMath::PIDIV2 = 1.570796371f;
    const float MyMath::PIDIV4 = 0.7853981853f;
    const float MyMath::PI2    = 6.283185482f;
}

#endif