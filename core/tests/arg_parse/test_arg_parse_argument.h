// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2013, Knut Reinert, FU Berlin
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Knut Reinert or the FU Berlin nor the names of
//       its contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL KNUT REINERT OR THE FU BERLIN BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
// DAMAGE.
//
// ==========================================================================
// Author: Stephan Aiche <stephan.aiche@fu-berlin.de>
// ==========================================================================
// Tests for arg_parse/arg_parse_argument.h.
// ==========================================================================

#ifndef SEQAN_CORE_TESTS_ARG_PARSE_TEST_ARG_PARSE_ARGUMENT_H_
#define SEQAN_CORE_TESTS_ARG_PARSE_TEST_ARG_PARSE_ARGUMENT_H_

#include <seqan/basic.h>

#include "test_extensions.h"
#include <seqan/arg_parse/arg_parse_argument.h>

using namespace seqan;

SEQAN_DEFINE_TEST(test_argument_string_label)
{
    ArgParseArgument arg1(ArgParseArgument::STRING);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "STR");

    arg1._numberOfValues = 2;
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "STR STR");

    ArgParseArgument arg2(ArgParseArgument::STRING);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg2), "STR");
}

SEQAN_DEFINE_TEST(test_argument_int_label)
{
    ArgParseArgument arg1(ArgParseArgument::INTEGER);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "NUM");

    arg1._numberOfValues = 2;
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "NUM NUM");

    ArgParseArgument arg2(ArgParseArgument::INTEGER);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg2), "NUM");
}

SEQAN_DEFINE_TEST(test_argument_double_label)
{
    ArgParseArgument arg1(ArgParseArgument::DOUBLE);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "NUM");

    arg1._numberOfValues = 2;
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "NUM NUM");

    ArgParseArgument arg2(ArgParseArgument::DOUBLE);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg2), "NUM");
}

SEQAN_DEFINE_TEST(test_argument_inputfile_label)
{
    ArgParseArgument arg1(ArgParseArgument::INPUTFILE);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "FILE");

    arg1._numberOfValues = 2;
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "FILE FILE");

    ArgParseArgument arg2(ArgParseArgument::INPUTFILE);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg2), "FILE");
}

SEQAN_DEFINE_TEST(test_argument_outputfile_label)
{
    ArgParseArgument arg1(ArgParseArgument::OUTPUTFILE);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "FILE");

    arg1._numberOfValues = 2;
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "FILE FILE");

    ArgParseArgument arg2(ArgParseArgument::OUTPUTFILE);
    SEQAN_ASSERT_EQ(getArgumentLabel(arg2), "FILE");
}

SEQAN_DEFINE_TEST(test_argument_user_defined_label)
{
    ArgParseArgument arg1(ArgParseArgument::STRING, "my_label");
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "my_label");

    arg1._numberOfValues = 2;
    SEQAN_ASSERT_EQ(getArgumentLabel(arg1), "my_label");
}

SEQAN_DEFINE_TEST(test_argument_invalid_cast)
{
    ArgParseArgument doubleArg(ArgParseArgument::DOUBLE);

    _checkValue(doubleArg, "6.0221418e23");
    _assignArgumentValue(doubleArg, "6.0221418e23");
    _checkValue(doubleArg);
    SEQAN_ASSERT_EQ(getArgumentValue(doubleArg), "6.0221418e23");

    _checkValue(doubleArg, "-6.022");
    _assignArgumentValue(doubleArg, "-6.022");
    _checkValue(doubleArg);
    SEQAN_ASSERT_EQ(getArgumentValue(doubleArg), "-6.022");

    // Test _checkValue() with value.
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(doubleArg, "-6.022aaa"),
                         "the given value '-6.022aaa' cannot be casted to double");

    // Test _checkValue() after assignment.
    _assignArgumentValue(doubleArg, "-6.022aaa");
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(doubleArg),
                         "the given value '-6.022aaa' cannot be casted to double");
}

SEQAN_DEFINE_TEST(test_argument_min_max_boundaries)
{
    ArgParseArgument arg(ArgParseArgument::DOUBLE);
    setMinValue(arg, "1");
    setMaxValue(arg, "6.0221418e23");

    _checkValue(arg, "1");
    _assignArgumentValue(arg, "1"); // should just work without throwing any exception
    _checkValue(arg);
    SEQAN_ASSERT_EQ(getArgumentValue(arg), "1");
    // Test _checkValue() with argument and value.
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(arg, "-1"),
                         "the given value '-1' is not in the interval [1:6.0221418e23]");
    // Test _checkValue() after assignment.
    _assignArgumentValue(arg, "-1");
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(arg),
                         "the given value '-1' is not in the interval [1:6.0221418e23]");

    // Tests _checkValue() with argument and value.
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(arg, "6.0321418e23"),
                         "the given value '6.0321418e23' is not in the interval [1:6.0221418e23]");
    // Test _checkValue() after assignment.
    _assignArgumentValue(arg, "6.0321418e23");
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(arg),
                         "the given value '6.0321418e23' is not in the interval [1:6.0221418e23]");
}

SEQAN_DEFINE_TEST(test_argument_valid_values)
{
    ArgParseArgument arg(ArgParseArgument::STRING);
    setValidValues(arg, "this that");

    _checkValue(arg, "this");
    _assignArgumentValue(arg, "this");
    _checkValue(arg);
    SEQAN_ASSERT_EQ(getArgumentValue(arg), "this");
    // Test _checkValue() with argument and value.
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(arg, "not-this-or-that"),
                         "the given value 'not-this-or-that' is not in the list of allowed values [this, that]");
    // Test _checkValue after assignment.
    _assignArgumentValue(arg, "not-this-or-that");
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(arg),
                         "the given value 'not-this-or-that' is not in the list of allowed values [this, that]");

    ArgParseArgument filearg(ArgParseArgument::INPUTFILE);
    setValidValues(filearg, ".txt .fasta");

    _assignArgumentValue(filearg, "textfile.txt");
    SEQAN_ASSERT_EQ(value(filearg.value, 0), "textfile.txt");

    // Test getFileExtension() function.
    SEQAN_ASSERT_EQ(getFileExtension(filearg), ".txt");

    // different case should also work
    _assignArgumentValue(filearg, "textfile.tXT");
    SEQAN_ASSERT_EQ(value(filearg.value, 0), "textfile.tXT");

    // Test getFileExtension() function.
    SEQAN_ASSERT_EQ(getFileExtension(filearg), ".txt");

    // Test getFileExtension() function with explicit file extension.
    filearg._fileExtensions.push_back(".fa");
    SEQAN_ASSERT_EQ(getFileExtension(filearg), ".fa");
    SEQAN_ASSERT_EQ(getFileExtension(filearg, 0), ".fa");

    // Test _checkValue() with argument and value.
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(filearg, "not-a-validfile.qxt"),
                         "the given path 'not-a-validfile.qxt' does not have one of the valid file extensions [*.txt, *.fasta]; the file extension was overridden to be '.fa'");
    // Test _checkValue after assignment.
    _assignArgumentValue(filearg, "not-a-validfile.qxt");
    SEQAN_TEST_EXCEPTION(ParseError,
                         _checkValue(filearg),
                         "the given path 'not-a-validfile.qxt' does not have one of the valid file extensions [*.txt, *.fasta]; the file extension was overridden to be '.fa'");
}

#endif // SEQAN_CORE_TESTS_ARG_PARSE_TEST_ARG_PARSE_ARGUMENT_H_
