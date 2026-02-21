#ifndef XTECHNICAL_RSI_HPP_INCLUDED
#define XTECHNICAL_RSI_HPP_INCLUDED

#include "../xtechnical_common.hpp"

namespace xtechnical {

    /** \brief 相对强弱指标 (RSI)
     */
    template <typename T, class MA_TYPE>
    class RSI {
    private:
        MA_TYPE iU;
        MA_TYPE iD;
        bool is_init_ = false;
        bool is_update_ = false;
        T prev_ = 0;
        T output_value = std::numeric_limits<T>::quiet_NaN();
    public:

        RSI() {}

        /** \brief 初始化相对强弱指标
         * \param period 指标周期
         */
        RSI(const size_t period) :
            iU(period), iD(period) {
        }

        /** \brief 初始化相对强弱指标
         * \param period 指标周期
         */
        inline void init(const size_t period) noexcept {
            is_update_ = false;
            iU = MA_TYPE(period);
            iD = MA_TYPE(period);
        }

        /** \brief 更新指标状态
         * \param in 输入信号
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T in) noexcept {
            if(!is_update_) {
                prev_ = in;
                output_value = std::numeric_limits<T>::quiet_NaN();
                is_update_ = true;
                return common::INDICATOR_NOT_READY_TO_WORK;
            }
            T u = 0, d = 0;
            if(prev_ < in) {
                u = in - prev_;
            } else
            if(prev_ > in) {
                d = prev_ - in;
            }
            int erru, errd = 0;
            erru = iU.update(u, u);
            errd = iD.update(d, d);
            prev_ = in;
            if(erru != common::OK || errd != common::OK) {
                output_value = std::numeric_limits<T>::quiet_NaN();
                return common::INDICATOR_NOT_READY_TO_WORK;
            }
            if(d == 0) {
                output_value = 100.0;
                return common::OK;
            }
            T rs = u / d;
            output_value = 100.0 - (100.0 / (1.0 + rs));
            return common::OK;
        }

        /** \brief 更新指标状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        int update(const T in, T &out) noexcept {
            const int err = update(in);
            out = output_value;
            return common::OK;
        }

        /** \brief 测试指标
         *
         * 此函数与update不同，不会影响指标的内部状态
         * \param in 输入信号
         * \return 成功返回0，否则参见ErrorType
         */
        int test(const T in) noexcept {
            if(!is_update_) {
                output_value = std::numeric_limits<T>::quiet_NaN();
                return common::INDICATOR_NOT_READY_TO_WORK;
            }
            T u = 0, d = 0;
            if(prev_ < in) {
                u = in - prev_;
            } else
            if(prev_ > in) {
                d = prev_ - in;
            }
            int erru, errd = 0;
            erru = iU.test(u, u);
            errd = iD.test(d, d);
            if(erru != common::OK || errd != common::OK) {
                output_value = std::numeric_limits<T>::quiet_NaN();
                return common::INDICATOR_NOT_READY_TO_WORK;
            }
            if(d == 0) {
                output_value = 100.0;
                return common::OK;
            }
            T rs = u / d;
            output_value = 100.0 - (100.0 / (1.0 + rs));
            return common::OK;
        }

        /** \brief 测试指标
         *
         * 此函数与update不同，
         * 不会影响指标的内部状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回0，否则参见ErrorType
         */
        int test(const T in, T &out) noexcept {
            const int err = test(in);
            out = output_value;
            return common::OK;
        }

        /** \brief 获取指标值
         * \return 指标值
         */
        inline T get() const noexcept {
            return output_value;
        }

        /** \brief 清除指标数据
         */
        void clear() noexcept {
            output_value = std::numeric_limits<T>::quiet_NaN();
            is_update_ = false;
            iU.clear();
            iD.clear();
        }
    };

}; // xtechnical

#endif // XTECHNICAL_RSI_HPP_INCLUDED
