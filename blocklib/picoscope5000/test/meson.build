###################################################
#    QA
###################################################

testdeps = [cppunit_dep, gnuradio_blocklib_picoscope5000_dep]
linklibs = ['-lgnuradio-runtime', '-lgnuradio-blocklib-blocks', '-lgnuradio-blocklib-streamops'] # TODO(PORT) only link necessary

qa_picoscope_5000 = executable('qa_picoscope_5000', 'qa_picoscope_5000.cc',
                                dependencies: testdeps,
                                link_args: linklibs)

# TODO only run when explicitly request per cmd-line arg
test('Picoscope 5000', qa_picoscope_5000)
