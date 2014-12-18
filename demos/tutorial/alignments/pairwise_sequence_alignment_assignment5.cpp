//![main]
#include <iostream>
#include <seqan/align.h>

using namespace seqan;

int main()
{
    typedef String<char> TSequence;
    typedef StringSet<TSequence> TStringSet;
    typedef Gaps<TSequence, ArrayGaps> TGaps;
    typedef Iterator<TGaps>::Type TGapsIterator;
    typedef Iterator<String<int> >::Type TIterator;

    TSequence text =    "MISSISSIPPIANDMISSOURI";
    TSequence pattern = "SISSI";
//![main]

//![verification]
    String<int> locations;
    for (unsigned i = 0; i < length(text) - length(pattern); ++i)
    {
        // Compute the MyersBitVector in current window of text.
        TSequence tmp = infix(text, i, i + length(pattern));

        // Report hits with at most 2 errors.
        if (globalAlignmentScore(tmp, pattern, MyersBitVector()) >= -2)
        {
            appendValue(locations, i);
        }
    }
//![verification]

//![alignment]
    TGaps gapsText;
    TGaps gapsPattern;
    assignSource(gapsPattern, pattern);
    std::cout << "Text: " << text << "\tPattern: " << pattern << std::endl;
    for (TIterator it = begin(locations); it != end(locations); ++it)
    {
        // Clear previously computed gaps.
        clearGaps(gapsText);
        clearGaps(gapsPattern);
        // Only recompute the area within the current window over the text.
        TSequence textInfix = infix(text, *it, *it + length(pattern));
        assignSource(gapsText, textInfix);

        // Use semi-global alignment since we do not want to track leading/trailing gaps in the pattern.
        // Restirct search space using a band allowing at most 2 errors in vertical/horizontal direction.
        int score = globalAlignment(gapsText, gapsPattern, Score<int>(0, -1, -1), AlignConfig<true, false, false, true>(), -2, 2);
//![alignment]

//![cigar]
        TGapsIterator itGapsPattern = begin(gapsPattern);
        TGapsIterator itGapsEnd = end(gapsPattern);

        // Remove trailing gaps in pattern.
        int count = 0;
        while (isGap(--itGapsEnd))
            ++count;
        setClippedEndPosition(gapsPattern, length(gapsPattern) - count);

        // Remove leading gaps in pattern.
        if (isGap(itGapsPattern))
        {
            setClippedBeginPosition(gapsPattern, countGaps(itGapsPattern));
            setClippedBeginPosition(gapsText, countGaps(itGapsPattern));
        }

        // Reinitilaize the iterators.
        TGapsIterator itGapsText = begin(gapsText);
        itGapsPattern = begin(gapsPattern);
        itGapsEnd = end(gapsPattern);

        // Use a stringstream to construct the cigar string.
        std::stringstream cigar;
        int numChar = 0;
        while (itGapsPattern != itGapsEnd)
        {
//![cigar]
//![cigarInsertion]
            // Count insertions.
            if (isGap(itGapsText))
            {
                int numGaps = countGaps(itGapsText);
                cigar << numGaps << "I";
                itGapsText += numGaps;
                itGapsPattern += numGaps;
                continue;
            }
//![cigarInsertion]
//![cigarDeletion]
            // Count deletions.
            if (isGap(itGapsPattern))
            {
                int numGaps = countGaps(itGapsPattern);
                cigar << numGaps << "D";
                itGapsText += numGaps;
                itGapsPattern += numGaps;
                continue;
            }
//![cigarDeletion]
//![cigarMatch]
            // Count matches.
            while (*itGapsText == *itGapsPattern && itGapsPattern != itGapsEnd)
            {
                ++numChar;
                ++itGapsText;
                ++itGapsPattern;
            }
            if (numChar != 0)
            {
                cigar << numChar << "M";
                numChar = 0;
                continue;
            }
//![cigarMatch]
//![cigarMismatch]
            // Count mismatches.
            while (*itGapsText != *itGapsPattern && itGapsPattern != itGapsEnd)
            {
                ++numChar;
                ++itGapsText;
                ++itGapsPattern;
            }
            if (numChar != 0)
                cigar << numChar << "S";
            numChar = 0;
        }
        // Output the hit position in the text, the total number of edits and the corresponding cigar string.
        std::cout << "Hit at position  " << *it << "\ttotal edits: " << abs(score) << "\tcigar: " << cigar.str() << std::endl;
    }

    return 0;
}
//![cigarMismatch]

