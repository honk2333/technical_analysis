#ifndef XTECHNICAL_DELAY_LINE_HPP_INCLUDED
#define XTECHNICAL_DELAY_LINE_HPP_INCLUDED

#include "../xtechnical_common.hpp"
#include "../xtechnical_circular_buffer.hpp"

namespace xtechnical {

     /** \brief 延迟线
     */
    template <typename T>
    class DelayLine {
    private:
        circular_buffer<T> buffer;
        size_t period = 0;
        T output_value = std::numeric_limits<T>::quiet_NaN();
    public:

        DelayLine() : buffer() {};

        /** \brief 延迟线构造函数
         * \param p     周期
         */
        DelayLine(const size_t p) :
            buffer(p + 1), period(p) {
        }

        /** \brief 更新指标状态
         * \param in    输入信号
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T in) noexcept {
            if(period == 0) {
                output_value = in;
                return common::OK;
            }
            buffer.update(in);
            if(buffer.full()) {
                output_value = buffer.front();
                return common::OK;
            } else {
                output_value = std::numeric_limits<T>::quiet_NaN();
            }
            return common::INDICATOR_NOT_READY_TO_WORK;
        }

        /** \brief 更新指标状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T in, T &out) noexcept {
            const int err = update(in);
            out = output_value;
            return err;
        }

        /** \brief 测试指标
         *
         * 此方法与update不同，
         * 不会影响指标的内部状态
         * \param in    输入信号
         * \return 成功返回0，否则参见ErrorType
         */
        int test(const T in) noexcept {
            if(period == 0) {
                output_value = in;
                return common::OK;
            }
            buffer.test(in);
            if(buffer.full()) {
                output_value = buffer.front();
                return common::OK;
            } else {
                output_value = std::numeric_limits<T>::quiet_NaN();
            }
            return common::INDICATOR_NOT_READY_TO_WORK;
        }

        /** \brief 测试指标
         *
         * 此方法与update不同，
         * 不会影响指标的内部状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        int test(const T in, T &out) noexcept {
            const int err = test(in);
            out = output_value;
            return err;
        }

        inline T get() const noexcept {
            return output_value;
        }

        /** \brief 清除指标数据
         */
        inline void clear() noexcept {
            buffer.clear();
            output_value = std::numeric_limits<T>::quiet_NaN();
        }
    };
}; // xtechnical

#endif // XTECHNICAL_FAST_MIN_MAX_HPP_INCLUDED
