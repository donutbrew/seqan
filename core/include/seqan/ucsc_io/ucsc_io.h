// ==========================================================================
//                 SeqAn - The Library for Sequence Analysis
// ==========================================================================
// Copyright (c) 2006-2014, Knut Reinert, FU Berlin
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
// Author: David Weese <david.weese@fu-berlin.de>
// Author: Manuel Holtgrewe <manuel.holtgrewe@fu-berlin.de>
// ==========================================================================

#ifndef SEQAN_CORE_INCLUDE_SEQAN_UCSC_UCSC_IO_H_
#define SEQAN_CORE_INCLUDE_SEQAN_UCSC_UCSC_IO_H_

namespace seqan
{

// ============================================================================
// Forwards
// ============================================================================

// ============================================================================
// Tags, Classes, Enums
// ============================================================================

// ----------------------------------------------------------------------------
// Tag Ucsc
// ----------------------------------------------------------------------------

template <typename TSpec = void>
struct Ucsc_;

/*!
 * @defgroup UcscFileIO
 * @brief Tags for UCSC I/O.
 */

/*!
 * @tag UcscFileIO#Ucsc
 * @headerfile <seqan/ucsc_io.h>
 * @brief UCSC Genome browser annotation file (aka knownGene format).
 *
 * @signature typedef Tag<Ucsc_<UcscKnownGene_> > const Ucsc;
 */

struct UcscKnownGene_;
typedef Tag<Ucsc_<UcscKnownGene_> > UcscKnownGene;

// ----------------------------------------------------------------------------
// Tag Ucsc
// ----------------------------------------------------------------------------

/*!
 * @tag UcscFileIO#UcscIsoforms
 * @headerfile <seqan/ucsc_io.h>
 * @brief UCSC Genome browser isoform file (aka knownIsoforms format).
 *
 * @signature typedef Tag<Ucsc_<UcscIsoforms_> > const UcscIsoforms;
 */

struct UcscKnownIsoforms_;
typedef Tag<Ucsc_<UcscKnownIsoforms_> > UcscKnownIsoforms;

// ----------------------------------------------------------------------------
// Class UcscIOContext
// ----------------------------------------------------------------------------

/*!
 * @class UcscIOContext
 * @headerfile <sean/ucsc_io.h>
 * @brief Context to use for UCSC file I/O.
 *
 * @signature struct UcscIOContext;
 */
struct UcscIOContext
{
    /*!
     * @var CharString UcscIOContext::buffer
     * @brief Buffer used during UCSC I/O.
     */
    CharString buffer;
};

// ============================================================================
// Metafunctions
// ============================================================================

// ----------------------------------------------------------------------------
// Metafunction FileExtensions
// ----------------------------------------------------------------------------

template <typename T>
struct FileExtensions<UcscKnownGene, T>
{
    static char const * VALUE[1];  // default is one extension
};

template <typename T>
char const * FileExtensions<UcscKnownGene, T>::VALUE[1] =
{
    "knownGene.txt"         // default output extension
};


template <typename T>
struct FileExtensions<UcscKnownIsoforms, T>
{
    static char const * VALUE[1];  // default is one extension
};

template <typename T>
char const * FileExtensions<UcscKnownIsoforms, T>::VALUE[1] =
{
    "knownIsoforms.txt"     // default output extension
};

// ============================================================================
// Functions
// ============================================================================

// ----------------------------------------------------------------------------
// Function readRecod
// ----------------------------------------------------------------------------

/*!
 * @fn UcscFileIO#readRecord
 * @headerfile <seqan/ucsc_io.h>
 * @brief Low-level reading for @link UcscRecord @endlink.
 *
 * @signature void recordRecord(record, ucscIOContext, iter, tag);
 *
 * @param[out]    record       The @link UcscRecord @endlink to store results in.
 * @param[in,out] uscscContext The @link UcscIOContext @endlink to use during the rading.
 * @param[in,out] iter         The @link ForwadIteratorConcept forward iterator @endlink to read from.
 * @param[in]     tag          Fixed to @link Ucsc @endlink.
 *
 * @throw IOError    in the case of I/O errors
 * @throw ParseError in the case of problems with parsing
 */
template <typename TForwardIter>
void readRecord(UcscRecord & record,
                UcscIOContext & /*ucscIOContext*/,
                TForwardIter & iter,
                UcscKnownIsoforms const & /*tag*/)
{
    OrFunctor<IsTab, AssertFunctor<NotFunctor<IsNewline>, ParseError, UcscKnownIsoforms> > nextRecord;
    OrFunctor<IsNewline, AssertFunctor<NotFunctor<IsTab>, ParseError, UcscKnownIsoforms> > lastRecord;

    clear(record);

    // skip comments
    while (!atEnd(iter) && value(iter) == '#')
        skipLine(iter);

    // read column 1: gene name (stored in .transName)
    readUntil(record.transName, iter, nextRecord);
    if (empty(record.transName))
        SEQAN_THROW(EmptyFieldError("clusterId"));
    insert(record.transName, 0, "GENE");    // prepend "GENE" for compatibility reasons
    skipOne(iter, IsTab());

    // read column 2: transcript name (stored in .contigName)
    readUntil(record.contigName, iter, lastRecord);
    if (empty(record.contigName))
        SEQAN_THROW(EmptyFieldError("transcript"));
    skipLine(iter);
}

template <typename TForwardIter>
void readRecord(UcscRecord & record,
                UcscIOContext & ucscIOContext,
                TForwardIter & iter,
                UcscKnownGene const & /*tag*/)
{
    OrFunctor<IsTab, AssertFunctor<NotFunctor<IsNewline>, ParseError, UcscKnownGene> > nextRecord;
    OrFunctor<IsNewline, AssertFunctor<NotFunctor<IsTab>, ParseError, UcscKnownGene> > lastRecord;

    clear(record);

    CharString & buffer = ucscIOContext.buffer;

    // skip comments
    while (!atEnd(iter) && value(iter) == '#')
        skipLine(iter);

    // read column 1: transcript name
    readUntil(record.transName, iter, nextRecord);
    if (empty(record.transName))
        SEQAN_THROW(EmptyFieldError("name"));
    skipOne(iter, IsTab());

    // read column 2: contig name
    readUntil(record.contigName, iter, nextRecord);
    if (empty(record.contigName))
        SEQAN_THROW(EmptyFieldError("chrom"));
    skipOne(iter, IsTab());

    // read column 3: orientation
    char orientation;
    readOne(orientation, iter, OrFunctor<EqualsChar<'+'>, EqualsChar<'-'> >());
    skipOne(iter, IsTab());

    // read column 4: transcript begin position
    clear(buffer);
    readUntil(buffer, iter, nextRecord);
    record.annotationBeginPos = lexicalCast<__uint32>(buffer);
    skipOne(iter, IsTab());

    // read column 5: transcript end position
    clear(buffer);
    readUntil(buffer, iter, nextRecord);
    record.annotationEndPos = lexicalCast<__uint32>(buffer);
    skipOne(iter, IsTab());

    // read column 6: CDS begin position
    clear(buffer);
    readUntil(buffer, iter, nextRecord);
    record.cdsBegin = lexicalCast<__uint32>(buffer);
    skipOne(iter, IsTab());

    // read column 7: CDS end position
    clear(buffer);
    readUntil(buffer, iter, nextRecord);
    record.cdsEnd = lexicalCast<__uint32>(buffer);
    skipOne(iter, IsTab());

    // read column 8: exon count
    unsigned exons;
    clear(buffer);
    readUntil(buffer, iter, nextRecord);
    lexicalCastWithException(exons, buffer);
    skipOne(iter, IsTab());

    // read column 9: exon begin positions
    for (unsigned i = 0; i < exons; ++i)
    {
        clear(buffer);
        readUntil(buffer, iter, OrFunctor<OrFunctor<EqualsChar<';'>, EqualsChar<','> >, AssertFunctor<NotFunctor<IsNewline>, ParseError, UcscKnownGene> >());

        __int32 tempBegin;
        lexicalCastWithException(tempBegin, buffer);
        appendValue(record.exonBegin, tempBegin);
        skipOne(iter);
    }
    skipOne(iter, IsTab());

    // read column 10: exon end positions
    for (unsigned int i = 0; i < exons; ++i)
    {
        clear(buffer);
        readUntil(buffer, iter, OrFunctor<OrFunctor<EqualsChar<';'>, EqualsChar<','> >, AssertFunctor<NotFunctor<IsNewline>, ParseError, UcscKnownGene> >());

        __int32 tempEnd;
        lexicalCastWithException(tempEnd, buffer);
        appendValue(record.exonEnds, tempEnd);
        skipOne(iter);
    }
    skipOne(iter, IsTab());

    // read column 11: protein name
    readUntil(record.proteinName, iter, nextRecord);
    skipOne(iter, IsTab());

    // skip column 12: align id
    skipUntil(iter, lastRecord);
    skipLine(iter);

    // adapt positions
    if (orientation == '-')
    {
        std::swap(record.annotationBeginPos, record.annotationEndPos);
        std::swap(record.cdsBegin, record.cdsEnd);
        for (unsigned i = 0; i < exons; ++i)
            std::swap(record.exonBegin[i], record.exonEnds[i]);
    }
}

// ----------------------------------------------------------------------------
// Function writeRecord
// ----------------------------------------------------------------------------

/*!
 * @fn UcscFileIO#writeRecord
 * @headerfile <seqan/ucsc_io.h>
 * @brief Low-level writing of @link UcscRecord @endlink.
 *
 * @signature void writeRecord(target, record, tag);
 *
 * @param[in,out] target @link OutputIteratorConcept Output iterator @endlink or @link ContainerConcept container
 *                       @endlink to write to.
 * @param[in]     record @link UcscRecord @endlink to write.
 * @param[in]     tag    Fixed to @link Ucsc @endlink.
 *
 * @throw IOError in case of I/O problems
 */

template <typename TTarget>
void writeRecord(TTarget & target,
                 UcscRecord const & record,
                 UcscKnownIsoforms const & /*tag*/)
{
    unsigned suf = 0;
    if (length(record.transName) >= 4 && prefix(record.transName, 4) == "GENE")
        suf = 4;

    // write column 1: transcript name
    // The letters until the first whitespace will be write.
    // Then, we skip until we hit the first tab character.
    write(target, suffix(record.transName, suf));
    writeValue(target, '\t');

    // write column 2: contig name
    write(target, record.contigName);
    writeValue(target, '\n');
}

template <typename TTarget>
void writeRecord(TTarget & target,
                 UcscRecord const & record,
                 UcscKnownGene const & /*tag*/)
{
    // write column 1: transcript name
    // The letters until the first whitespace will be write.
    // Then, we skip until we hit the first tab character.
    write(target, record.transName);
    writeValue(target, '\t');

    // write column 2: contig name
    write(target, record.contigName);
    writeValue(target, '\t');

    // write column 3: orientation
    __uint32 transBeginPos, transEndPos;
    __uint32 cdsBeginPos, cdsEndPos;
    if (record.annotationBeginPos < record.annotationEndPos)
    {
        writeValue(target, '+');
        transBeginPos = record.annotationBeginPos;
        transEndPos = record.annotationEndPos;
        cdsBeginPos = record.cdsBegin;
        cdsEndPos = record.cdsEnd;
    }
    else
    {
        writeValue(target, '-');
        transEndPos = record.annotationBeginPos;
        transBeginPos = record.annotationEndPos;
        cdsEndPos = record.cdsBegin;
        cdsBeginPos = record.cdsEnd;
    }
    writeValue(target, '\t');

    // write column 4: transcript begin position
    appendNumber(target, transBeginPos);
    writeValue(target, '\t');

    // write column 5: transcript end position
    appendNumber(target, transEndPos);
    writeValue(target, '\t');

    // write column 6: CDS begin position
    appendNumber(target, cdsBeginPos);
    writeValue(target, '\t');

    // write column 7: CDS end position
    appendNumber(target, cdsEndPos);
    writeValue(target, '\t');

    // write column 8: exon count
    appendNumber(target, length(record.exonBegin));
    writeValue(target, '\t');

    // write column 9: exon begin positions
    for (unsigned i = 0; i < length(record.exonBegin); ++i)
    {
        appendNumber(target, std::min(record.exonBegin[i], record.exonEnds[i]));
        writeValue(target, ',');
    }
    writeValue(target, '\t');

    // write column 10: exon end positions
    for (unsigned i = 0; i < length(record.exonBegin); ++i)
    {
        appendNumber(target, std::max(record.exonBegin[i], record.exonEnds[i]));
        writeValue(target, ',');
    }
    writeValue(target, '\t');

    // write column 11: protein name
    write(target, record.proteinName);
    writeValue(target, '\t');

    // write column 12: align id
    write(target, record.transName);
    writeValue(target, '\n');
}

}  // namespace seqan

#endif  // #ifndef SEQAN_CORE_INCLUDE_SEQAN_UCSC_UCSC_IO_H_