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


#include <gnuradio/attributes.h>
#include <cppunit/TestAssert.h>
#include "qa_function_ff.h"
#include <digitizers/function_ff.h>
#include <digitizers/tags.h>
#include <gnuradio/blocks/vector_source_f.h>
#include <gnuradio/blocks/vector_sink_f.h>
#include <gnuradio/top_block.h>
#include "qa_common.h"

namespace gr {
  namespace digitizers {


    struct function_test_flowgraph_t
    {
      gr::top_block_sptr top;
      gr::blocks::vector_source_f::sptr timing;
      function_ff::sptr func;
      gr::blocks::vector_sink_f::sptr ref_sink;
      gr::blocks::vector_sink_f::sptr min_sink;
      gr::blocks::vector_sink_f::sptr max_sink;

      function_test_flowgraph_t(size_t nsamples, function_ff::sptr function, const std::vector<tag_t> &tags=std::vector<tag_t>())
      {
        func = function;

        top = gr::make_top_block("test");
        timing = gr::blocks::vector_source_f::make(std::vector<float>(nsamples), false, 1, tags);
        ref_sink = gr::blocks::vector_sink_f::make();
        min_sink = gr::blocks::vector_sink_f::make();
        max_sink = gr::blocks::vector_sink_f::make();

        top->connect(timing, 0, func, 0);

        top->connect(func, 0, ref_sink, 0);
        top->connect(func, 1, min_sink, 0);
        top->connect(func, 2, max_sink, 0);
      }

      void run()
      {
        top->run();
      }

      void reset()
      {
        ref_sink->reset();
        min_sink->reset();
        max_sink->reset();
        timing->rewind();
      }
    };

    void
    qa_function_ff::test_no_timing()
    {
      std::vector<float> time = { 0.0, 1.0 };
      std::vector<float> ref  = { 1.5, 2.5 };
      std::vector<float> min  = { 1.0, 2.0 };
      std::vector<float> max  = { 2.0, 3.0 };

      auto func = function_ff::make(1);

      size_t nsamples = 10000;
      function_test_flowgraph_t fg(nsamples, func);
      fg.run();

      auto dref = fg.ref_sink->data();
      auto dmin = fg.min_sink->data();
      auto dmax = fg.max_sink->data();

      CPPUNIT_ASSERT_EQUAL(nsamples, dref.size());
      CPPUNIT_ASSERT_EQUAL(nsamples, dmin.size());
      CPPUNIT_ASSERT_EQUAL(nsamples, dmax.size());

      ASSERT_VECTOR_EQUAL(0.0, dref.begin(), dref.end());
      ASSERT_VECTOR_EQUAL(0.0, dmin.begin(), dmin.end());
      ASSERT_VECTOR_EQUAL(0.0, dmax.begin(), dmax.end());

      func->set_function(time, ref, min, max);
      fg.reset();
      fg.run();

      dref = fg.ref_sink->data();
      dmin = fg.min_sink->data();
      dmax = fg.max_sink->data();

      CPPUNIT_ASSERT_EQUAL(nsamples, dref.size());
      CPPUNIT_ASSERT_EQUAL(nsamples, dmin.size());
      CPPUNIT_ASSERT_EQUAL(nsamples, dmax.size());

      ASSERT_VECTOR_EQUAL(ref[0], dref.begin(), dref.end());
      ASSERT_VECTOR_EQUAL(min[0], dmin.begin(), dmin.end());
      ASSERT_VECTOR_EQUAL(max[0], dmax.begin(), dmax.end());
    }

    void
    qa_function_ff::test_function()
    {
      std::vector<float> time = { 0.5, 0.9 };
      std::vector<float> ref  = { 1.5, 2.5 };
      std::vector<float> min  = { 1.0, 2.0 };
      std::vector<float> max  = { 2.0, 3.0 };

      auto func = function_ff::make(1);
      func->set_function(time, ref, min, max);

      size_t nsamples = 10000;
      acq_info_t info {};
      info.timebase = 1.0 / nsamples;

      function_test_flowgraph_t fg(nsamples, func, std::vector<gr::tag_t> { make_acq_info_tag(info) });
      fg.run();

      auto dref = fg.ref_sink->data();
      auto dmin = fg.min_sink->data();
      auto dmax = fg.max_sink->data();

      CPPUNIT_ASSERT_EQUAL(ref[0], dref[0]);
      CPPUNIT_ASSERT_EQUAL(min[0], min[0]);
      CPPUNIT_ASSERT_EQUAL(max[0], max[0]);

      CPPUNIT_ASSERT_EQUAL(ref[1], dref[nsamples - 1]);
      CPPUNIT_ASSERT_EQUAL(min[1], dmin[nsamples - 1]);
      CPPUNIT_ASSERT_EQUAL(max[1], dmax[nsamples - 1]);

      // TODO: check with epsilon, move offset of beam in...
    }

    void
    qa_function_ff::test_decimation()
    {
      // Put test here
    }

  } /* namespace digitizers */
} /* namespace gr */

