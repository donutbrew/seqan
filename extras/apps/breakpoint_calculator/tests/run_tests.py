#!/usr/bin/env python
"""Execute the tests for breakpoint_calculator.

The golden test outputs are generated by the script generate_outputs.sh.

You have to give the root paths to the source and the binaries as arguments to
the program.  These are the paths to the directory that contains the 'projects'
directory.

Usage:  run_tests.py SOURCE_ROOT_PATH BINARY_ROOT_PATH
"""
import logging
import os.path
import sys

# Automagically add util/py_lib to PYTHONPATH environment variable.
path = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..',
                                    '..', '..', 'util', 'py_lib'))
sys.path.insert(0, path)

import seqan.app_tests as app_tests

def main(source_base, binary_base):
    """Main entry point of the script."""

    print 'Executing test for breakpoint_calculator'
    print '========================='
    print
    
    ph = app_tests.TestPathHelper(
        source_base, binary_base,
        'extras/apps/breakpoint_calculator/tests')  # tests dir

    # ============================================================
    # Auto-detect the binary path.
    # ============================================================

    path_to_program = app_tests.autolocateBinary(
      binary_base, 'extras/apps/breakpoint_calculator', 'breakpoint_calculator')

    # ============================================================
    # Built TestConf list.
    # ============================================================

    # Build list with TestConf objects, analoguely to how the output
    # was generated in generate_outputs.sh.
    conf_list = []

    # ============================================================
    # Run breakpoint_calculator
    # ============================================================

    # pairwise counts, xmfa format
    conf = app_tests.TestConf(
        program=path_to_program,
        redir_stdout=ph.outFile('d2_xmfa.stdout'),
        args=['-d2',
              ph.inFile('alignment.xmfa')],
        to_diff=[(ph.inFile('d2_xmfa.stdout'),
                  ph.outFile('d2_xmfa.stdout'))])
    conf_list.append(conf)

    # pairwise counts, maf format, detailed list of all pairs
    conf = app_tests.TestConf(
        program=path_to_program,
        redir_stdout=ph.outFile('d2_maf.stdout'),
        args=['-d2', '-d',
              ph.inFile('alignment.maf')],
        to_diff=[(ph.inFile('d2_maf.stdout'),
                  ph.outFile('d2_maf.stdout'))])
    conf_list.append(conf)

    # threeway counts, xmfa format, format directly specified
    conf = app_tests.TestConf(
        program=path_to_program,
        redir_stdout=ph.outFile('d3_xmfa.stdout'),
        args=['-d3', '-f', 'xmfa',
              ph.inFile('alignment.xmfa')],
        to_diff=[(ph.inFile('d3_xmfa.stdout'),
                  ph.outFile('d3_xmfa.stdout'))])
    conf_list.append(conf)

    # threeway counts, maf format, detailed list of all triplets
    conf = app_tests.TestConf(
        program=path_to_program,
        redir_stdout=ph.outFile('d3_maf.stdout'),
        args=['-d3', '-d',
              ph.inFile('alignment.maf')],
        to_diff=[(ph.inFile('d3_maf.stdout'),
                  ph.outFile('d3_maf.stdout'))])
    conf_list.append(conf)

    # ============================================================
    # Execute the tests.
    # ============================================================
    failures = 0
    for conf in conf_list:
        res = app_tests.runTest(conf)
        # Output to the user.
        print ' '.join(['breakpoint_calculator'] + conf.args),
        if res:
             print 'OK'
        else:
            failures += 1
            print 'FAILED'

    # Cleanup.
    ph.deleteTempDir()

    print '=============================='
    print '     total tests: %d' % len(conf_list)
    print '    failed tests: %d' % failures
    print 'successful tests: %d' % (len(conf_list) - failures)
    print '=============================='
    # Compute and return return code.
    return failures != 0


if __name__ == '__main__':
    sys.exit(app_tests.main(main))
