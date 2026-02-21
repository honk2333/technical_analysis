#ifndef XTECHNICAL_SMA_HPP_INCLUDED
#define XTECHNICAL_SMA_HPP_INCLUDED

#include "../xtechnical_common.hpp"

namespace xtechnical {

    /** \brief 简单移动平均线
     */
    template <typename T>
    class SMA {
    private:
        xtechnical::circular_buffer<T> buffer;
        T last_data = 0;
        T output_value = std::numeric_limits<T>::quiet_NaN();
        size_t period = 0;
    public:
        SMA() {};

        /** \brief 初始化简单移动平均线
         * \param p     周期
         */
        SMA(const size_t p) :
                buffer(p + 1), period(p) {
        }

        /** \brief 更新指标状态
         * \param in 输入信号
         * \return 成功返回 0，否则参见 ErrorType
         */
        int update(const T in) noexcept {
            if(period == 0) {
                output_value = std::numeric_limits<T>::quiet_NaN();
                return common::NO_INIT;
            }
            buffer.update(in);
            if(buffer.full()) {
                last_data = last_data + (in - buffer.front());
                output_value = last_data/(T)period;
            } else {
                last_data += in;
                output_value = std::numeric_limits<T>::quiet_NaN();
                return common::INDICATOR_NOT_READY_TO_WORK;
            }
            return common::OK;
        }

        /** \brief 更新指标状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回 0，否则参见 ErrorType
         */
        int update(const T in, T &out) noexcept {
            const int err = update(in);
            out = output_value;
            return err;
        }

        /** \brief 测试指标
         *
         * 此方法与 update 方法的区别在于
         * 不影响指标的内部状态
         * \param in 输入信号
         * \return 成功返回 0，否则参见 ErrorType
         */
        int test(const T in) noexcept {
            if(period == 0) {
                output_value = std::numeric_limits<T>::quiet_NaN();
                return common::NO_INIT;
            }
            buffer.test(in);
            if(buffer.full()) {
                output_value = (last_data + (in - buffer.front()))/(T)period;
            } else {
                output_value = std::numeric_limits<T>::quiet_NaN();
                return common::INDICATOR_NOT_READY_TO_WORK;
            }
            return common::OK;
        }

        /** \brief 测试指标
         *
         * 此方法与 update 方法的区别在于
         * 不影响指标的内部状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回 0，否则参见 ErrorType
         */
        int test(const T in, T &out) noexcept {
            const int err = test(in);
            out = output_value;
            return err;
        }

        /** \brief 获取指标值
         * \return 指标值
         */
        inline T get() const noexcept {
            return output_value;
        }

        /** \brief 清除指标数据
         */
        inline void clear() noexcept {
            buffer.clear();
            output_value = std::numeric_limits<T>::quiet_NaN();
            last_data = 0;
        }
    };

}; // xtechnical

#endif // XTECHNICAL_SMA_HPP_INCLUDED
