#ifndef XTECHNICAL_ATR_HPP_INCLUDED
#define XTECHNICAL_ATR_HPP_INCLUDED

#include "../xtechnical_common.hpp"
#include "xtechnical_true_range.hpp"

namespace xtechnical {

    /** \brief 平均真实范围 (Average True Range)
     */
    template <typename T, class MA_TYPE>
    class ATR {
    private:
        MA_TYPE 	    ma;
		TrueRange<T> 	tr;
        T output_value = std::numeric_limits<T>::quiet_NaN();
    public:
        ATR() {};

		ATR(const size_t period) : ma(period) {}

		inline int update(const T high, const T low, const T close) noexcept {
            tr.update(high, low, close);
			if (std::isnan(tr.get())) return common::NO_INIT;
			ma.update(tr.get());
			if (std::isnan(ma.get())) return common::NO_INIT;
			output_value = ma.get();
            return common::OK;
        }

		inline int update(const T high, const T low, const T close, T &out) noexcept {
            const int err = update(high, low, close);
			out = output_value;
            return err;
        }

        /** \brief 更新指标状态
         * \param in 	输入信号
         * \return 成功返回 0，否则参见 ErrorType
         */
        inline int update(const T in) noexcept {
            tr.update(in);
			if (std::isnan(tr.get())) return common::NO_INIT;
			ma.update(tr.get());
			if (std::isnan(ma.get())) return common::NO_INIT;
			output_value = ma.get();
            return common::OK;
        }

        /** \brief 更新指标状态
         * \param in    输入信号
         * \param out   输出信号
         * \return 成功返回 0，否则参见 ErrorType
         */
        inline int update(const T in, T &out) noexcept {
            const int err = update(in);
            out = output_value;
            return err;
        }

		inline int test(const T high, const T low, const T close) noexcept {
            tr.test(high, low, close);
			if (std::isnan(tr.get())) return common::NO_INIT;
			ma.test(tr.get());
			if (std::isnan(ma.get())) return common::NO_INIT;
			output_value = ma.get();
            return common::OK;
        }

		inline int test(const T high, const T low, const T close, T &out) noexcept {
            const int err = test(high, low, close);
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
        inline int test(const T in) noexcept {
            tr.test(in);
			if (std::isnan(tr.get())) return common::NO_INIT;
			ma.test(tr.get());
			if (std::isnan(ma.get())) return common::NO_INIT;
			output_value = ma.get();
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
        inline int test(const T in, T &out) noexcept {
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
            output_value = std::numeric_limits<T>::quiet_NaN();
            tr.clear();
            ma.clear();
        }
    }; // ATR

}; // xtechnical

#endif // XTECHNICAL_ATR_HPP_INCLUDED
