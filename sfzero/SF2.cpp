/*************************************************************************************
 * Original code copyright (C) 2012 Steve Folta
 * Converted to Juce module (C) 2016 Leo Olivers
 * Forked from https://github.com/stevefolta/SFZero
 * For license info please see the LICENSE file distributed with this source code
 *************************************************************************************/
#include "SF2.h"
#include "RIFF.h"

#define readAbyte(name, file) name = (byte)file->readByte();
#define readAchar(name, file) name = file->readByte();
#define readAdword(name, file) name = (dword)file->readInt();
#define readAword(name, file) name = (word)file->readShort();
#define readAshort(name, file) name = file->readShort();
#define readAchar20(name, file) file->read(name, 20);
#define readAgenAmountType(name, file) name.shortAmount = file->readShort();

#define SF2Field(type, name) readA##type(name, file)

void sfzero::SF2::iver::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/iver.h"
    SF2Field(word, major);
    SF2Field(word, minor);
}

void sfzero::SF2::phdr::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/phdr.h"
    SF2Field(char20, presetName)
    SF2Field(word, preset)
    SF2Field(word, bank)
    SF2Field(word, presetBagNdx)
    SF2Field(dword, library)
    SF2Field(dword, genre)
    SF2Field(dword, morphology)
}

void sfzero::SF2::pbag::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/pbag.h"
    SF2Field(word, genNdx)
    SF2Field(word, modNdx)
}

void sfzero::SF2::pmod::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/pmod.h"
    SF2Field(word, modSrcOper)
    SF2Field(word, modDestOper)
    SF2Field(short, modAmount)
    SF2Field(word, modAmtSrcOper)
    SF2Field(word, modTransOper)
}

void sfzero::SF2::pgen::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/pgen.h"
    SF2Field(word, genOper)
    SF2Field(genAmountType, genAmount)
}

void sfzero::SF2::inst::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/inst.h"
    SF2Field(char20, instName)
    SF2Field(word, instBagNdx)
}

void sfzero::SF2::ibag::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/ibag.h"
    SF2Field(word, instGenNdx)
    SF2Field(word, instModNdx)
}

void sfzero::SF2::imod::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/imod.h"
    SF2Field(word, modSrcOper)
    SF2Field(word, modDestOper)
    SF2Field(short, modAmount)
    SF2Field(word, modAmtSrcOper)
    SF2Field(word, modTransOper)
}

void sfzero::SF2::igen::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/igen.h"
    SF2Field(word, genOper)
    SF2Field(genAmountType, genAmount)
}

void sfzero::SF2::shdr::readFrom(juce::InputStream *file)
{
//#include "sf2-chunks/shdr.h"
    SF2Field(char20, sampleName)
    SF2Field(dword, start)
    SF2Field(dword, end)
    SF2Field(dword, startLoop)
    SF2Field(dword, endLoop)
    SF2Field(dword, sampleRate)
    SF2Field(byte, originalPitch)
    SF2Field(char, pitchCorrection)
    SF2Field(word, sampleLink)
    SF2Field(word, sampleType)
}

sfzero::SF2::Hydra::Hydra() //:
//phdrItems(nullptr),
//pbagItems(nullptr),
//pmodItems(nullptr),
//pgenItems(nullptr),
//instItems(nullptr),
//ibagItems(nullptr),
//imodItems(nullptr),
//igenItems(nullptr),
//shdrItems(nullptr),
//phdrNumItems(0),
//pbagNumItems(0),
//pmodNumItems(0),
//pgenNumItems(0),
//instNumItems(0),
//ibagNumItems(0),
//imodNumItems(0),
//igenNumItems(0),
//shdrNumItems(0)
{
}

sfzero::SF2::Hydra::~Hydra()
{
//    delete phdrItems;
//    delete pbagItems;
//    delete pmodItems;
//    delete pgenItems;
//    delete instItems;
//    delete ibagItems;
//    delete imodItems;
//    delete igenItems;
//    delete shdrItems;
}

void sfzero::SF2::Hydra::readFromOld(juce::InputStream *file, juce::int64 pdtaChunkEnd)
{
#if false
    int i, numItems;
    
#define HandleChunk(chunkName)                              \
    if (FourCCEquals(chunk.id, #chunkName))                 \
    {                                                       \
        numItems = chunk.size / SF2::chunkName::sizeInFile; \
        chunkName##NumItems = numItems;                     \
        chunkName##Items = new SF2::chunkName[numItems];    \
        for (i = 0; i < numItems; ++i)                      \
        {                                                   \
            chunkName##Items[i].readFrom(file);             \
        }                                                   \
    }                                                       \
    else
    
    while (file->getPosition() < pdtaChunkEnd)
    {
        sfzero::RIFFChunk chunk;
        chunk.readFrom(file);
        
        HandleChunk(phdr) HandleChunk(pbag) HandleChunk(pmod) HandleChunk(pgen) HandleChunk(inst) HandleChunk(ibag) HandleChunk(imod)
        HandleChunk(igen) HandleChunk(shdr)
        {
        }
        chunk.seekAfter(file);
    }
#endif
}

void sfzero::SF2::Hydra::readFrom(juce::InputStream* file, juce::int64 pdtaChunkEnd)
{
    auto check = [](sfzero::RIFFChunk& chunk, sfzero::fourcc chunkName)
    {
        sfzero::fourcc& chunkID = chunk.id;
        return
        chunkID[0] == chunkName[0] &&
        chunkID[1] == chunkName[1] &&
        chunkID[2] == chunkName[2] &&
        chunkID[3] == chunkName[3];
    };
    
    sfzero::fourcc phdrType = {'p','h','d','r'};
    sfzero::fourcc pbagType = {'p','b','a','g'};
    sfzero::fourcc pmodType = {'p','m','o','d'};
    sfzero::fourcc pgenType = {'p','g','e','n'};
    sfzero::fourcc instType = {'i','n','s','t'};
    sfzero::fourcc ibagType = {'i','b','a','g'};
    sfzero::fourcc imodType = {'i','m','o','d'};
    sfzero::fourcc igenType = {'i','g','e','n'};
    sfzero::fourcc shdrType = {'s','h','d','r'};
    
    while( file->getPosition() < pdtaChunkEnd )
    {
        sfzero::RIFFChunk chunk;
        chunk.readFrom(file);
        
        if( check(chunk, phdrType ) )
        {
            readHelper(chunk, phdrItems, file);
        }
        else if( check(chunk, pbagType ) )
        {
            readHelper(chunk, pbagItems, file);
        }
        else if( check(chunk, pmodType ) )
        {
            readHelper(chunk, pmodItems, file);
        }
        else if( check(chunk, pgenType ) )
        {
            readHelper(chunk, pgenItems, file);
        }
        else if( check(chunk, instType) )
        {
            readHelper(chunk, instItems, file);
        }
        else if( check(chunk, ibagType ) )
        {
            readHelper(chunk, ibagItems, file);
        }
        else if( check(chunk, imodType) )
        {
            readHelper(chunk, imodItems, file);
        }
        else if( check(chunk, igenType ) )
        {
            readHelper(chunk, igenItems, file);
        }
        else if( check(chunk, shdrType ) )
        {
            readHelper(chunk, shdrItems, file);
        }
        
        chunk.seekAfter(file);
    }
}

bool sfzero::SF2::Hydra::isComplete()
{
    return
    !phdrItems.empty() &&
    !pbagItems.empty() &&
    !pmodItems.empty() &&
    !pgenItems.empty() &&
    !instItems.empty() &&
    !ibagItems.empty() &&
    !imodItems.empty() &&
    !igenItems.empty() &&
    !shdrItems.empty();
}
