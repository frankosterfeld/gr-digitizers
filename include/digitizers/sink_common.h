/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_DIGITIZERS_SINK_COMMON_H
#define INCLUDED_DIGITIZERS_SINK_COMMON_H

#include <digitizers/api.h>
#include <cstdint>
#include <string>

namespace gr {
namespace digitizers {

    /*!
     * \brief Holds static information about the signal.
     *
     * \ingroup digitizers
     */
    struct DIGITIZERS_API signal_metadata_t
    {
      std::string unit;
      std::string name;
    };

    /*!
     * \brief Measurement information.
     *
     * Note, trigger_timestamp is the the same timestamp as provided to the time-realignment block
     * (via the add_timing_event method), else it contains the local timestamp of the first
     * post-trigger sample.
     *
     * Member-variable timestamp contains timestamp of the first pre-trigger sample, or if there is
     * none the timestamp of the first sample.
     *
     * \ingroup digitizers
     */
    struct DIGITIZERS_API measurement_info_t
    {
      double timebase;               // distance between samples (in seconds)
      double user_delay;             // in seconds
      double actual_delay;           // in seconds (including realignment and user delay)

      int64_t  timestamp;            // nanoseconds UTC
      int64_t  trigger_timestamp;    // nanoseconds UTC
      uint32_t status;               // Refer to gr::digitizers::channel_status_t
      uint32_t pre_trigger_samples;
      uint32_t post_trigger_samples;
    };

    /*!
     * \brief Holding information about the timestamp (trigger or acquisition) when the callback
     * is called.
     *
     * Note, trigger_timestamp is the the same timestamp as provided to the time-realignment block
     * (via the add_timing_event method), else it contains the local timestamp of the first
     * post-trigger sample.
     *
     * \ingroup digitizers
     */
    struct DIGITIZERS_API data_available_event_t
    {
      int64_t trigger_timestamp; // trigger timestamp (without any realignment or user delay)
      std::string signal_name;   // signal name
    };

    /*!
     * \brief Callback deceleration
     */
    typedef void (*data_available_cb_t)(const data_available_event_t *evt, void *userdata);

  }
} // namespace gr

#endif /* INCLUDED_DIGITIZERS_SINK_COMMON_H */

